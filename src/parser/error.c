/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:10:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 17:15:48 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	range_error_message(char *buffer, t_field_rule *field, double value)
{
	if (!field)
		return ;
	ft_strlcat(buffer, "'", ERR_BUFFER_SIZE);
	if (field->name)
		ft_strlcat(buffer, field->name, ERR_BUFFER_SIZE);
	ft_strlcat(buffer, "' value ", ERR_BUFFER_SIZE);
	append_float(buffer, value, field->precision);
	ft_strlcat(buffer, " out of range [", ERR_BUFFER_SIZE);
	append_float(buffer, field->min, field->precision);
	ft_strlcat(buffer, ",", ERR_BUFFER_SIZE);
	append_float(buffer, field->max, field->precision);
	ft_strlcat(buffer, "]", ERR_BUFFER_SIZE);
}

static void	format_error_message(char *buffer, t_field_rule *field)
{
	if (!field)
		return ;
	ft_strlcat(buffer, "expected ", ERR_BUFFER_SIZE);
	if (field->type == FIELD_INT)
		ft_strlcat(buffer, "int", ERR_BUFFER_SIZE);
	else if (field->type == FIELD_FLOAT)
		ft_strlcat(buffer, "float", ERR_BUFFER_SIZE);
	else if (field->type == FIELD_VEC3)
		ft_strlcat(buffer, "vec3 'x,y,z'", ERR_BUFFER_SIZE);
	else if (field->type == FIELD_COLOR)
		ft_strlcat(buffer, "color 'r,g,b'", ERR_BUFFER_SIZE);
	ft_strlcat(buffer, " for '", ERR_BUFFER_SIZE);
	if (field->name)
		ft_strlcat(buffer, field->name, ERR_BUFFER_SIZE);
	ft_strlcat(buffer, "'", ERR_BUFFER_SIZE);
}

static void	count_error_message(char *buffer, t_error_type code,
	t_metadata *meta)
{
	int	value;

	if (!meta)
		return ;
	ft_strlcat(buffer, "Global check: ", ERR_BUFFER_SIZE);
	if (code == ERR_AMBIENT_COUNT)
	{
		value = meta->ambient_count;
		ft_strlcat(buffer, "ambient light 'L'", ERR_BUFFER_SIZE);
	}
	else if (code == ERR_CAMERA_COUNT)
	{
		value = meta->camera_count;
		ft_strlcat(buffer, "camera 'C'", ERR_BUFFER_SIZE);
	}
	else if (code == ERR_LIGHT_COUNT)
	{
		value = meta->light_count;
		ft_strlcat(buffer, "light 'L'", ERR_BUFFER_SIZE);
	}
	ft_strlcat(buffer, " must appear exactly once (found ", ERR_BUFFER_SIZE);
	append_int(buffer, value);
	ft_strlcat(buffer, ")", ERR_BUFFER_SIZE);
}

static void	global_error_message(char *buffer, int errors)
{
	ft_strlcat(buffer, "Validation failed: ", ERR_BUFFER_SIZE);
	append_int(buffer, errors);
	ft_strlcat(buffer, " error(s) found in configuration file.",
		ERR_BUFFER_SIZE);
}

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
