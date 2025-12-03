#include "minirt.h"

t_plane	new_plane(t_vec3 point, t_vec3 normal, t_material mat)
{
	t_plane	plane;

	plane.point = point;
	plane.normal = vec3_normalize(normal);
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
	
	// Calculate denominator (ray direction dot plane normal)
	denom = vec3_dot(ray->direction, plane->normal);
	
	// If denominator is close to zero, ray is parallel to plane
	if (fabs(denom) < 1e-8)
		return (0);
	
	// Calculate t parameter for intersection
	t = vec3_dot(vec3_subtract(plane->point, ray->origin), plane->normal) / denom;
	
	// Check if intersection is within the ray interval
	if (!interval_surrounds(&ray_t, t))
		return (0);
	
	// Fill hit record
	record->t = t;
	record->p = ray_at(ray, t);
	
	// Determine which side of the plane we're hitting
	outward_normal = plane->normal;
	set_face_normal(ray, outward_normal, record);
	record->mat = plane->mat;
	
	return (1);
}
