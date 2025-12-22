/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:47:33 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/22 10:53:03 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	append_int(char *buffer, int value)
{
	char	*tmp;

	tmp = ft_itoa(value);
	if (tmp)
		ft_strlcat(buffer, tmp, ERR_BUFFER_SIZE);
	free(tmp);
}

void	append_float(char *buffer, double value, int precision)
{
	char	*tmp;

	tmp = ft_ftoa(value, precision);
	if (tmp)
		ft_strlcat(buffer, tmp, ERR_BUFFER_SIZE);
	free(tmp);
}

void	append_line(char *buffer, int line_no)
{
	if (line_no == -1)
		return ;
	ft_strlcat(buffer, "Line ", ERR_BUFFER_SIZE);
	append_int(buffer, line_no);
	ft_strlcat(buffer, ": ", ERR_BUFFER_SIZE);
}

void	append_header(char *buffer, int errors)
{
	if (errors == 0)
		ft_strlcpy(buffer, "Error\n", ERR_BUFFER_SIZE);
}
