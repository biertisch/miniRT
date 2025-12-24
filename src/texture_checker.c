/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:08:03 by bliu              #+#    #+#             */
/*   Updated: 2025/12/24 03:24:13 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_checker_tex	checker_texture(double scale, t_texture *even, t_texture *odd)
{
	t_checker_tex	texture;

	texture.inv_scale = scale;
	texture.even = even;
	texture.odd = odd;
	texture.base.value = &checker_texture_value;
	texture.base.type = CHECKER;
	return (texture);
}

t_checker_tex	*checker_texture_colors(double scale, t_color even_c,
	t_color odd_c)
{
	t_checker_tex		*texture;
	t_solid_color_tex	*even_tex;
	t_solid_color_tex	*odd_tex;

	texture = malloc(sizeof(t_checker_tex));
	even_tex = malloc(sizeof(t_solid_color_tex));
	odd_tex = malloc(sizeof(t_solid_color_tex));
	*even_tex = solid_color_texture(even_c);
	*odd_tex = solid_color_texture(odd_c);
	*texture = checker_texture(scale, (t_texture *)even_tex,
			(t_texture *)odd_tex);
	return (texture);
}

t_color	checker_texture_value(t_texture *texture, double u, double v, t_vec3 p)
{
	int				check;
	t_checker_tex	*ct;

	ct = (t_checker_tex *)texture;
	check = ((int)floor(u * ct->inv_scale) + (int)floor(v * ct->inv_scale)) % 2;
	if (check == 0)
		return (texture_value(ct->even, u, v, p));
	else
		return (texture_value(ct->odd, u, v, p));
}

/*
t_color	checker_texture_value_old(t_texture *texture, 
	double u, double v, t_vec3 p)
{
	int				xInt;
	int				yInt;
	int				zInt;
	int				isEven;
	t_checker_tex	*checker_tex;

	checker_tex = (t_checker_tex *)texture;
	xInt = (int)(floor(p.x * checker_tex->inv_scale));
	yInt = (int)(floor(p.y * checker_tex->inv_scale));
	zInt = (int)(floor(p.z * checker_tex->inv_scale));
	isEven = (xInt + yInt + zInt) % 2 == 0;
	if (isEven)
		return (texture_value(checker_tex->even, u, v, p));
	else
		return (texture_value(checker_tex->odd, u, v, p));
}*/