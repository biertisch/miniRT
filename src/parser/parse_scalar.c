/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scalar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:17:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 17:08:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_int(int *out, const char **s, t_field_rule *field, t_metadata *meta)
{
	int	tmp;
	int	read;

	skip_spaces(s);
	read = 0;
	tmp = ft_atoi_count(*s, &read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace((unsigned char)**s)))
		return (report_error(ERR_FORMAT, meta, field, -1));
	if ((field->min != 0 || field->max != 0)
		&& (tmp < (int)field->min || tmp > (int)field->max))
		return (report_error(ERR_RANGE, meta, field, tmp));
	*out = tmp;
	return (1);
}

int	parse_float(double *out, const char **s, t_field_rule *field,
	t_metadata *meta)
{
	double	tmp;
	int		read;

	skip_spaces(s);
	read = 0;
	tmp = ft_atof_count(*s, &read);
	(*s) += read;
	if (read == 0 || (**s != '\0' && !ft_isspace((unsigned char)**s)))
		return (report_error(ERR_FORMAT, meta, field, -1));
	if ((field->min != 0 || field->max != 0)
		&& (tmp < field->min || tmp > field->max))
		return (report_error(ERR_RANGE, meta, field, tmp));
	*out = tmp;
	return (1);
}
