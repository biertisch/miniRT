/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:36:25 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:55:08 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_sphere_fields(t_world *scene, char **s, t_metadata *meta)
{
	t_sphere	sphere;
	t_material	mat;
	t_color		color;

	if (!parse_color(&color, s, meta))
		return (0);
	mat = get_material(LAMBERTIAN, color, 0);
	sphere = new_sphere((t_vec3){0, 0, 0}, 0, mat);
	if (!parse_vec3(&sphere.center, s, meta))
		return (0);
	if (!parse_float(&sphere.radius, s, meta, 4))
		return (0);
	add_object_to_world(scene, SPHERE, &sphere);
	return (1);
}

int	parse_plane_fields(t_world *scene, char **s, t_metadata *meta)
{
	t_plane		plane;
	t_material	mat;
	t_color		color;

	if (!parse_color(&color, s, meta))
		return (0);
	mat = get_material(LAMBERTIAN, color, 0);
	plane = new_plane((t_vec3){0, 0, 0}, 0, mat);
	if (!parse_vec3(&plane.point, s, meta))
		return (0);
	if (!parse_vec3(&plane.normal, s, meta))
		return (0);
	add_object_to_world(scene, PLANE, &plane);
	return (1);
}
