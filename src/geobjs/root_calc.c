/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root_calc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 12:24:18 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 12:31:32 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	calc_sphere_roots(t_sphere *s, t_ray *ray, t_interval ray_t, double *root)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	discriminant;
	double	sqrtd;

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

int	calc_cylinder_side_roots(t_ray *ray, t_cylinder *cy, double *t1, double *t2)
{
	t_vec3			d_cross_a;
	t_vec3			oc_cross_a;
	t_roots_holder	rh;

	rh = (t_roots_holder){0};
	d_cross_a = vec3_cross(ray->direction, cy->axis);
	oc_cross_a = vec3_cross(vec3_sub(ray->origin, cy->center), cy->axis);
	rh.a = vec3_dot(d_cross_a, d_cross_a);
	rh.b = 2.0 * vec3_dot(d_cross_a, oc_cross_a);
	rh.c = vec3_dot(oc_cross_a, oc_cross_a) - cy->radius * cy->radius;
	rh.disc = rh.b * rh.b - 4 * rh.a * rh.c;
	if (rh.disc >= 0)
	{
		rh.sqrt_disc = sqrt(rh.disc);
		*t1 = (-rh.b - rh.sqrt_disc) / (2 * rh.a);
		*t2 = (-rh.b + rh.sqrt_disc) / (2 * rh.a);
	}
	return (rh.disc >= 0);
}
