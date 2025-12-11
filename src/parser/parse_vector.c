/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:56:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:07:08 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_normal(t_vec3 *normal, t_field_rule *rule)
{
	return (!(ft_strcmp(rule->name, "orientation")
		|| ft_strcmp(rule->name, "normal")
		|| ft_strcmp(rule->name, "axis")
		&& (normal.x == 0 && normal.y == 0 && normal.z == 0)));
}

static int	parse_vec3_component(const char **s, double *out, int i)
{
	double	value;
	int		read;

	skip_spaces(s);
	read = 0;
	value = ft_atof_count(*s, *read);
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
	return (1);
}

int	parse_vec3(t_vec3 *out, const char **s, t_metadata *meta)
{
	t_vec3	tmp;

	if (!parse_vec3_component(s, &tmp.x, 0)
		|| !parse_vec3_component(s, &tmp.y, 1)
		|| !parse_vec3_component(s, &tmp.z, 2))
	{
		return (report_error(meta, "expected vec3 'x,y,z' for", meta->rule->name));
	}
	if ((meta->rule->min != 0 || meta->rule->max != 0)
		&& (tmp.x < meta->rule->min || tmp.x > meta->rule->max))
		return (report_range_error(meta, tmp.x));
	if ((meta->rule->min != 0 || meta->rule->max != 0)
		&& (tmp.y < meta->rule->min || tmp.y > meta->rule->max))
		return (report_range_error(meta, tmp.y));
	if ((meta->rule->min != 0 || meta->rule->max != 0)
		&& (tmp.z < meta->rule->min || tmp.z > meta->rule->max))
		return (report_range_error(meta, tmp.z));
	if (!validate_normal(&tmp, meta->rule))
		return (report_error(meta, "invalid values for", meta->rule->name));
	*out = tmp;
	return (1);
}
