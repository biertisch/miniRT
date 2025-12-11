/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:39:46 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:35:09 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_global_rules(t_metadata *meta)
{
	if (meta->ambient_count != 1)
		report_global_error(meta,
			"Global check: ambient light 'A' must appear exactly once (found ",
			meta->ambient_count);
	if (meta->camera_count != 1)
		report_global_error(meta,
			"Global check: camera 'C' must appear exactly once (found ",
			meta->camera_count);
	if (meta->light_count != 1)
		report_global_error(meta,
			"Global check: light 'L' must appear exactly once (found ",
			meta->light_count);
	if (meta->errors != 0)
	{
		ft_putstr_fd("Validation failed: ", STDERR_FILENO);
		ft_putnbr_fd(meta->errors, STDERR_FILENO);
		ft_putstr_fd(" error(s) found in configuration file.\n", STDERR_FILENO);
	}
}

static int	validate_config_file(t_world *scene, int fd)
{
	t_metadata	meta;
	char		*line;

	ft_memset(&meta, 0, sizeof(meta));
	line = NULL;
	while (1)
	{
		free(line);
		line = get_next_line(fd);
		if (!line)
			break ;
		meta.line_no++;
	    parse_line(scene, line, &meta);
	}
	free(line);
	validate_global_rules(&meta);
	close(fd);
	return (meta.errors == 0);
}

static int	validate_extension(char *file)
{
	char	*extension;

	extension = ft_strrchr(file, '.');
	if (!extension || ft_strcmp(extension, ".rt"))
	{
		ft_putstr_fd("Error\nConfiguration file must have a .rt extension\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	parser(t_world *scene, char *file)
{
	int	fd;

	if (!file || !validate_extension(file))
		return (0);
    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("Error\nfopen");
        return (0);
    }
	if (!validate_config_file(scene, file))
		return (0);
	return (1);
}
