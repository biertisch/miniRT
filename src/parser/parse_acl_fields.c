/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_acl_fields.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:28:11 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 14:54:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	add_light_to_world(t_world *world, t_s_light *light)
{
	t_s_light	*new_light;

	if (world->num_lights >= MAX_LIGHTS)
	{
		ft_putstr_fd("Error\nSpot light limit reached\n", STDERR_FILENO);
		return ;
	}
	new_light = malloc(sizeof(t_s_light));
	if (!new_light)
		error_exit(world, "Error\nmalloc");
	new_light->position = light->position;
	new_light->brightness = light->brightness;
	new_light->color = light->color;
	world->lights[world->num_lights] = new_light;
	world->num_lights++;
}

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
	t_s_light		light;
	t_field_rule	*fields;

	fields = meta->rule->fields;
	if (!parse_vec3(&light.position, s, &fields[0], meta))
		return (0);
	if (!parse_float(&light.brightness, s, &fields[1], meta))
		return (0);
	if (!parse_color(&light.color, s, &fields[2], meta))
		return (0);
	light.color = norm_color(light.color);
	add_light_to_world(scene, &light);
	return (1);
}
