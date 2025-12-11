/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:05:44 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 18:56:49 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	parse_ambient_fields(t_world *scene, char **s, t_object_rule *rule, int line_no, int *errors)
{
	if (parse_float(rule, &scene->ambient_ratio, s, line_no, errors))
		return (0);
	if (parse_color(rule, &scene->ambient, s, line_no, errors))
		return (0);
	return (1);
}

static int	parse_camera_fields(t_world *scene, char **s, t_object *rule, int line_no, int *errors)
{
	int	fov;

	if (parse_vec3(rule, &scene->camera.lookfrom, s, line_no, errors))
		return (0);
	if (parse_vec3(rule, &scene->camera.vup, s, line_no, errors))
		return (0);
	if (parse_int(rule, &fov, s, line_no, errors))
		return (0);
	scene->camera.vfov = (double)fov;
	return (1);
}

static int	parse_light_fields(t_world *scene, char **s, t_object *rule, int line_no, int *errors)
{
	if (parse_vec3(rule, &scene->spot_light.position, s, line_no, errors))
		return (0);
	if (parse_float(rule, &scene->spot_light.brightness, s, line_no, errors))
		return (0);
	if (parse_color(rule, &scene->spot_light.light_color, s, line_no, errors))
		return (0);
	return (1);
}

static int	parse_fields(t_world *scene, char **s, t_object_rule *rule, int line_no, int *errors)
{
	if (ft_strcmp(rule->keyword, "A") == 0)
		return (parse_ambient_fields(scene, s, rule, line_no, errors));
	if (ft_strcmp(rule->keyword, "C") == 0)
		return (parse_camera_fields(scene, s, rule, line_no, errors));
	if (ft_strcmp(rule->keyword, "L") == 0)
		return (parse_light_fields(scene, s, rule, line_no, errors));
}

int	parse_line(t_world *scene, char *line, int line_no, t_color *acl, int *errors)
{
	t_object_rule	*rule;
	char			key[4];

	skip_spaces(&line);
	if (*line == '\0' || *line == '\n' || *line == '#')
		return (0);
	if (!is_valid_keyword(line, key, acl))
		return (report_error(errors, line_no, "unknown or malformed keyword", NULL));
	rule = find_rule(key);
	if (!rule)
		return (report_error(errors, line_no, "unknown keyword", key));
	line += ft_strlen(key);
	if (!parse_fields(scene, &line, rule, line_no, errors))
		return (0);
	if (!check_trailing(line))
		return (report_error(errors, line_no, "extra tokens or malformed input after fields", NULL));
    return (1);
}
