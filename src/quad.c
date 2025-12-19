#include "minirt.h"

// unnecessary for parser
t_quad	new_quad(t_vec3 Q, t_vec3 u, t_vec3 v, t_material mat)
{
	t_quad	quad;
	t_vec3	n;

	quad.q = Q;
	quad.u = u;
	quad.v = v;
	quad.mat = mat;
	n = vec3_cross(u, v);
	quad.normal = unit_vector(n);
	quad.d = vec3_dot(quad.normal, Q);
	quad.w = vec3_mul_n(n, 1 / vec3_dot(n, n));

	quad.area = vec3_length(n);
	return (quad);
}

int is_interior(double alpha, double beta, t_hit_record *record)
{
	t_interval unit_interval;
	unit_interval = new_interval(0.0, 1.0);
	if (!interval_contains(&unit_interval, alpha) || !interval_contains(&unit_interval, beta))
		return (0);
	record->u = alpha;
	record->v = beta;
	return (1);
}

int	quad_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	double	denom;
	double	t;
	double	alpha;
	double	beta;
	t_vec3	intersection;
	t_vec3	planar_hitpt_vec;

	denom = vec3_dot(obj.geo.quad.normal, ray->direction);
	if (fabs(denom) < 1e-8)
		return (0);
	t = (obj.geo.quad.d - vec3_dot(obj.geo.quad.normal, ray->origin)) / denom;
	if (!interval_contains(&ray_t, t))
		return (0);
	intersection = ray_at(ray, t);
	planar_hitpt_vec = vec3_sub(intersection, obj.geo.quad.q);
	alpha = vec3_dot(obj.geo.quad.w, vec3_cross(planar_hitpt_vec, obj.geo.quad.v));
	beta = vec3_dot(obj.geo.quad.w, vec3_cross(obj.geo.quad.u, planar_hitpt_vec));
	if (!is_interior(alpha, beta, record))
		return (0);
	record->t = t;
	record->p = intersection;
	record->mat = obj.geo.quad.mat;
	set_face_normal(ray, obj.geo.quad.normal, record);
	return (1);
}

/*

t_vec3	quad_random(t_object obj, t_vec3 origin)
{
	t_vec3	p;

	p = vec3_add(obj.geo.quad.Q,
			vec3_add(
				vec3_mul_n(obj.geo.quad.u, random_double()),
				vec3_mul_n(obj.geo.quad.v, random_double())
			)
		);
	return (vec3_sub(p, origin));
}

double	quad_pdf_value(t_object obj, t_vec3 origin, t_vec3 direction)
{
	t_hit_record	rec;
	t_ray			r;
	t_quad			*quad;
	double			distance_squared;
	double			cosine;

	r = rt_ray(origin, direction);
	if (!quad_hit(&r, new_interval(0.001, RT_INFINITY), obj, &rec))
		return (0.0);
	distance_squared = vec3_length_squared(direction) * rec.t * rec.t;
	cosine = fabs(vec3_dot(direction, rec.normal) / vec3_length(direction));

	return (distance_squared / (cosine * obj.geo.quad.area));
}
*/