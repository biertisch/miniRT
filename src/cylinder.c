#include "minirt.h"

t_cylinder	new_cylinder(t_vec3 center, t_vec3 axis, double radius, double height, t_material mat)
{
	t_cylinder	cylinder;

	cylinder.center = center;
	cylinder.axis = vec3_norm(axis);
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.mat = mat;
	return (cylinder);
}

static int	check_cylinder_side(t_ray *ray, t_cylinder *cy, double t, t_hit_record *rec)
{
	t_vec3 p;
	t_vec3 axis;
	t_vec3 v;
	double half_h;
	double proj;
	t_vec3 proj_point;
	t_vec3 outward;

	p = ray_at(ray, t);
	axis = vec3_norm(cy->axis);
	v = vec3_sub(p, cy->center);
	half_h = cy->height / 2.0;
	proj = vec3_dot(v, axis);
	if (proj < -half_h || proj > half_h)
		return (0);
	proj_point = vec3_add(cy->center, vec3_mul_n(axis, proj));
	outward = vec3_norm(vec3_sub(p, proj_point));
	rec->t = t;
	rec->p = p;
	rec->mat = cy->mat;
	set_face_normal(ray, outward, rec);
	return (1);
}

int cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *rec)
{
    t_cylinder *cy = &obj.geo.cylinder;
    t_vec3 oc = vec3_sub(ray->origin, cy->center);
    t_vec3 axis = vec3_norm(cy->axis);
    double a, b, c, disc, sqrt_disc;
    double t1, t2;
    t_hit_record tmp;
    int hit_any = 0;
    double closest_t = ray_t.max;  // Track closest hit instead of modifying ray_t

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
        if (t1 >= ray_t.min && t1 < closest_t)
        {
            if (check_cylinder_side(ray, cy, t1, &tmp))
            {
                *rec = tmp;
                closest_t = tmp.t;
                hit_any = 1;
            }
        }
        if (t2 >= ray_t.min && t2 < closest_t)
        {
            if (check_cylinder_side(ray, cy, t2, &tmp))
            {
                *rec = tmp;
                closest_t = tmp.t;
                hit_any = 1;
            }
        }
    }

    // ==== Check top/bottom caps ====
    t_vec3 half_axis = vec3_mul_n(axis, cy->height / 2.0);
    t_vec3 cap1 = vec3_sub(cy->center, half_axis);
    t_vec3 cap2 = vec3_add(cy->center, half_axis);
    double denom = vec3_dot(ray->direction, axis);

    if (fabs(denom) > 1e-8)
    {
        // bottom cap
        double tcap = vec3_dot(vec3_sub(cap1, ray->origin), axis) / denom;
        if (tcap >= ray_t.min && tcap < closest_t)  // ← Check against closest_t
        {
            t_vec3 p = ray_at(ray, tcap);
            if (vec3_length_squared(vec3_sub(p, cap1)) <= cy->radius * cy->radius)
            {
                rec->t = tcap;
                rec->p = p;
                rec->mat = cy->mat;
                set_face_normal(ray, vec3_mul_n(axis, -1), rec);
                closest_t = tcap;
                hit_any = 1;
            }
        }

        // top cap
        tcap = vec3_dot(vec3_sub(cap2, ray->origin), axis) / denom;
        if (tcap >= ray_t.min && tcap < closest_t)  // ← Check against closest_t
        {
            t_vec3 p = ray_at(ray, tcap);
            if (vec3_length_squared(vec3_sub(p, cap2)) <= cy->radius * cy->radius)
            {
                rec->t = tcap;
                rec->p = p;
                rec->mat = cy->mat;
                set_face_normal(ray, axis, rec);
                closest_t = tcap;
                hit_any = 1;
            }
        }
    }
	if (hit_any)
		rec->hit_obj = &obj;

    return (hit_any);
}
/*

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

static int	check_cylinder_caps(t_ray *ray, t_cylinder *cylinder, double t, t_hit_record *record)
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

int	cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
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
	
	// For infinite cylinder, we solve: ||(o + td) - c - ((o + td - c) · a)a||² = r²
	// This reduces to a quadratic equation in t
	ray_cross_axis = vec3_cross(ray->direction, cylinder->axis);
	oc_cross_axis = vec3_cross(oc, cylinder->axis);
	
	a = vec3_dot(ray_cross_axis, ray_cross_axis);
	b = 2.0 * vec3_dot(ray_cross_axis, oc_cross_axis);
	c = vec3_dot(oc_cross_axis, oc_cross_axis) - (cylinder->radius * cylinder->radius);
	
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

int	cylinder_hit_old(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *rec)
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
				hit_any = 1;
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