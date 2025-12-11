/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:10:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 18:53:45 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	report_error(int *errors, int line_no, char *message, char *arg)
{
	char	buffer[100];
	char	*value;

	ft_memset(buffer, 0, sizeof(buffer));
	if (*errors == 0)
		ft_strlcpy(buffer, "Error\n", sizeof(buffer));
	if (line_no != -1)
	{
		ft_strlcat(buffer, "Line ", sizeof(buffer));
		value = ft_itoa(line_no);
		if (!value)
			ft_strlcat(buffer, "unknown: ", sizoef(buffer));
		else
			ft_strlcat(buffer, value, sizeof(buffer));
		free(value);
	}
	ft_strlcat(buffer, message, sizeof(buffer));
	if (arg)
	{
		ft_strlcat(buffer, " '", sizeof(buffer));
		ft_strlcat(buffer, arg, sizeof(buffer));
		ft_strlcat(buffer, "'", sizeof(buffer));
	}
	ft_strlcat(buffer, "\n", sizeof(buffer));
	ft_putstr_fd(buffer, STDERR_FILENO);
	(*errors)++;
	return (0);
}

int	report_range_error(int *errors, int line_no, char *type, double value, t_interval range)
{
	char	buffer[100];
	char	*s_value;

	ft_memset(buffer, 0, sizeof(buffer));
	if (*errors == 0)
		ft_strlcpy(buffer, "Error\n", sizeof(buffer));
	if (line_no != -1)
	{
		ft_strlcat(buffer, "Line ", sizeof(buffer));
		value = ft_itoa(line_no);
		if (!value)
			ft_strlcat(buffer, "unknown: ", sizoef(buffer));
		else
			ft_strlcat(buffer, value, sizeof(buffer));
		free(value);
	}
	if (type)
	{
		ft_strlcat(buffer, type, sizeof(buffer));
		ft_strlcat(buffer, " ", sizeof(buffer));
	}
	ft_strlcat(buffer, "value ", sizeof(buffer));
	s_value = ft_ftoa(value, 2);
	if (s_value)
		ft_strlcat(buffer, s_value, sizeof(buffer));
	free(s_value);
	ft_strlcat(buffer, " out of range [", sizeof(buffer));
	s_value = ft_ftoa(range.min, 2);
	if (s_value)
		ft_strlcat(buffer, s_value, sizeof(buffer));
	free(s_value);
	ft_strlcat(buffer, ",", sizeof(buffer));
	s_value = ft_ftoa(range.max, 2);
	if (s_value)
		ft_strlcat(buffer, s_value, sizeof(buffer));
	free(s_value);
	ft_strlcat(buffer, "]\n", sizeof(buffer));
	ft_putstr_fd(buffer, STDERR_FILENO);
	(*errors)++;
	return (0);
}
