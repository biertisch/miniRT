/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:15:34 by bliu              #+#    #+#             */
/*   Updated: 2026/01/02 00:29:15 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_hit_record_triangle(t_hit_record *h_rec, t_triangle *t, t_ray *ray)
{
	h_rec->p = ray_at(ray, h_rec->t);
	h_rec->g_norm = t->normal;
	h_rec->is_d_side = 0;
	h_rec->mat = t->mat;
	set_face_normal(ray, t->normal, h_rec);
}

t_aabb	triangle_get_aabb(t_triangle *tri)
{
	t_aabb	box;
	double	c_eps;

	box.min.x = fmin(fmin(tri->p1.x, tri->p2.x), tri->p3.x);
	box.min.y = fmin(fmin(tri->p1.y, tri->p2.y), tri->p3.y);
	box.min.z = fmin(fmin(tri->p1.z, tri->p2.z), tri->p3.z);
	box.max.x = fmax(fmax(tri->p1.x, tri->p2.x), tri->p3.x);
	box.max.y = fmax(fmax(tri->p1.y, tri->p2.y), tri->p3.y);
	box.max.z = fmax(fmax(tri->p1.z, tri->p2.z), tri->p3.z);
	c_eps = 1e-6;
	box.min.x -= c_eps;
	box.max.x += c_eps;
	box.min.y -= c_eps;
	box.max.y += c_eps;
	box.min.z -= c_eps;
	box.max.z += c_eps;
	return (box);
}

int	axis_check(t_interval box, t_interval ray_t, double r_dirv, double r_oriv)
{
	double		inv_d;
	double		t0;
	double		t1;
	double		tmp;

	inv_d = 1.0 / r_dirv;
	t0 = (box.min - r_oriv) * inv_d;
	t1 = (box.max - r_oriv) * inv_d;
	if (inv_d < 0.0)
	{
		tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
	ray_t.min = fmax(t0, ray_t.min);
	ray_t.max = fmin(t1, ray_t.max);
	if (ray_t.max < ray_t.min)
		return (0);
	return (1);
}

int	ray_aabb_hit(t_ray *ray, t_aabb box, t_interval ray_t)
{
	if (!axis_check((t_interval){box.min.x, box.max.x}, ray_t,
		ray->direction.x, ray->origin.x))
		return (0);
	if (!axis_check((t_interval){box.min.y, box.max.y}, ray_t,
		ray->direction.y, ray->origin.y))
		return (0);
	if (!axis_check((t_interval){box.min.z, box.max.z}, ray_t,
		ray->direction.z, ray->origin.z))
		return (0);
	return (1);
}

int	triangle_hit2(t_ray *ray, t_interval ray_t, t_triangle *o_t,
	t_hit_record *h_rec)
{
	// t_triangle	*o_t;
	t_vec3		h;
	t_vec3		s;
	t_interval	a_f;

	// o_t = &obj;
	if (!ray_aabb_hit(ray, o_t->bbox, ray_t))
		return (0);
	h = vec3_cross(ray->direction, o_t->e2);
	a_f.min = vec3_dot(o_t->e1, h);
	if (fabs(a_f.min) < 1e-6)
		return (0);
	a_f.max = 1.0 / a_f.min;
	s = vec3_sub(ray->origin, o_t->p1);
	h_rec->u = a_f.max * vec3_dot(s, h);
	if (h_rec->u < 0.0 || h_rec->u > 1.0)
		return (0);
	h_rec->v = a_f.max * vec3_dot(ray->direction, vec3_cross(s, o_t->e1));
	if (h_rec->v < 0.0 || h_rec->u + h_rec->v > 1.0)
		return (0);
	h_rec->t = a_f.max * vec3_dot(o_t->e2, vec3_cross(s, o_t->e1));
	if (h_rec->t < ray_t.min || h_rec->t > ray_t.max)
		return (0);
	set_hit_record_triangle(h_rec, o_t, ray);
	return (1);
}

int	triangle_hit(t_ray *ray, t_interval ray_t, t_object obj,
	t_hit_record *h_rec)
{
	t_triangle	*o_t;
	t_vec3		h;
	t_vec3		s;
	t_interval	a_f;

	o_t = &obj.geo.triangle;
	if (!ray_aabb_hit(ray, o_t->bbox, ray_t))
		return (0);
	h = vec3_cross(ray->direction, o_t->e2);
	a_f.min = vec3_dot(o_t->e1, h);
	if (fabs(a_f.min) < 1e-6)
		return (0);
	a_f.max = 1.0 / a_f.min;
	s = vec3_sub(ray->origin, o_t->p1);
	h_rec->u = a_f.max * vec3_dot(s, h);
	if (h_rec->u < 0.0 || h_rec->u > 1.0)
		return (0);
	h_rec->v = a_f.max * vec3_dot(ray->direction, vec3_cross(s, o_t->e1));
	if (h_rec->v < 0.0 || h_rec->u + h_rec->v > 1.0)
		return (0);
	h_rec->t = a_f.max * vec3_dot(o_t->e2, vec3_cross(s, o_t->e1));
	if (h_rec->t < ray_t.min || h_rec->t > ray_t.max)
		return (0);
	set_hit_record_triangle(h_rec, o_t, ray);
	return (1);
}
