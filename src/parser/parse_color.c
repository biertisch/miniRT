/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:50:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 21:55:47 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

int	parse_color(t_color *out, const char **s, t_metadata *meta)
{
	t_color tmp;
	if (!parse_color_component(s, &tmp.r, 0)
		|| !parse_color_component(s, &tmp.g, 1)
		|| !parse_color_component(s, &tmp.b, 2))
	{
		return (report_error(meta, "expected color 'r,g,b' for". meta->rule->name));
	}
	if (tmp.r < (int)meta->rule->min || tmp.r > (int)meta->rule->max)
		return (report_range_error(meta, tmp.r));
	if (tmp.g < (int)meta->rule->min || tmp.g > (int)meta->rule->max)
		return (report_range_error(meta, tmp.g));
	if (tmp.b < (int)meta->rule->min || tmp.b > (int)meta->rule->max)
		return (report_range_error(meta, tmp.b));
	*out = tmp;
	return (1);
}
