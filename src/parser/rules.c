/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:45:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/30 01:08:08 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_field_rule	*get_ambient_fields(void)
{
	static t_field_rule	fields[] = {
	{"ratio", FIELD_FLOAT, 0.0, 1.0, 2},
	{"color", FIELD_COLOR, 0, 255, 0}
	};

	return (fields);
}

static t_field_rule	*get_camera_fields(void)
{
	static t_field_rule	fields[] = {
	{"origin", FIELD_VEC3, 0, 0, 2},
	{"orientation", FIELD_VEC3, -1, 1, 2},
	{"fov", FIELD_INT, 0, 180, 0}
	};

	return (fields);
}

static t_field_rule	*get_light_fields(void)
{
	static t_field_rule	fields[] = {
	{"position", FIELD_VEC3, 0, 0, 2},
	{"brightness", FIELD_FLOAT, 0, 1, 2},
	{"color", FIELD_COLOR, 0, 255, 0}
	};

	return (fields);
}

static t_object_rule	*init_rule(t_object_rule *rule, const char *keyword,
	t_field_rule *fields, int field_count)
{
	rule->keyword = (char *)keyword;
	rule->fields = fields;
	rule->field_count = field_count;
	return (rule);
}

t_object_rule	*find_rule(const char *keyword)
{
	static t_object_rule	rule;

	if (ft_strcmp(keyword, "A") == 0)
		return (init_rule(&rule, "A", get_ambient_fields(), 2));
	else if (ft_strcmp(keyword, "C") == 0)
		return (init_rule(&rule, "C", get_camera_fields(), 3));
	else if (ft_strcmp(keyword, "L") == 0)
		return (init_rule(&rule, "L", get_light_fields(), 3));
	else if (ft_strcmp(keyword, "sp") == 0)
		return (init_rule(&rule, "sp", get_sphere_fields(), 3));
	else if (ft_strcmp(keyword, "pl") == 0)
		return (init_rule(&rule, "pl", get_plane_fields(), 3));
	else if (ft_strcmp(keyword, "cy") == 0)
		return (init_rule(&rule, "cy", get_cylinder_fields(), 5));
	else if (ft_strcmp(keyword, "co") == 0)
		return (init_rule(&rule, "co", get_cone_fields(), 5));
	else if (ft_strcmp(keyword, "tr") == 0)
		return (init_rule(&rule, "tr", get_triangle_fields(), 4));
	return (NULL);
}
