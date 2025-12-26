/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:12:47 by bliu              #+#    #+#             */
/*   Updated: 2025/12/26 22:53:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	on_cy_side(t_ray *ray, t_cylinder *cy, double t,
				t_hit_record *rec)
{
	t_vec3	p;
	t_vec3	axis;
	t_vec3	v;
	double	proj;
	t_vec3	proj_point;

	p = ray_at(ray, t);
	axis = vec3_norm(cy->axis);
	v = vec3_sub(p, cy->center);
	proj = vec3_dot(v, axis);
	if (proj < -(cy->height / 2.0) || proj > cy->height / 2.0)
		return (0);
	proj_point = vec3_add(cy->center, vec3_mul_n(axis, proj));
	rec->t = t;
	rec->p = p;
	rec->mat = cy->mat;
	rec->is_d_side = 0;
	rec->g_norm = vec3_norm(vec3_sub(p, proj_point));
	set_face_normal(ray, vec3_norm(vec3_sub(p, proj_point)), rec);
	return (1);
}

int	calc_cylinder_side_roots(t_ray *ray, t_cylinder *cy,
	double *t1, double *t2)
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

int	cylinder_side_check_v1(t_ray *ray, t_cylinder *cy, t_interval *ray_t,
	t_hit_record *rec)
{
	double			t1;
	double			t2;
	int				hit_side;

	hit_side = 0;
	if (calc_cylinder_side_roots(ray, cy, &t1, &t2))
	{
		if (t1 >= ray_t->min && t1 < ray_t->max
			&& on_cy_side(ray, cy, t1, rec))
		{
			ray_t->max = rec->t;
			hit_side = 2;
		}
		if (t2 >= ray_t->min && t2 < ray_t->max
			&& on_cy_side(ray, cy, t2, rec))
		{
			ray_t->max = rec->t;
			hit_side = 2;
		}
	}
	return (hit_side);
}

int	cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *rec)
{
	t_cylinder	*cy;
	int			side_face;
	int			tp_face;
	int			hit_any;

	cy = &obj.geo.cylinder;
	side_face = 0;
	tp_face = 0;
	hit_any = 0;
	cy->axis = vec3_norm(cy->axis);
	side_face = cylinder_side_check_v1(ray, cy, &ray_t, rec);
	tp_face = cylinder_cap_check_v1(ray, &ray_t, cy, rec);
	if (side_face || tp_face)
		hit_any = 1;
	if (tp_face)
		cylinder_uv(cy, rec, tp_face);
	else if (side_face)
		cylinder_uv(cy, rec, side_face);
	if (hit_any)
		rec->is_d_side = 0;
	return (hit_any);
}

t_vec3	apply_bumpf_cylinder(t_tbn tbn, t_hit_record *rec, int face_hit,
	double (*height)(double, double))
{
	double	eps;
	double	scale;
	double	du;
	double	dv;
	t_vec3	bumped;

	eps = 0.001;
	scale = 0.1;
	(void)face_hit;
	if (face_hit == 1)
		return (tbn.cb);
	if (face_hit == 3)
		return (vec3_mul_n(tbn.cb, -1));
	du = height(rec->u + eps, rec->v) - height(rec->u - eps, rec->v);
	dv = height(rec->u, rec->v + eps) - height(rec->u, rec->v - eps);
	bumped = vec3_add(tbn.cn, vec3_add(
				vec3_mul_n(tbn.ct, du * scale),
				vec3_mul_n(tbn.cb, dv * scale)));
	return (vec3_norm(bumped));
}

void	change_cynormal_according_bump(t_cylinder *c, t_hit_record *rec,
	int face_hit)
{
	if (((t_object *)c)->tex_type == BUMP_FUNC)
		rec->g_norm = apply_bumpf_cylinder(cylinder_tbn(rec->p, c->center,
					c->axis), rec, face_hit, sine_bump);
	else if (((t_object *)c)->tex_type == PICTURE)
	{
		if (face_hit == 2)
		{
			rec->g_norm = apply_bump_map(cylinder_tbn(rec->p, c->center,
						c->axis), bump_tangent_normal(
						&((t_pic_tex *)c->mat.data.lamb.tex)->bump_tex,
						rec->u, rec->v, 0.1));
		}
		else if (face_hit == 1)
			rec->g_norm = c->axis;
		else if (face_hit == 3)
			rec->g_norm = vec3_mul_n(c->axis, -1);
	}
}

/*

int	cylinder_side_check_v0(t_ray *ray, t_cylinder *cy, t_interval *ray_t,
	t_hit_record *rec)
{
	t_hit_record	tmp;
	t_vec3			d_cross_a;
	t_vec3			oc_cross_a;
	double			a;
	double			b;
	double			c;
	double			disc;

	d_cross_a = vec3_cross(ray->direction, cy->axis);
	oc_cross_a = vec3_cross(vec3_sub(ray->origin, cy->center), cy->axis);
	a = vec3_dot(d_cross_a, d_cross_a);
	b = 2.0 * vec3_dot(d_cross_a, oc_cross_a);
	c = vec3_dot(oc_cross_a, oc_cross_a) - cy->radius * cy->radius;
	disc = b * b - 4 * a * c;
    if (disc >= 0)
    {
        double sqrt_disc = sqrt(disc);
        double t1 = (-b - sqrt_disc) / (2 * a);
        double t2 = (-b + sqrt_disc) / (2 * a);
        if (t1 >= ray_t->min && t1 < ray_t->max)
        {
            if (check_cylinder_side(ray, cy, t1, &tmp))
            {
                *rec = tmp;
				ray_t->max = tmp.t;
                return (2);
            }
        }
        if (t2 >= ray_t->min && t2 < ray_t->max)
        {
            if (check_cylinder_side(ray, cy, t2, &tmp))
            {
                *rec = tmp;
				ray_t->max = tmp.t;
                return (2);
            }
        }
    }
	return (0);
}

// Returns 0 if no hit, 3 if bottom cap hit, 1 if top cap hit
int	cylinder_cap_check_v0(t_ray *ray, t_interval *ray_t, t_cylinder *cy,
	t_hit_record *rec)
{
	int		hit_any;
	t_vec3	axis;
    double	denom;

	hit_any = 0;
	axis = vec3_norm(cy->axis);
	denom = vec3_dot(ray->direction, axis);
	cy->axis = vec3_norm(cy->axis);
    if (fabs(denom) > 1e-8)
    {
        // bottom cap
		t_vec3 b_cap = vec3_sub(cy->center, vec3_mul_n(axis, cy->height / 2.0));
        double tcap = vec3_dot(vec3_sub(b_cap, ray->origin), axis) / denom;
	return box;
}    
	if (tcap >= ray_t->min && tcap < ray_t->max)  // ← Check against closest_t
	{
		t_vec3 p = ray_at(ray, tcap);
		if (vec3_length_squared(vec3_sub(p, b_cap)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, vec3_mul_n(axis, -1), rec);
			ray_t->max = tcap;
			hit_any = 3;
		}
	}
	// top cap
	t_vec3 t_cap = vec3_add(cy->center, vec3_mul_n(axis, cy->height / 2.0));
	tcap = vec3_dot(vec3_sub(t_cap, ray->origin), axis) / denom;
	if (tcap >= ray_t->min && tcap < ray_t->max)
	{
		t_vec3 p = ray_at(ray, tcap);
		if (vec3_length_squared(vec3_sub(p, t_cap)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, axis, rec);
			ray_t->max = tcap;
			hit_any = 1;
		}
	}
    }
	return (hit_any);
}

// unnecessary for parser
t_cylinder	new_cylinder(t_vec3 center, t_vec3 axis, double radius,
	double height, t_material mat)
{
	t_cylinder	cylinder;

	cylinder.center = center;
	cylinder.axis = vec3_norm(axis);
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.mat = mat;
	return (cylinder);
}

t_aabb cylinder_bbox(t_cylinder *cy)
{
	t_vec3 half_axis = vec3_multiply(cy->axis, cy->height / 2.0);
	t_vec3 p1 = vec3_subtract(cy->center, half_axis);
	t_vec3 p2 = vec3_add(cy->center, half_axis);

	t_aabb box;
	box.x.min = fmin(p1.x, p2.x) - cy->radius;
	box.x.max = fmax(p1.x, p2.x) + cy->radius;
	box.y.min = fmin(p1.y, p2.y) - cy->radius;
	box.y.max = fmax(p1.y, p2.y) + cy->radius;
	box.z.min = fmin(p1.z, p2.z) - cy->radius;
	box.z.max = fmax(p1.z, p2.z) + cy->radius;

	return box;
}

t_aabb	cylinder_compute_bbox(t_cylinder *cy)
{
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	half_axis;
	t_aabb	bbox;

	// 半高方向向量
	half_axis = vec3_multiply(cy->axis, cy->height / 2.0);
	// 上下端点
	p1 = vec3_subtract(cy->center, half_axis);
	p2 = vec3_add(cy->center, half_axis);

	// X 轴范围
	bbox.x.min = fmin(p1.x, p2.x) - cy->radius;
	bbox.x.max = fmax(p1.x, p2.x) + cy->radius;

	// Y 轴范围
	bbox.y.min = fmin(p1.y, p2.y) - cy->radius;
	bbox.y.max = fmax(p1.y, p2.y) + cy->radius;

	// Z 轴范围
	bbox.z.min = fmin(p1.z, p2.z) - cy->radius;
	bbox.z.max = fmax(p1.z, p2.z) + cy->radius;
	return (bbox);
}

static int	check_cylinder_caps(t_ray *ray, t_cylinder *cylinder,
 double t, t_hit_record *record)
{
t_vec3	hit_point;
t_vec3	to_hit;
double	distance_along_axis;
double	distance_from_axis_squared;

hit_point = ray_at(ray, t);
to_hit = vec3_subtract(hit_point, cylinder->center);
distance_along_axis = vec3_dot(to_hit, cylinder->axis);

// Check if hit point is within the cylinder height bounds
if (distance_along_axis < 0 || distance_along_axis > cylinder->height)
	return (0);

// Calculate distance from cylinder axis
t_vec3 projection_on_axis = vec3_multiply(cylinder->axis, distance_along_axis);
t_vec3 radial_vector = vec3_subtract(to_hit, projection_on_axis);
distance_from_axis_squared = vec3_dot(radial_vector, radial_vector);

// Check if hit point is within cylinder radius
if (distance_from_axis_squared > cylinder->radius * cylinder->radius)
	return (0);

// Calculate outward normal at hit point
t_vec3 outward_normal = vec3_multiply(radial_vector, 1.0 / cylinder->radius);

record->t = t;
record->p = hit_point;
set_face_normal(ray, outward_normal, record);
record->mat = cylinder->mat;

return (1);
}

int	cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj,
	t_hit_record *record)
{
	t_cylinder	*cylinder;
	t_vec3		oc;
	t_vec3		ray_cross_axis;
	t_vec3		oc_cross_axis;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		sqrt_discriminant;
	double		t1;
	double		t2;

	cylinder = &obj.geo.cylinder;
	
	// Vector from ray origin to cylinder center
	oc = vec3_subtract(ray->origin, cylinder->center);
	
//For infinite cylinder, we solve: ||(o + td) - c - ((o + td - c) · a)a||² = r²
	// This reduces to a quadratic equation in t
	ray_cross_axis = vec3_cross(ray->direction, cylinder->axis);
	oc_cross_axis = vec3_cross(oc, cylinder->axis);
	
	a = vec3_dot(ray_cross_axis, ray_cross_axis);
	b = 2.0 * vec3_dot(ray_cross_axis, oc_cross_axis);
c = vec3_dot(oc_cross_axis, oc_cross_axis) 
	- (cylinder->radius * cylinder->radius);
	
	discriminant = b * b - 4.0 * a * c;
	
	// No intersection with infinite cylinder
	if (discriminant < 0)
		return (0);
	
	sqrt_discriminant = sqrt(discriminant);
	
	// Calculate both intersection points
	t1 = (-b - sqrt_discriminant) / (2.0 * a);
	t2 = (-b + sqrt_discriminant) / (2.0 * a);
	
	// Check first intersection
	if (interval_surrounds(&ray_t, t1))
	{
		if (check_cylinder_caps(ray, cylinder, t1, record))
			return (1);
	}
	
	// Check second intersection
	if (interval_surrounds(&ray_t, t2))
	{
		if (check_cylinder_caps(ray, cylinder, t2, record))
			return (1);
	}
	
	return (0);
}

int	cylinder_hit_old(t_ray *ray, t_interval ray_t, t_object obj,
 t_hit_record *rec)
{
	t_cylinder	*cy = &obj.geo.cylinder;
	t_vec3		oc = vec3_subtract(ray->origin, cy->center);
	t_vec3		axis = vec3_normalize(cy->axis);
	double		a, b, c, disc, sqrt_disc;
	double		t1, t2;
	t_hit_record	tmp;
	int			hit_any = 0;

	// ==== Solve for side surface ====
	t_vec3 d_cross_a = vec3_cross(ray->direction, axis);
	t_vec3 oc_cross_a = vec3_cross(oc, axis);
	a = vec3_dot(d_cross_a, d_cross_a);
	b = 2.0 * vec3_dot(d_cross_a, oc_cross_a);
	c = vec3_dot(oc_cross_a, oc_cross_a) - cy->radius * cy->radius;
	disc = b * b - 4 * a * c;
	if (disc >= 0)
	{
		sqrt_disc = sqrt(disc);
		t1 = (-b - sqrt_disc) / (2 * a);
		t2 = (-b + sqrt_disc) / (2 * a);

		// Check both intersections (side)
		if (interval_surrounds(&ray_t, t1))
		{
			if (check_cylinder_side(ray, cy, t1, &tmp))
			{
				*rec = tmp;
				ray_t.max = tmp.t;
				hit_any = 1;
			}
		}
		if (interval_surrounds(&ray_t, t2))
		{
			if (check_cylinder_side(ray, cy, t2, &tmp))
			{
				*rec = tmp;
				ray_t.max = tmp.t;
				hit_any = 1;
			}
		}
	}

	// ==== Check top/bottom caps ====
	t_vec3 half_axis = vec3_multiply(axis, cy->height / 2.0);
	t_vec3 cap1 = vec3_subtract(cy->center, half_axis);
	t_vec3 cap2 = vec3_add(cy->center, half_axis);
	double denom = vec3_dot(ray->direction, axis);

	// bottom cap
	if (fabs(denom) > 1e-8)
	{
	double tcap = vec3_dot(vec3_subtract(cap1, ray->origin), axis) / denom;
	if (interval_surrounds(&ray_t, tcap))
	{
		t_vec3 p = ray_at(ray, tcap);
	if (vec3_length_squared(vec3_subtract(p, cap1)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, vec3_multiply(axis, -1), rec);
			ray_t.max = tcap;
			hit_any = 1;p cap
	tcap = vec3_dot(vec3_subtract(cap2, ray->origin), axis) / denom;
	if (interval_surrounds(&ray_t, tcap))
	{
		t_vec3 p = ray_at(ray, tcap);
	if (vec3_length_squared(vec3_subtract(p, cap2)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, axis, rec);
			ray_t.max = tcap;
			hit_any = 1;
		}
	}
		}
	}

	// top cap
	tcap = vec3_dot(vec3_subtract(cap2, ray->origin), axis) / denom;
	if (interval_surrounds(&ray_t, tcap))
	{
		t_vec3 p = ray_at(ray, tcap);
	if (vec3_length_squared(vec3_subtract(p, cap2)) <= cy->radius * cy->radius)
		{
			rec->t = tcap;
			rec->p = p;
			rec->mat = cy->mat;
			set_face_normal(ray, axis, rec);
			ray_t.max = tcap;
			hit_any = 1;
		}
	}
	}

	return (hit_any);
}
*/