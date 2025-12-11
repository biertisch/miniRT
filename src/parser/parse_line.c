/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:05:44 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 22:52:28 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	parse_ambient_fields(t_world *scene, char **s, t_metadata *meta)
{
	if (!parse_float(&scene->ambient_ratio, s, meta, 2))
		return (0);
	if (!parse_color(&scene->ambient, s, meta))
		return (0);
	return (1);
}

static int	parse_camera_fields(t_world *scene, char **s, t_metadata *meta)
{
	int	fov;

	if (!parse_vec3(&scene->camera.lookfrom, s, meta))
		return (0);
	if (!parse_vec3(&scene->camera.vup, s, meta))
		return (0);
	if (!parse_int(&fov, s, meta))
		return (0);
	scene->camera.vfov = (double)fov;
	return (1);
}

static int	parse_light_fields(t_world *scene, char **s, t_metadata *meta)
{
	if (!parse_vec3(&scene->spot_light.position, s, meta))
		return (0);
	if (!parse_float(&scene->spot_light.brightness, s, meta, 2))
		return (0);
	if (!parse_color(&scene->spot_light.light_color, s, meta))
		return (0);
	return (1);
}

static int	parse_fields(t_world *scene, char **s, t_metadata *meta)
{
	if (ft_strcmp(rule->keyword, "A") == 0)
		return (parse_ambient_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "C") == 0)
		return (parse_camera_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "L") == 0)
		return (parse_light_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "sp") == 0)
		return (parse_sphere_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "pl") == 0)
		return (parse_plane_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "cy") == 0)
		return (parse_cylinder_fields(scene, s, meta));
	if (ft_strcmp(rule->keyword, "co") == 0)
		return (parse_cone_fields(scene, s, meta));
	return (0);
}

int	parse_line(t_world *scene, char *line, t_metadata *meta)
{
	char	key[4];

	skip_spaces(&line);
	if (*line == '\0' || *line == '\n' || *line == '#')
		return (0);
	if (!is_valid_keyword(line, key, meta))
		return (report_error(meta, "unknown or malformed keyword", NULL));
	meta->rule = find_rule(key);
	if (!meta->rule)
		return (report_error(meta, "unknown keyword", key));
	line += ft_strlen(key);
	if (!parse_fields(scene, &line, meta))
		return (0);
	if (!check_trailing(line))
		return (report_error(meta, "extra tokens or malformed input after fields", NULL));
    return (1);
}
