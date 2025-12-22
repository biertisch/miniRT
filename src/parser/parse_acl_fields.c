/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_acl_fields.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:28:11 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/22 00:04:12 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_ambient_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_field_rule	*fields;

	fields = meta->rule->fields;
	if (!parse_float(&scene->ambient_ratio, s, &fields[0], meta))
		return (0);
	if (!parse_color(&scene->ambient, s, &fields[1], meta))
		return (0);
	scene->ambient = norm_color(scene->ambient);
	return (1);
}

int	parse_camera_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_field_rule	*fields;
	int				fov;

	fields = meta->rule->fields;
	if (!parse_vec3(&scene->camera.lookfrom, s, &fields[0], meta))
		return (0);
	if (!parse_vec3(&scene->camera.forword, s, &fields[1], meta))
		return (0);
	if (!parse_int(&fov, s, &fields[2], meta))
		return (0);
	scene->camera.vfov = (double)fov;
	return (1);
}

int	parse_light_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_field_rule	*fields;

	fields = meta->rule->fields;
	if (!parse_vec3(&scene->spot_light.position, s, &fields[0], meta))
		return (0);
	if (!parse_float(&scene->spot_light.brightness, s, &fields[1], meta))
		return (0);
	if (!parse_color(&scene->spot_light.color, s, &fields[2], meta))
		return (0);
	scene->spot_light.color = norm_color(scene->spot_light.color);
	return (1);
}
