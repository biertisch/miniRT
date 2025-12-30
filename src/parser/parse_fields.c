/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fields.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:05:44 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/30 00:51:55 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	next_char_valid(const char *s)
{
	return (*s == '\0' || ft_isspace((unsigned char)*s) || *s == '#');
}

void	skip_spaces(const char **s)
{
	while (**s && ft_isspace((unsigned char)**s))
		(*s)++;
}

static int	check_trailing(const char *s)
{
	skip_spaces(&s);
	return (*s == '\0' || *s == '\n' || *s == '#');
}

static int	parse_fields(t_world *scene, const char **s, t_metadata *meta)
{
	if (ft_strcmp(meta->rule->keyword, "A") == 0)
		return (parse_ambient_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "C") == 0)
		return (parse_camera_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "L") == 0)
		return (parse_light_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "sp") == 0)
		return (parse_sphere_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "pl") == 0)
		return (parse_plane_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "cy") == 0)
		return (parse_cylinder_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "co") == 0)
		return (parse_cone_fields(scene, s, meta));
	if (ft_strcmp(meta->rule->keyword, "tr") == 0)
		return (parse_triangle_fields(scene, s, meta));
	return (0);
}

int	parse_line(t_world *scene, const char *line, t_metadata *meta)
{
	char	key[4];

	skip_spaces(&line);
	if (*line == '\0' || *line == '\n' || *line == '#')
		return (0);
	if (!is_valid_keyword(line, key, meta))
		return (report_error(ERR_KEYWORD, meta, NULL, -1));
	meta->rule = find_rule(key);
	if (!meta->rule)
		return (report_error(ERR_KEYWORD, meta, NULL, -1));
	line += ft_strlen(key);
	if (!parse_fields(scene, &line, meta))
		return (0);
	if (!check_trailing(line))
		return (report_error(ERR_TRAILING, meta, NULL, -1));
	return (1);
}
