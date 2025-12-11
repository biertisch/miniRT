/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scalar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:17:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:13:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_int(int *out, const char **s, t_metadata *meta)
{
	int	tmp;
	int	read;

	skip_spaces(s);
	read = 0;
	tmp = ft_atoi_count(*s, &read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace(**s) && **s != ','))
		return (report_error(meta, "expected int for ", meta->rule->name));
	if ((meta->rule->min != 0 || meta->rule->max != 0)
		&& (tmp < (int)meta->rule->min || tmp > (int)meta->rule->max))
		return (report_range_error(meta, tmp, 0));
	*out = tmp;
	return (1);
}

int	parse_float(double *out, const char **s, t_metadata *meta, int precision)
{
	double	tmp;
	int		read;

	skip_spaces(s);
	read = 0;
	tmp = ft_atof_count(*s, *read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace(**s) && **s != ','))
		return (report_error(meta, "expected float for", meta->rule->name));
	if ((meta->rule->min != 0 || meta->rule->max != 0)
		&& (tmp < meta->rule->min || tmp > meta->rule->max))
		return (report_range_error(meta, tmp, precision));
	*out = tmp;
	return (1);
}
