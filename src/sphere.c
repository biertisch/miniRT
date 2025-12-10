#include "minirt.h"

t_sphere	new_sphere(t_vec3 center, double radius, t_material mat)
{
	t_sphere	sphere;

	sphere.center = center;
	sphere.radius = radius;
	sphere.mat = mat;
	// sphere.bbox = get_aabb(
	// 	vec3_subtract(center, (t_vec3){radius, radius, radius}),
	// 	vec3_add(center, (t_vec3){radius, radius, radius})
	// );
	return (sphere);
}

void	set_face_normal(t_ray *ray, t_vec3 outward_normal, t_hit_record *record)
{
	if (vec3_dot(ray->direction, outward_normal) < 0)
	{
		record->front_face = 1;
		record->normal = outward_normal;
	}
	else
	{
		record->front_face = 0;
		record->normal = vec3_multiply(outward_normal, -1);
	}
}

void get_sphere_uv(t_vec3 p, double *u, double *v)
{
	double	theta;
	double	phi;

	theta = acos(-p.y);
	phi = atan2(-p.z, p.x) + M_PI;
	*u = phi / (2 * M_PI);
	*v = theta / M_PI;
}

int	sphere_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	c;
	double	discriminant;
	t_sphere	*sphere;

	sphere = &obj.geo.sphere;
	oc = vec3_subtract(sphere->center, ray->origin);
	a = vec3_dot(ray->direction, ray->direction);
	h = vec3_dot(oc, ray->direction);
	c = vec3_dot(oc, oc) - (sphere->radius * sphere->radius);
	discriminant = h * h - a * c;
	if (discriminant < 0)
		return (0);
	double	sqrtd = sqrt(discriminant);
	double	root = (h - sqrtd) / a;
	if (!interval_surrounds(&ray_t, root))
	{
		root = (h + sqrtd) / a;
		if (!interval_surrounds(&ray_t, root))
			return (0);
	}
	record->t = root;
	record->p = ray_at(ray, record->t);
	t_vec3 outward_normal = vec3_multiply(vec3_subtract(record->p, sphere->center), 1.0 / sphere->radius);
	set_face_normal(ray, outward_normal, record);
	get_sphere_uv(outward_normal, &record->u, &record->v);
	record->mat = sphere->mat;
	return (1);
}
