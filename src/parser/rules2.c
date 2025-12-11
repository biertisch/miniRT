/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:48:06 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 12:56:39 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_field_rule	*get_sphere_fields(void)
{
	static t_field_rule	fields[] = {
		{"center", FIELD_VEC3, 0, 0},
		{"radius", FIELD_FLOAT, 0.0001, 1e6},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

t_field_rule	*get_plane_fields(void)
{
	static t_field_rule	fields[] = {
		{"point", FIELD_VEC3, 0, 0},
		{"normal", FIELD_VEC3, -1, 1},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

t_field_rule	*get_cylinder_fields(void)
{
	static t_field_rule	fields[] = {
		{"center", FIELD_VEC3, 0, 0},
		{"axis", FIELD_VEC3, -1, 1},
		{"radius", FIELD_FLOAT, 0.0001, 1e6},
		{"height", FIELD_FLOAT, 0.0001, 1e6},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

t_field_rule	*get_cone_fields(void)
{
	static t_field_rule	fields[] = {
		{"apex", FIELD_VEC3, 0, 0},
		{"axis", FIELD_VEC3, -1, 1},
		{"radius", FIELD_FLOAT, 0.0001, 1e6},
		{"height", FIELD_FLOAT, 0.0001, 1e6},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}
