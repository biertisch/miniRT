/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object_fields2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:36:25 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/30 13:26:58 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_material	parse_material(const char **s, t_color color);

int	parse_triangle_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_triangle	triangle;
	t_color		color;

	if (!parse_vec3(&triangle.p1, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&triangle.p2, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_vec3(&triangle.p3, s, &meta->rule->fields[2], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[3], meta))
		return (0);
	triangle.mat = parse_material(s, color);
	triangle.e1 = vec3_sub(triangle.p2, triangle.p1);
	triangle.e2 = vec3_sub(triangle.p3, triangle.p1);
	triangle.normal = vec3_norm(vec3_cross(triangle.e1, triangle.e2));
	triangle.bbox = triangle_get_aabb(&triangle);
	add_object_to_world(scene, TRIANGLE, &triangle);
	return (1);
}
