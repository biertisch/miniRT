/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:39:46 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 17:22:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	validate_config_file(t_world *scene, int fd)
{
	char	*line;
	int		line_no;
	int		errors;
	t_color	acl;

	acl = (t_color){0, 0, 0};
	errors = 0;
	line_no = 0;
	line = NULL;
	while (1)
	{
		free(line);
		line = get_next_line(fd);
		if (!line)
			break ;
		line_no++;
	    parse_line(scene, line, line_no, &acl, &errors);
	}
	free(line);
	check_global_rules(acl, &errors);
	close(fd);
	return (errors == 0);
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
