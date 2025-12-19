/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:18:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 19:04:03 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// remove?
t_cone	new_cone(t_vec3 apex, t_vec3 axis, double radius, double height,
	t_material mat)
{
	return ((t_cone){apex, axis, radius, height, mat});
}

static t_vec3	cone_normal(t_cone *cone, t_vec3 v, t_vec3 axis, double proj)
{
	t_vec3	m;
	t_vec3	outward;

	m = vec3_mul_n(axis, proj);
	outward = vec3_sub(v, m);
	return (vec3_norm(outward));
}

static int	check_cone_base(t_ray *ray, t_interval *ray_t, t_cone *cone,
	t_hit_record *rec)
{
	t_vec3	center;
	t_vec3	axis;
	t_vec3	p;
	double	denom;
	double	t;

	axis = vec3_mul_n(vec3_norm(cone->axis), -1);
	center = vec3_add(cone->apex, vec3_mul_n(axis, cone->height));
	denom = vec3_dot(ray->direction, axis);
	if (fabs(denom) <= 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(center, ray->origin), axis) / denom;
	if (t < ray_t->min || t > ray_t->max)
		return (0);
	p = ray_at(ray, t);
	if (vec3_length_squared(vec3_sub(p, center))
		> cone->radius * cone->radius)
		return (0);
	rec->t = t;
	rec->p = p;
	rec->mat = cone->mat;
	rec->normal = axis;
	set_face_normal(ray, axis, rec);
	ray_t->max = t;
	return (1);
}

static int	check_cone_side(t_ray *ray, t_cone *cone, t_hit_record *rec,
	double t)
{
	t_vec3	p;
	t_vec3	v;
	t_vec3	axis;
	double	proj;

	p = ray_at(ray, t);
	v = vec3_sub(p, cone->apex);
	axis = vec3_mul_n(vec3_norm(cone->axis), -1);
	proj = vec3_dot(v, axis);
	if (proj < 0 || proj > cone->height)
		return (0);
	rec->t = t;
	rec->p = p;
	rec->mat = cone->mat;
	rec->normal = cone_normal(cone, v, axis, proj);
	set_face_normal(ray, rec->normal, rec);
	return (1);
}

int	cone_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *rec)
{
	t_cone			*c;
	double			t1;
	double			t2;
	int				hit_any;

	c = &obj.geo.cone;
	if (!solve_cone_quadratic(ray, c, &t1, &t2))
		return (0);
	hit_any = 0;
	if (t1 >= ray_t.min && t1 <= ray_t.max && check_cone_side(ray, c, rec, t1))
	{
		ray_t.max = t1;
		hit_any = 1;
	}
	if (t2 >= ray_t.min && t2 <= ray_t.max && check_cone_side(ray, c, rec, t2))
	{
		ray_t.max = t2;
		hit_any = 1;
	}
	if (check_cone_base(ray, &ray_t, c, rec))
		hit_any = 1;
	if (hit_any)
		rec->hit_obj = &obj;
	return (hit_any);
}
