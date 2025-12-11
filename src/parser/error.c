/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:10:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:28:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	add_range(char *buffer, t_field_rule *rule, int precision, int cap)
{
	char	*tmp;

	ft_strlcat(buffer, " out of range [", cap);
	tmp = ft_ftoa(rule->rule->min, precision);
	if (tmp)
		ft_strlcat(buffer, tmp, cap);
	free(tmp);
	ft_strlcat(buffer, ",", cap);
	tmp = ft_ftoa(rule->max, precision);
	if (tmp)
		ft_strlcat(buffer, tmp, cap);
	free(tmp);
	ft_strlcat(buffer, "]\n", cap);
}

static void	add_line(char *buffer, int line_no, int cap)
{
	char	*tmp;

	if (line_no == -1)
		return ;
	ft_strlcat(buffer, "Line ", cap);
	tmp = ft_itoa(line_no);
	if (!tmp)
		ft_strlcat(buffer, "unknown: ", cap);
	else
	{
		ft_strlcat(buffer, tmp, cap);
		ft_strlcat(buffer, ": ", cap);
	}
	free(tmp);
}

int	report_error(t_metadata *meta, char *message, char *arg)
{
	char	buffer[100];

	ft_memset(buffer, 0, sizeof(buffer));
	if (meta->errors == 0)
		ft_strlcpy(buffer, "Error\n", sizeof(buffer));
	add_line(buffer, meta->line_no, sizeof(buffer));
	ft_strlcat(buffer, message, sizeof(buffer));
	if (arg)
	{
		ft_strlcat(buffer, " '", sizeof(buffer));
		ft_strlcat(buffer, arg, sizeof(buffer));
		ft_strlcat(buffer, "'", sizeof(buffer));
	}
	ft_strlcat(buffer, "\n", sizeof(buffer));
	ft_putstr_fd(buffer, STDERR_FILENO);
	meta->errors++;
	return (0);
}

int	report_range_error(t_metadata *meta, double value, int precision)
{
	char	buffer[100];
	char	*tmp;

	ft_memset(buffer, 0, sizeof(buffer));
	if (meta->errors == 0)
		ft_strlcpy(buffer, "Error\n", sizeof(buffer));
	add_line(buffer, meta->line_no, sizeof(buffer));
	ft_strlcat(buffer, meta->rule->keyword, sizeof(buffer));
	ft_strlcat(buffer, " value ", sizeof(buffer));
	tmp = ft_ftoa(value, precision);
	if (tmp)
		ft_strlcat(buffer, tmp, sizeof(buffer));
	free(tmp);
	add_range(buffer, meta->rule, precision, sizeof(buffer));
	ft_putstr_fd(buffer, STDERR_FILENO);
	meta->errors++;
	return (0);
}

int	report_global_error(t_metadata *meta, char *message, int count)
{
	char	buffer[100];
	char	*tmp;

	ft_memset(buffer, 0, sizeof(buffer));
	if (meta->errors == 0)
		ft_strlcpy(buffer, "Error\n", sizeof(buffer));
	add_line(buffer, meta->line_no, sizeof(buffer));
	ft_strlcat(buffer, message, sizeof(buffer));
	tmp = ft_itoa(count);
	if (tmp)
		ft_strlcat(buffer, tmp, sizeof(buffer));
	free(tmp);
	ft_strlcat(buffer, ")\n", sizeof(buffer));
	ft_putstr_fd(buffer, STDERR_FILENO);
	meta->errors++;
	return (0);
}
