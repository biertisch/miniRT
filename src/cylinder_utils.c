/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:58:57 by bliu              #+#    #+#             */
/*   Updated: 2025/12/23 18:19:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	on_cylinder_bcap(t_ray *ray, t_interval *ray_t,
	t_cylinder *cy, t_hit_record *rec)
{
	t_vec3	b_cap;
	t_vec3	p;
	double	denom;
	double	bcap;
	int		hit_any_local;

	hit_any_local = 0;
	denom = vec3_dot(ray->direction, cy->axis);
	b_cap = vec3_sub(cy->center, vec3_mul_n(cy->axis, cy->height / 2.0));
	bcap = vec3_dot(vec3_sub(b_cap, ray->origin), cy->axis) / denom;
	if (bcap >= ray_t->min && bcap < ray_t->max)
	{
		p = ray_at(ray, bcap);
		if (vec3_length_squared(vec3_sub(p, b_cap)) <= cy->radius * cy->radius)
		{
			rec->t = bcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, vec3_mul_n(cy->axis, -1), rec);
			ray_t->max = bcap;
			hit_any_local = 3;
		}
	}
	return (hit_any_local);
}

int	on_cylinder_tcap(t_ray *ray, t_interval *ray_t,
	t_cylinder *cy, t_hit_record *rec)
{
	t_vec3	t_cap;
	t_vec3	p;
	double	denom;
	double	tcap;
	int		hit_any_local;

	hit_any_local = 0;
	denom = vec3_dot(ray->direction, cy->axis);
	t_cap = vec3_add(cy->center, vec3_mul_n(cy->axis, cy->height / 2.0));
	tcap = vec3_dot(vec3_sub(t_cap, ray->origin), cy->axis) / denom;
	if (tcap >= ray_t->min && tcap < ray_t->max)
	{
		p = ray_at(ray, tcap);
		if (vec3_length_squared(vec3_sub(p, t_cap)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, cy->axis, rec);
			ray_t->max = tcap;
			hit_any_local = 1;
		}
	}
	return (hit_any_local);
}

// Returns 0 if no hit, 3 if bottom cap hit, 1 if top cap hit
int	cylinder_cap_check_v1(t_ray *ray, t_interval *ray_t, t_cylinder *cy,
	t_hit_record *rec)
{
	int		hit_any;
	double	denom;

	hit_any = 0;
	denom = vec3_dot(ray->direction, cy->axis);
	if (fabs(denom) > 1e-8)
	{
		if (on_cylinder_bcap(ray, ray_t, cy, rec) == 3)
			hit_any = 3;
		if (on_cylinder_tcap(ray, ray_t, cy, rec) == 1)
			hit_any = 1;
	}
	return (hit_any);
}

void	cylinder_side_uv(t_cylinder *c, t_hit_record *rec, t_vec3 v_axis,
	t_vec3 vv_axis)
{
	t_vec3	c2hit;
	double	t;

	c2hit = vec3_sub(rec->p, c->center);
	t = vec3_dot(c2hit, c->axis);
	rec->v = t / c->height;
	c2hit = vec3_sub(c2hit, vec3_mul_n(c->axis, t));
	rec->u = atan2(vec3_dot(c2hit, vv_axis),
			vec3_dot(c2hit, v_axis)) / (2.0 * M_PI);
	if (rec->u < 0)
		rec->u += 1.0;
}

// face:  1 - top cap, 2 - side, 3 - bottom cap
void	cylinder_uv(t_cylinder *c, t_hit_record *rec, int face)
{
	t_vec3	v_axis;
	t_vec3	vv_axis;
	t_vec3	c2hit;
	t_vec3	center;

	center = (t_vec3){0, 0, 0};
	if (fabs(c->axis.y) < 0.999)
		v_axis = vec3_norm(vec3_cross(c->axis, (t_vec3){0, 1, 0}));
	else
		v_axis = vec3_norm(vec3_cross(c->axis, (t_vec3){1, 0, 0}));
	vv_axis = vec3_cross(c->axis, v_axis);
	if (face == 2)
		cylinder_side_uv(c, rec, v_axis, vv_axis);
	else
	{
		if (face == 1)
			center = vec3_add(c->center, vec3_mul_n(c->axis, c->height / 2.0));
		else
			center = vec3_sub(c->center, vec3_mul_n(c->axis, c->height / 2.0));
		c2hit = vec3_sub(rec->p, center);
		rec->u = 0.5 + vec3_dot(c2hit, v_axis) / (2.0 * c->radius);
		rec->v = 0.5 + vec3_dot(c2hit, vv_axis) / (2.0 * c->radius);
	}
	rec->normal = apply_bump(get_tbn_cylinder(vec3_sub(rec->p, c->center), c->axis),
		rec->u, rec->v, sine_bump);
}
