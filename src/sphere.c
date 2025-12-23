/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:27:57 by bliu              #+#    #+#             */
/*   Updated: 2025/12/23 18:37:00 by bliu             ###   ########.fr       */
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
	t_vec3		otwrd_norm;

	s = &obj.geo.sphere;
	if (!root_calc(s, ray, ray_t, &root))
		return (0);
	record->t = root;
	record->p = ray_at(ray, record->t);
	otwrd_norm = vec3_mul_n(vec3_sub(record->p, s->center), 1.0 / s->radius);
	set_face_normal(ray, otwrd_norm, record);
	get_sphere_uv(otwrd_norm, &record->u, &record->v);
// apply_sphere_bump(record, s);
	// record->normal = apply_bump(get_tbn_sphere(otwrd_norm), record->u, record->v, sine_bump);
	record->mat = s->mat;
	return (1);
}
