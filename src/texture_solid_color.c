/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_solid_color.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 17:20:37 by bliu              #+#    #+#             */
/*   Updated: 2025/12/24 03:24:34 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_solid_color_tex	solid_color_texture(t_color color)
{
	t_solid_color_tex	texture;

	texture.base.value = &solid_color_value;
	texture.base.type = SOLID_COLOR;
	texture.albedo = color;
	return (texture);
}

t_solid_color_tex	*solid_color_texture_ptr(t_color color)
{
	t_solid_color_tex	*texture;

	texture = malloc(sizeof(t_solid_color_tex));
	texture->base.value = &solid_color_value;
	texture->base.type = SOLID_COLOR;
	texture->albedo = color;
	return (texture);
}

t_color	solid_color_value(t_texture *self, double u, double v, t_vec3 p)
{
	t_solid_color_tex	*texture;

	(void)u;
	(void)v;
	(void)p;
	texture = (t_solid_color_tex *)self;
	return (texture->albedo);
}
