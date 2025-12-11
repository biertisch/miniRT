/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_field.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:17:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 18:53:24 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_int(const	t_field_rule *rule, int *out, const char **s, int line_no, int *errors)
{
	int	value;
	int	read;

	skip_spaces(s);
	read = 0;
	value = ft_atoi_count(*s, &read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace(**s) && **s != ','))
		return (report_error(errors, line_no, "expected int for ", rule->name));
	if ((rule->min != 0 || rule->max != 0) && (value < (int)rule->min || value > (int)rule->max))
		return (report_range_error(errors, line_no, rule->name, value, (t_interval){rule->min, rule->max}));
	*out = value;
	return (1);
}

int	parse_float(const t_field_rule *rule, double *out, const char **s, int line_no, int *errors)
{
	double	value;
	int		read;

	skip_spaces(s);
	read = 0;
	value = ft_atof_count(*s, *read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace(**s) && **s != ','))
		return (report_error(errors, line_no, "expected float for", rule->name));
	if ((rule->min != 0 || rule->max != 0) && (value < rule->min || value > rule->max))
		return (report_range_error(errors, line_no, rule->name, value, (t_interval){rule->min, rule->max}));
	*out = value;
	return (1);
}

static int	parse_color_component(const char **s, int *out, int i)
{
	int	value;
	int	read;

	skip_spaces(s);
	read = 0;
	value = ft_atoi_count(*s, *read);
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

int	parse_color(const t_field_rule *rule, t_color *out, const char **s, int line_no, int *errors)
{
	if (!parse_color_component(s, &out->r, 0)
		|| !parse_color_component(s, &out->g, 1)
		|| !parse_color_component(s, &out->b, 2))
	{
		return (report_error(errors, line_no, "expected color 'r,g,b' for". rule->name));
	}
	if (out->r < (int)rule->min || out->r > (int)rule->max)
		return (report_range_error(errors, line_no, rule->name, out->x, (t_interval){rule->min, rule->max}));
	if (out->g < (int)rule->min || out->g > (int)rule->max)
		return (report_range_error(errors, line_no, rule->name, out->g, (t_interval){rule->min, rule->max}));
	if (out->b < (int)rule->min || out->b > (int)rule->max)
		return (report_range_error(errors, line_no, rule->name, out->b, (t_interval){rule->min, rule->max}));
	return (1);
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

int	parse_vec3(const t_field_rule *rule, t_vec3 *out, const char **s, int line_no, int *errors)
{
	if (!parse_vec3_component(s, &out->x, 0)
		|| !parse_vec3_component(s, &out->y, 1)
		|| !parse_vec3_component(s, &out->z, 2))
	{
		return (report_error(errors, line_no, "expected vec3 'x,y,z' for". rule->name));
	}
	if (out->x < rule->min || out->x > rule->max)
		return (report_range_error(errors, line_no, rule->name, out->x, (t_interval){rule->min, rule->max}));
	if (out->y < rule->min || out->y > rule->max)
		return (report_range_error(errors, line_no, rule->name, out->y, (t_interval){rule->min, rule->max}));
	if (out->z < rule->min || out->z > rule->max)
		return (report_range_error(errors, line_no, rule->name, out->z, (t_interval){rule->min, rule->max}));
	return (1);
}
