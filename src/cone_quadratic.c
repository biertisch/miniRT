/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_quadratic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 16:58:29 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/17 17:02:16 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static double	get_cone_c(t_cone *cone, t_vec3 o)
{
	t_vec3	v;
	t_vec3	w;
	double	c;
	double	k;

	v = vec3_multiply(vec3_normalize(cone->axis), -1);
	w = vec3_subtract(o, cone->apex);
	k = cone->radius / cone->height;
	c = vec3_dot(w, w) - (1 + k * k) * pow(vec3_dot(w, v), 2);
	return (c);
}

static double	get_cone_b(t_cone *cone, t_vec3 d, t_vec3 o)
{
	t_vec3	v;
	t_vec3	w;
	double	b;
	double	k;

	v = vec3_multiply(vec3_normalize(cone->axis), -1);
	w = vec3_subtract(o, cone->apex);
	k = cone->radius / cone->height;
	b = 2 * (vec3_dot(d, w) - (1 + k * k) * vec3_dot(d, v) * vec3_dot(w, v));
	return (b);
}

static double	get_cone_a(t_cone *cone, t_vec3 d)
{
	t_vec3	v;
	double	a;
	double	k;

	v = vec3_multiply(vec3_normalize(cone->axis), -1);
	k = cone->radius / cone->height;
	a = vec3_dot(d, d) - (1 + k * k) * (pow(vec3_dot(d, v), 2));
	return (a);
}

int	solve_cone_quadratic(t_ray *ray, t_cone *cone, double *t1, double *t2)
{
	double	a;
	double	b;
	double	c;
	double	disc;
	double	sqrt_disc;

	a = get_cone_a(cone, ray->direction);
	b = get_cone_b(cone, ray->direction, ray->origin);
	c = get_cone_c(cone, ray->origin);
	disc = b * b - 4 * a * c;
	if (disc < 0)
		return (0);
	sqrt_disc = sqrt(disc);
	*t1 = (-b - sqrt_disc) / (2 * a);
	*t2 = (-b + sqrt_disc) / (2 * a);
	return (1);
}
