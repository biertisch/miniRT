/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:56:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/29 16:38:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_normal(t_vec3 *normal, t_field_rule *rule)
{
	if ((ft_strcmp(rule->name, "orientation") == 0
			|| ft_strcmp(rule->name, "normal") == 0
			|| ft_strcmp(rule->name, "axis") == 0)
		&& fabs(vec3_length(*normal) - 1) > 1e-3)
		return (0);
	return (1);
}

static int	parse_vec3_component(const char **s, double *out, int i)
{
	double	value;
	int		read;

	read = 0;
	value = ft_atof_count(*s, &read);
	(*s) += read;
	if (read == 0)
		return (0);
	if (i < 2)
	{
		if (**s != ',')
			return (0);
		(*s)++;
	}
	if (i == 2 && **s != '\0' && !ft_isspace((unsigned char)**s) && **s != '#')
		return (0);
	*out = value;
	return (1);
}

int	parse_vec3(t_vec3 *out, const char **s, t_field_rule *field,
	t_metadata *meta)
{
	t_vec3	tmp;

	skip_spaces(s);
	if (!parse_vec3_component(s, &tmp.x, 0)
		|| !parse_vec3_component(s, &tmp.y, 1)
		|| !parse_vec3_component(s, &tmp.z, 2))
	{
		return (report_error(ERR_FORMAT, meta, field, -1));
	}
	if ((field->min != 0 || field->max != 0)
		&& (tmp.x < field->min || tmp.x > field->max))
		return (report_error(ERR_RANGE, meta, field, tmp.x));
	if ((field->min != 0 || field->max != 0)
		&& (tmp.y < field->min || tmp.y > field->max))
		return (report_error(ERR_RANGE, meta, field, tmp.y));
	if ((field->min != 0 || field->max != 0)
		&& (tmp.z < field->min || tmp.z > field->max))
		return (report_error(ERR_RANGE, meta, field, tmp.z));
	if (!validate_normal(&tmp, field))
	{
		printf("Current length: %f\n", vec3_length(tmp));
		return (report_error(ERR_NORMAL, meta, field, -1));
	}
	*out = tmp;
	return (1);
}
