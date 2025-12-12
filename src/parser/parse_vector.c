/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:56:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/12 20:28:32 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_normal(t_vec3 *normal, t_field_rule *rule)
{
	if ((ft_strcmp(rule->name, "orientation") == 0
			|| ft_strcmp(rule->name, "normal") == 0
			|| ft_strcmp(rule->name, "axis") == 0)
		&& vec3_length(*normal) != 1)
		return (0);
	return (1);
}

static int	parse_vec3_component(const char **s, double *out, int i)
{
	double	value;
	int		read;

	skip_spaces(s);
	read = 0;
	value = ft_atof_count(*s, &read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace(**s) && **s != ','))
		return (0);
	skip_spaces(s);
	if (i < 2)
	{
		if (**s != ',')
			return (0);
		(*s)++;
	}
	*out = value;
	return (1);
}

int	parse_vec3(t_vec3 *out, const char **s, t_field_rule *field,
	t_metadata *meta)
{
	t_vec3	tmp;

	if (!parse_vec3_component(s, &tmp.x, 0)
		|| !parse_vec3_component(s, &tmp.y, 1)
		|| !parse_vec3_component(s, &tmp.z, 2))
	{
		return (report_error(meta, "expected vec3 'x,y,z' for", field->name));
	}
	if ((field->min != 0 || field->max != 0)
		&& (tmp.x < field->min || tmp.x > field->max))
		return (report_range_error(meta, field, tmp.x));
	if ((field->min != 0 || field->max != 0)
		&& (tmp.y < field->min || tmp.y > field->max))
		return (report_range_error(meta, field, tmp.y));
	if ((field->min != 0 || field->max != 0)
		&& (tmp.z < field->min || tmp.z > field->max))
		return (report_range_error(meta, field, tmp.z));
	if (!validate_normal(&tmp, field))
		return (report_error(meta, "invalid values for", field->name)); //more descriptive message
	*out = tmp;
	return (1);
}
