/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 13:48:14 by bliu              #+#    #+#             */
/*   Updated: 2025/12/26 16:12:30 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static double	bump_map_height(t_bump_tex *tex, double u, double v)
{
	int				x;
	int				y;
	unsigned int	pixel;

	x = (int)((1 - u) * (tex->width - 1));
	if (x < 0)
		x = 0;
	else if (x >= tex->width)
		x = tex->width - 1;
	y = (int)((v) * (tex->height - 1));
	if (y < 0)
		y = 0;
	else if (y >= tex->height)
		y = tex->height - 1;
	pixel = *(unsigned int *)(tex->addr
			+ y * tex->line_len
			+ x * (tex->bpp / 8));
	return ((pixel & 0xFF) / 255.0);
}

t_vec3	bump_tangent_normal(t_bump_tex *tex, double u, double v,
	double strength)
{
	double	du;
	double	dv;
	double	height;
	double	height_u;
	double	height_v;

	height = bump_map_height((t_bump_tex *)tex, u, v);
	height_u = bump_map_height((t_bump_tex *)tex, u + 0.001, v);
	height_v = bump_map_height((t_bump_tex *)tex, u, v + 0.001);
	du = (height_u - height) * strength;
	dv = (height_v - height) * strength;
	return (vec3_norm((t_vec3){du, dv, 1.0}));
}

t_vec3	apply_bump_map(t_tbn tbn, t_vec3 Nt)
{
	t_vec3	bumped;

	bumped = vec3_add(vec3_mul_n(tbn.ct, Nt.x),
			vec3_add(vec3_mul_n(tbn.cb, Nt.y), vec3_mul_n(tbn.cn, Nt.z)));
	return (vec3_norm(bumped));
}
