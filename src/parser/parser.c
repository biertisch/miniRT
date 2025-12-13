/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:39:46 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 17:09:52 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_global_rules(t_metadata *meta)
{
	if (meta->ambient_count != 1)
		report_error(ERR_AMBIENT_COUNT, meta, NULL, -1);
	if (meta->camera_count != 1)
		report_error(ERR_CAMERA_COUNT, meta, NULL, -1);
	if (meta->light_count != 1)
		report_error(ERR_LIGHT_COUNT, meta, NULL, -1);
	if (meta->errors != 0)
		return (report_error(ERR_VALIDATE, meta, NULL, -1));
	return (1);
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
		ft_putstr_fd("Error\nConfiguration file must have a .rt extension\n",
			STDERR_FILENO);
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
	if (!validate_config_file(scene, fd))
		return (0);
	return (1);
}
