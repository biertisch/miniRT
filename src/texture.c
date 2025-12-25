/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 17:18:25 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 17:14:17 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	texture_value(t_texture *self, double u, double v, t_vec3 p)
{
	return (self->value(self, u, v, p));
}

void	load_default_textures(t_world *wld)
{
	wld->pic_c_tex = picture_texture(load_xpm(wld->mlx,
				"textures/earthmap4k.xpm"));
	wld->pic_c_tex.bump_tex = load_xpm(wld->mlx,
			"textures/earthmap4kspecular.xpm");
}
