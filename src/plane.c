#include "minirt.h"

t_plane	new_plane(t_vec3 point, t_vec3 normal, t_material mat)
{
	t_plane	plane;

	plane.point = point;
	plane.normal = vec3_norm(normal);
	plane.mat = mat;
	return (plane);
}

int	plane_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_plane		*plane;
	double		denom;
	double		t;
	t_vec3		outward_normal;

	plane = &obj.geo.plane;
	denom = vec3_dot(ray->direction, plane->normal);
	if (fabs(denom) < 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(plane->point, ray->origin), plane->normal) / denom;
	if (!interval_surrounds(&ray_t, t))
		return (0);
	record->t = t;
	record->p = ray_at(ray, t);
	outward_normal = plane->normal;
	set_face_normal(ray, outward_normal, record);
	record->mat = plane->mat;
	record->hit_obj = &obj;
	return (1);
}
