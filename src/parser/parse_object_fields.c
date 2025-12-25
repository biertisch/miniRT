/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object_fields.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:36:25 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/25 14:03:55 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_material	parse_material(const char **s, t_color color)
{
	t_checker_tex	*checker;

	skip_spaces(s);
	if (**s == 'c' && (*(*s + 1) == '\0' || ft_isspace(*(*s + 1))
			|| *(*s + 1) == '#'))
	{
		(*s)++;
		world()->tex_type = CHECKER;
		checker = checker_texture_colors(20, norm_color(color),
				norm_color((t_color){0, 125, 125}));
		return (get_material_texture(LAMBERTIAN, (t_texture *)checker, 0));
	}
	if (**s == 'p' && (*(*s + 1) == '\0' || ft_isspace(*(*s + 1))
			|| *(*s + 1) == '#'))
	{
		(*s)++;
		world()->tex_type = PICTURE;
		return (get_material_texture(LAMBERTIAN, (t_texture *)&(world()->pic_c_tex), 0));
	}
	if (**s == 'b' && (*(*s + 1) == '\0' || ft_isspace(*(*s + 1))
			|| *(*s + 1) == '#'))
	{
		(*s)++;
		world()->tex_type = BUMP_FUNC;
	}
	else
		world()->tex_type = SOLID_COLOR;
	return (get_material(LAMBERTIAN, color, 0));
}

int	parse_sphere_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_sphere	sphere;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&sphere.center, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[1], meta))
		return (0);
	sphere.radius = diameter / 2;
	if (!parse_color(&color, s, &meta->rule->fields[2], meta))
		return (0);
	sphere.mat = parse_material(s, color);
	add_object_to_world(scene, SPHERE, &sphere);
	return (1);
}

int	parse_plane_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_plane		plane;
	t_color		color;

	if (!parse_vec3(&plane.point, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&plane.normal, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[2], meta))
		return (0);
	plane.mat = parse_material(s, color);
	add_object_to_world(scene, PLANE, &plane);
	return (1);
}

int	parse_cylinder_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_cylinder	cylinder;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&cylinder.center, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&cylinder.axis, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[2], meta))
		return (0);
	cylinder.radius = diameter / 2;
	if (!parse_float(&cylinder.height, s, &meta->rule->fields[3], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[4], meta))
		return (0);
	cylinder.mat = parse_material(s, color);
	add_object_to_world(scene, CYLINDER, &cylinder);
	return (1);
}

int	parse_cone_fields(t_world *scene, const char **s, t_metadata *meta)
{
	t_cone		cone;
	t_color		color;
	double		diameter;

	if (!parse_vec3(&cone.apex, s, &meta->rule->fields[0], meta))
		return (0);
	if (!parse_vec3(&cone.axis, s, &meta->rule->fields[1], meta))
		return (0);
	if (!parse_float(&diameter, s, &meta->rule->fields[2], meta))
		return (0);
	cone.radius = diameter / 2;
	if (!parse_float(&cone.height, s, &meta->rule->fields[3], meta))
		return (0);
	if (!parse_color(&color, s, &meta->rule->fields[4], meta))
		return (0);
	cone.mat = parse_material(s, color);
	add_object_to_world(scene, CONE, &cone);
	return (1);
}
