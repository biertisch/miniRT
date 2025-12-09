/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:44:12 by bliu              #+#    #+#             */
/*   Updated: 2025/12/09 16:09:27 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
/*
// static t_field_rule g_ambient_fields[] =
// {
// 	{"ratio", FIELD_FLOAT, 0.0, 1.0},
// 	{"color", FIELD_COLOR, 0, 255}
// };

// static t_field_rule g_camera_fields[] = {
// 	{"origin", FIELD_VEC3, 0, 0},
// 	{"orientation", FIELD_VEC3, -1, 1},
// 	{"fov", FIELD_FLOAT, 0, 180}
// };

// static t_field_rule g_light_fields[] = {
// 	{"pos", FIELD_VEC3, 0, 0},
// 	{"bright", FIELD_FLOAT, 0, 1},
// 	{"color", FIELD_COLOR, 0, 255}
// };

// static t_field_rule g_sphere_fields[] = {
// 	{"center", FIELD_VEC3, 0, 0},
// 	{"radius", FIELD_FLOAT, 0.0001, 1e6},
// 	{"color", FIELD_COLOR, 0, 255}
// };

// static t_object_rule g_rules[] = {
// 	{"A", g_ambient_fields, 2},
// 	{"C", g_camera_fields, 3},
// 	{"L", g_light_fields, 3},
// 	{"sp", g_sphere_fields, 3},
// 	{NULL, NULL, 0}
// };

// t_object_rule   *find_rule(const char *keyword)
// {
// 	int i = 0;

// 	while (g_rules[i].keyword)
// 	{
// 		if (strcmp(g_rules[i].keyword, keyword) == 0)
// 			return (&g_rules[i]);
// 		i++;
// 	}
// 	return (NULL);
// }
*/

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
	{"fov", FIELD_FLOAT, 0, 180}
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

static t_field_rule	*get_sphere_fields(void)
{
	static t_field_rule	fields[] = {
	{"center", FIELD_VEC3, 0, 0},
	{"radius", FIELD_FLOAT, 0.0001, 1e6},
	{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

static	t_field_rule	*get_plane_fields(void)
{
	static t_field_rule	fields[] = {
	{"point", FIELD_VEC3, 0, 0},
	{"normal", FIELD_VEC3, -1, 1},
	{"color", FIELD_COLOR, 0, 255}
	};

	return (fields);
}

static t_field_rule	*get_cylinder_fields(void)
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

//added bea
static t_field_rule	*get_cone_fields(void)
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

t_object_rule	*initialize_object_rule(t_object_rule *rule, const char *keyword,
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

	if (strcmp(keyword, "A") == 0)
		return (initialize_object_rule(&rule, "A", get_ambient_fields(), 2));
	else if (strcmp(keyword, "C") == 0)
		return (initialize_object_rule(&rule, "C", get_camera_fields(), 3));
	else if (strcmp(keyword, "L") == 0)
		return (initialize_object_rule(&rule, "L", get_light_fields(), 3));
	else if (strcmp(keyword, "sp") == 0)
		return (initialize_object_rule(&rule, "sp", get_sphere_fields(), 3));
	else if (strcmp(keyword, "pl") == 0)
		return (initialize_object_rule(&rule, "pl", get_plane_fields(), 3));
	else if (strcmp(keyword, "cy") == 0)
		return (initialize_object_rule(&rule, "cy", get_cylinder_fields(), 5));
	//added bea
	else if (strcmp(keyword, "co") == 0)
		return (initialize_object_rule(&rule, "co", get_cone_fields(), 5));
	return (NULL);
}
