#include "minirt.h"

void pad_to_mininums(t_aabb *aabb)
{
	double	epsilon;

	epsilon = 0.0001;
	if (interval_size(&aabb->x) < epsilon)
		aabb->x = interval_expand(&aabb->x, epsilon);
	if (interval_size(&aabb->y) < epsilon)
		aabb->y = interval_expand(&aabb->y, epsilon);
	if (interval_size(&aabb->z) < epsilon)
		aabb->z = interval_expand(&aabb->z, epsilon);
}

t_aabb	get_aabb(t_vec3 a, t_vec3 b)
{
	t_aabb	aabb;

	if (a.x <= b.x)
		aabb.x = new_interval(a.x, b.x);
	else
		aabb.x = new_interval(b.x, a.x);
	if (a.y <= b.y)
		aabb.y = new_interval(a.y, b.y);
	else
		aabb.y = new_interval(b.y, a.y);
	if (a.z <= b.z)
		aabb.z = new_interval(a.z, b.z);
	else
		aabb.z = new_interval(b.z, a.z);
	pad_to_mininums(&aabb);
	return (aabb);
}

t_aabb	get_aabb_surrounding(t_aabb *a, t_aabb *b)
{
	t_aabb	surrounding;

	surrounding.x = interval_union(&a->x, &b->x);
	surrounding.y = interval_union(&a->y, &b->y);
	surrounding.z = interval_union(&a->z, &b->z);
	return (surrounding);
}

t_interval	aabb_axis_interval(t_aabb *aabb, int axis_index)
{
	if (axis_index == 1)
		return (aabb->y);
	else if (axis_index == 2)
		return (aabb->z);
	else
		return (aabb->x);
}

double	vec3_axis_origin(t_vec3 v, int axis_index)
{
	if (axis_index == 1)
		return (v.y);
	else if (axis_index == 2)
		return (v.z);
	else
		return (v.x);
}

int	aabb_hit(t_aabb *aabb, t_ray *r, t_interval ray_t)
{
	int		axis;
	t_interval	ax;
	double	adinv;
	double	t0;
	double	t1;

	axis = 0;
	while (axis < 3)
	{
		ax = aabb_axis_interval(aabb, axis);
		adinv = 1.0 / vec3_axis_origin(r->direction, axis);
		t0 = (ax.min - vec3_axis_origin(r->origin, axis)) * adinv;
		t1 = (ax.max - vec3_axis_origin(r->origin, axis)) * adinv;		
		if (t0 < t1)
		{
			ray_t.min = fmax(t0, ray_t.min);
			ray_t.max = fmin(t1, ray_t.max);
		}
		else
		{
			ray_t.min = fmax(t1, ray_t.min);
			ray_t.max = fmin(t0, ray_t.max);
		}
		if (ray_t.max <= ray_t.min)
			return (0);
		axis++;
	}
	return (1);
}

int	aabb_longest_axis(t_aabb *aabb)
{
	if (interval_size(&aabb->x) > interval_size(&aabb->y))
	{
		if (interval_size(&aabb->x) > interval_size(&aabb->z))
			return (0);
		else
			return (2);
	}
	else
	{
		if (interval_size(&aabb->y) > interval_size(&aabb->z))
			return (1);
		else
			return (2);
	}
}

t_aabb	aabb_empty(void)
{
	t_aabb	aabb;

	aabb.x = new_interval(RT_INFINITY, -RT_INFINITY);
	aabb.y = new_interval(RT_INFINITY, -RT_INFINITY);
	aabb.z = new_interval(RT_INFINITY, -RT_INFINITY);
	return (aabb);
}

t_aabb	aabb_universal(void)
{
	t_aabb	aabb;

	aabb.x = new_interval(-RT_INFINITY, RT_INFINITY);
	aabb.y = new_interval(-RT_INFINITY, RT_INFINITY);
	aabb.z = new_interval(-RT_INFINITY, RT_INFINITY);
	return (aabb);
}
