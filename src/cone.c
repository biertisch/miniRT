/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:18:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/12 16:29:07 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_cone	new_cone(t_vec3 apex, t_vec3 axis, double radius, double height, t_material mat)
{
	return ((t_cone){apex, axis, radius, height, mat});
}

static t_vec3	compute_normal(t_cone *cone, t_vec3 v, t_vec3 axis, double proj)
{
	t_vec3	parallel;
	t_vec3	perp;
	double	theta;
	double	k;

	theta = atan(cone->radius / cone->height);
	parallel = vec3_multiply(axis, proj);
	perp = vec3_subtract(v, parallel);
	k = vec3_length(perp) / tan(theta);
	return (vec3_normalize(vec3_subtract(perp, vec3_multiply(axis, k))));
}

static int	check_height(t_ray *ray, t_cone *cone, double t, t_hit_record *rec)
{
	t_vec3	p;
	t_vec3	v;
	t_vec3	axis;
	double	proj;

	p = ray_at(ray, t);
	v = vec3_subtract(p, cone->apex);
	axis = vec3_normalize(cone->axis);
	proj = vec3_dot(v, axis);
	if (proj < 0 || proj > cone->height)
		return (0);
	rec->t = t;
	rec->p = p;
	//store color?
	rec->normal = compute_normal(cone, v, axis, proj);
	return (1);
}

static int	check_cone_base(t_ray *ray, t_interval *ray_t, t_cone *cone, t_hit_record *record)
{
	t_vec3	center;
	t_vec3	axis;
	t_vec3	d;
	t_vec3	p;
	t_vec3	v;
	double	denom;
	double	t;

	axis = vec3_normalize(cone->axis);
	d = vec3_normalize(ray->direction);
	center = vec3_add(cone->apex, vec3_multiply(axis, cone->height));
	denom = vec3_dot(d, axis);
	if (fabs(denom) <= 1e-8)
		return (0);
	t = vec3_dot(vec3_subtract(center, ray->origin), axis) / denom;
	if (!interval_surrounds(ray_t, t))
		return (0);
	p = ray_at(ray, t);
	v = vec3_subtract(p, center);
	if (vec3_length_squared(v) <= cone->radius * cone->radius)
	{
		record->t = t;
		record->p = p;
		record->normal = axis;
		ray_t->max = t;
		return (1);
	}
	return (0);
}

static int	check_cone_side(t_ray *ray, t_interval *ray_t, t_cone *cone, t_hit_record *record, double t)
{
	t_hit_record	tmp;

	if (interval_surrounds(ray_t, t) && check_height(ray, cone, t, &tmp))
	{
		*record = tmp;
		ray_t->max = tmp.t;
		return (1);
	}
	return (0);
}

static int	solve_cone_quadratic(t_ray *ray, t_cone *cone, double *t1, double *t2)
{
	t_vec3	d;
	t_vec3	v;
	t_vec3	w;
	double	theta;
	double	a;
	double	b;
	double	c;
	double	disc;

	theta = atan(cone->radius / cone->height);
	d = vec3_normalize(ray->direction); // necessary?
	v = vec3_normalize(cone->axis); // necessary?
	w = vec3_subtract(ray->origin, cone->apex);
	a = pow(vec3_dot(d, v), 2) - pow(cos(theta), 2);
	b = 2 * (vec3_dot(d, v) * vec3_dot(w, v) - pow(cos(theta), 2) * vec3_dot(d, w));
	c = pow(vec3_dot(w, v), 2) - pow(cos(theta), 2) * vec3_dot(w, w);
	disc =	pow(b, 2) - 4 * a * c;
	if (disc < 0)
		return (0);
	*t1 = (-b - sqrt(disc)) / (2 * a);
	*t2 = (-b + sqrt(disc)) / (2 * a);
	return (1);
}

int	cone_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_cone			*cone;
	double			t1;
	double			t2;
	int				hit_any;

	cone = &obj.geo.cone;
	if (!solve_cone_quadratic(ray, cone, &t1, &t2))
		return (0);

	hit_any = 0;
	if (check_cone_side(ray, &ray_t, cone, record, t1))
		hit_any = 1;
	if (check_cone_side(ray, &ray_t, cone, record, t2))
		hit_any = 1;
	if (check_cone_base(ray, &ray_t, cone, record))
		hit_any = 1;
	return (hit_any);
}
