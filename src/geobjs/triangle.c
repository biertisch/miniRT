#include "minirt.h"

int	triangle_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_triangle	*t;
	t_vec3		e1;
	t_vec3		e2;
	t_vec3		h;
	t_vec3		s;
	double		a;
	double		f;
	double		u;
	double		v;
	double		t_temp;

	t = &obj.geo.triangle;
	e1 = vec3_sub(t->p2, t->p1);
	e2 = vec3_sub(t->p3, t->p1);
	h = vec3_cross(ray->direction, e2);
	a = vec3_dot(e1, h);
	if (fabs(a) < 1e-6)
		return (0);
	f = 1.0 / a;
	s = vec3_sub(ray->origin, t->p1);
	u = f * vec3_dot(s, h);
	if (u < 0.0 || u > 1.0)
		return (0);
	v = f * vec3_dot(ray->direction, vec3_cross(s, e1));
	if (v < 0.0 || u + v > 1.0)
		return (0);
	t_temp = f * vec3_dot(e2, vec3_cross(s, e1));
	if (t_temp < ray_t.min || t_temp > ray_t.max)
		return (0);
	record->t = t_temp;
	record->p = ray_at(ray, record->t);
	record->g_norm = t->normal;
	record->is_d_side = 0;
	record->u = u;
	record->v = v;
	record->mat = t->mat;
	set_face_normal(ray, t->normal, record);
	return (1);
}
