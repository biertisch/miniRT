/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:36:25 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/12 20:11:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_sphere_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_sphere	sphere;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&sphere.center, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[1], meta))
		return (0);
	sphere.radius = diameter / 2;
	if (!parse_color(&color, s, &meta->rule->fields[2], meta))
		return (0);
	sphere.mat = get_material(LAMBERTIAN, color, 0);
	add_object_to_world(scene, SPHERE, &sphere);
	return (1);
}

int	parse_plane_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_plane		plane;
	t_color		color;

	if (!parse_vec3(&plane.point, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&plane.normal, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[2], meta))
		return (0);
	plane.mat = get_material(LAMBERTIAN, color, 0);
	add_object_to_world(scene, PLANE, &plane);
	return (1);
}

int	parse_cylinder_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_cylinder	cylinder;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&cylinder.center, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&cylinder.axis, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[2], meta))
		return (0);
	cylinder.radius = diameter / 2;
	if (!parse_float(&cylinder.height, s, &meta->rule->fields[3], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[4], meta))
		return (0);
	cylinder.mat = get_material(LAMBERTIAN, color, 0);
	add_object_to_world(scene, CYLINDER, &cylinder);
	return (1);
}

int	parse_cone_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_cone		cone;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&cone.apex, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&cone.axis, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[2], meta))
		return (0);
	cone.radius = diameter / 2;
	if (!parse_float(&cone.height, s, &meta->rule->fields[3], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[4], meta))
		return (0);
	cone.mat = get_material(LAMBERTIAN, color, 0);
	add_object_to_world(scene, CONE, &cone);
	return (1);
}
