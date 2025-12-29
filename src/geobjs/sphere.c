/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:27:57 by bliu              #+#    #+#             */
/*   Updated: 2025/12/29 11:21:56 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
// unnecessary for parser
t_sphere	new_sphere(t_vec3 center, double radius, t_material mat)
{
	t_sphere	sphere;

	sphere.center = center;
	sphere.radius = radius;
	sphere.mat = mat;
	return (sphere);
}
*/

void	set_face_normal(t_ray *ray, t_vec3 outward_normal, t_hit_record *record)
{
	if (vec3_dot(ray->direction, outward_normal) < 0)
	{
		record->front_face = 1;
		record->normal = outward_normal;
	}
	else
	{
		record->front_face = 0;
		record->normal = vec3_mul_n(outward_normal, -1);
	}
}

static void	get_sphere_uv(t_vec3 p, double *u, double *v)
{
	*u = 0.5 + (atan2(p.z, p.x) / (2 * M_PI));
	*v = 0.5 - (asin(p.y) / M_PI);
}

static int	root_calc(t_sphere *s, t_ray *ray, t_interval ray_t, double *root)
{
	t_vec3		oc;
	double		a;
	double		h;
	double		discriminant;
	double		sqrtd;

	oc = vec3_sub(s->center, ray->origin);
	a = vec3_dot(ray->direction, ray->direction);
	h = vec3_dot(oc, ray->direction);
	discriminant = h * h - a * (vec3_dot(oc, oc) - (s->radius * s->radius));
	if (discriminant < 0)
		return (0);
	sqrtd = sqrt(discriminant);
	*root = (h - sqrtd) / a;
	if (!interval_surrounds(&ray_t, *root))
	{
		*root = (h + sqrtd) / a;
		if (!interval_surrounds(&ray_t, *root))
			return (0);
	}
	return (1);
}

int	sphere_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_sphere	*s;
	double		root;

	s = &obj.geo.sphere;
	if (!root_calc(s, ray, ray_t, &root))
		return (0);
	record->t = root;
	record->p = ray_at(ray, record->t);
	record->g_norm = vec3_norm(vec3_mul_n(vec3_sub(record->p, s->center),
				1.0 / s->radius));
	record->is_d_side = 0;
	set_face_normal(ray, record->g_norm, record);
	get_sphere_uv(record->g_norm, &record->u, &record->v);
	if (obj.tex_type == BUMP_FUNC)
		record->g_norm = apply_bump_f(sphere_tbn(record->g_norm),
				record->u, record->v, sine_bump);
	else if (obj.tex_type == PICTURE)
		record->g_norm = apply_bump_map(sphere_tbn(record->g_norm),
				bump_tangent_normal(
					&((t_pic_tex *)s->mat.data.lamb.tex)->bump_tex,
					record->u, record->v, 4));
	record->mat = s->mat;
	return (1);
}
