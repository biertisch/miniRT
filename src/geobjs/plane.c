/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:55:45 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 00:56:26 by bliu             ###   ########.fr       */
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

static void	plane_uv(t_vec3 p2hit, t_tbn tbn, t_hit_record *record)
{
	record->u = vec3_dot(p2hit, tbn.ct);
	record->v = vec3_dot(p2hit, tbn.cb);
	record->u -= floor(record->u);
	record->v -= floor(record->v);
}

void	change_plane_normal_according_bump(t_plane *pl, t_hit_record *rec,
	t_tex_type tex_type)
{
	if (tex_type == BUMP_FUNC)
		rec->g_norm = apply_bump_f(plane_tbn(rec->g_norm), rec->u,
				rec->v, sine_bump);
	else if (tex_type == PICTURE)
		rec->g_norm = apply_bump_map(plane_tbn(rec->g_norm),
				bump_tangent_normal(
					&((t_pic_tex *)pl->mat.data.lamb.tex)->bump_tex,
					rec->u, rec->v, 0.1));
}

int	plane_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *rec)
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
	rec->t = t;
	rec->p = ray_at(ray, t);
	rec->is_d_side = 1;
	// set_face_normal(ray, plane->normal, rec);
	// rec->g_norm = rec->normal;
	rec->g_norm = plane->normal;
	plane_uv(vec3_sub(rec->p, plane->point), plane_tbn(plane->normal), rec);
	change_plane_normal_according_bump(plane, rec, obj.tex_type);
	rec->mat = plane->mat;
	return (1);
}
