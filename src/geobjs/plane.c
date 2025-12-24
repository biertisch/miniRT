/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:55:45 by bliu              #+#    #+#             */
/*   Updated: 2025/12/23 23:54:47 by bliu             ###   ########.fr       */
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

static void	get_plane_uv(t_vec3 p, double *u, double *v)
{
	*u = p.x - floor(p.x);
	*v = p.z - floor(p.z);
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
	// record->g_norm = plane->normal;
	record->is_d_side = 1;
	set_face_normal(ray, plane->normal, record);
	record->g_norm = record->normal;
	get_plane_uv(vec3_sub(record->p, plane->point), &record->u, &record->v);
	record->normal = apply_bump(get_tbn_plane(), record->u,
			record->v, sine_bump);
	record->mat = plane->mat;
	return (1);
}
