/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:50:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/12 20:22:25 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	parse_color_component(const char **s, int *out, int i)
{
	int	value;
	int	read;

	skip_spaces(s);
	read = 0;
	value = ft_atoi_count(*s, &read);
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

int	parse_color(t_color *out, const char **s, t_field_rule *field,
	t_metadata *meta)
{
	int	color[3];

	if (!parse_color_component(s, &color[0], 0)
		|| !parse_color_component(s, &color[1], 1)
		|| !parse_color_component(s, &color[2], 2))
	{
		return (report_error(meta, "expected color 'r,g,b' for", field->name));
	}
	if (color[0] < (int)field->min || color[0] > (int)field->max)
		return (report_range_error(meta, field, color[0]));
	if (color[1] < (int)field->min || color[1] > (int)field->max)
		return (report_range_error(meta, field, color[1]));
	if (color[2] < (int)field->min || color[2] > (int)field->max)
		return (report_range_error(meta, field, color[2]));
	out->r = color[0];
	out->g = color[1];
	out->b = color[2];
	return (1);
}
