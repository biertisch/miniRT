/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:45:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 15:29:36 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_field_rule	*get_ambient_fields(void)
{
	static t_field_rule	fields[] = {
		{"ratio", FIELD_FLOAT, 0.0, 1.0},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

static t_field_rule	*get_camera_fields(void)
{
	static t_field_rule	fields[] = {
		{"origin", FIELD_VEC3, 0, 0},
		{"orientation", FIELD_VEC3, -1, 1},
		{"fov", FIELD_FLOAT, 0, 180} // change to int
	};

	return (fields);
}

static t_field_rule	*get_light_fields(void)
{
	static t_field_rule	fields[] = {
		{"pos", FIELD_VEC3, 0, 0},
		{"bright", FIELD_FLOAT, 0, 1},
		{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

static t_object_rule	*initialize_object_rule(t_object_rule *rule, const char *keyword,
	t_field_rule *fields, int field_count)
{
	rule->keyword = (char *)keyword;
	rule->fields = fields;
	rule->field_count = field_count;
	return (rule);
}

t_object_rule	*find_rule(const char *keyword)
{
	static	t_object_rule	rule;

	if (ft_strcmp(keyword, "A") == 0)
		return (initalize_object_rule(&rule, "A", get_ambient_fields(), 2));
	else if (ft_strcmp(keyword, "C") == 0)
		return (initialize_object_rule(&rule, "C", get_camera_fields(), 3));
	else if (ft_strcmp(keyword, "L") == 0)
		return (initialize_object_rule(&rule, "L", get_light_fields(), 3));
	else if (ft_strcmp(keyword, "sp") == 0)
		return (initialize_object_rule(&rule, "sp", get_sphere_fields(), 3));
	else if (ft_strcmp(keyword, "pl") == 0)
		return (initialize_object_rule(&rule, "pl", get_plane_fields(), 3));
	else if (ft_strcmp(keyword, "cy") == 0)
		return (initialize_object_rule(&rule, "cy", get_cylinder_fields(), 5));
	else if (ft_strcmp(keyword, "co") == 0)
		return (initialize_object_rule(&rule, "co", get_cone_fields(), 5));
	return (NULL);
}
