/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:55:45 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 15:36:14 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// unnecessary for parser
t_plane	new_plane(t_vec3 point, t_vec3 normal, t_material mat)
{
	t_plane	plane;

	plane.point = point;
	plane.normal = vec3_norm(normal);
	plane.mat = mat;
	return (plane);
}

static void	get_plane_uv(t_vec3 p2hit, t_tbn tbn, t_hit_record *record)
{
	record->u = vec3_dot(p2hit, tbn.ct);
	record->v = vec3_dot(p2hit, tbn.cb);
	record->u -= floor(record->u);
	record->v -= floor(record->v);
}

int	plane_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_plane		*plane;
	double		denom;
	double		t;

	plane = &obj.geo.plane;
	denom = vec3_dot(ray->direction, plane->normal);
	if (fabs(denom) < 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(plane->point, ray->origin), plane->normal) / denom;
	if (!interval_surrounds(&ray_t, t))
		return (0);
	record->t = t;
	record->p = ray_at(ray, t);
	record->is_d_side = 1;
	set_face_normal(ray, plane->normal, record);
	record->g_norm = record->normal;
	get_plane_uv(vec3_sub(record->p, plane->point), get_tbn_plane(plane->normal), record);
	if (obj.tex_type == BUMP_FUNC)
		record->g_norm = apply_bump(get_tbn_plane(record->g_norm), record->u,
				record->v, sine_bump);
	else if (obj.tex_type == PICTURE)
		record->g_norm = apply_bump_map(get_tbn_plane(record->g_norm),
				bump_tangent_normal(
					&((t_pic_tex *)plane->mat.data.lamb.tex)->bump_tex,
					record->u, record->v, 0.1));
	record->mat = plane->mat;
	return (1);
}
