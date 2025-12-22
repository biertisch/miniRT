/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:10:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/22 10:53:48 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	report_error(t_error_type code, t_metadata *meta, t_field_rule *field,
	double value)
{
	char	buffer[ERR_BUFFER_SIZE];

	ft_memset(buffer, 0, ERR_BUFFER_SIZE);
	append_header(buffer, meta->errors);
	if (code == ERR_VALIDATE)
		global_error_message(buffer, meta->errors);
	else if (code == ERR_AMBIENT_COUNT || code == ERR_CAMERA_COUNT
		|| code == ERR_LIGHT_COUNT)
		count_error_message(buffer, code, meta);
	else
		append_line(buffer, meta->line_no);
	if (code == ERR_KEYWORD)
		ft_strlcat(buffer, "unknown keyword", ERR_BUFFER_SIZE);
	else if (code == ERR_FORMAT)
		format_error_message(buffer, field);
	else if (code == ERR_RANGE)
		range_error_message(buffer, field, value);
	else if (code == ERR_TRAILING)
		ft_strlcat(buffer, "trailing data after fields", ERR_BUFFER_SIZE);
	else if (code == ERR_NORMAL)
		normal_error_message(buffer, field);
	ft_strlcat(buffer, "\n", ERR_BUFFER_SIZE);
	ft_putstr_fd(buffer, STDERR_FILENO);
	meta->errors++;
	return (0);
}

void	error_exit(t_world *world, char *message)
{
	perror(message);
	free_all_the_world(world);
	exit(EXIT_FAILURE);
}
