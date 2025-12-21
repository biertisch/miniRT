/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surfacechecker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:09:27 by bliu              #+#    #+#             */
/*   Updated: 2025/12/20 18:33:40 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	point_on_cone(t_vec3 p, t_cone *c)
{
	t_vec3	ap;
	double	proj;
	double	dist_to_axis;
	double	radius_at_p;
	int		on_surface;

	on_surface = 0;
	ap = vec3_sub(p, c->apex);
	proj = vec3_dot(ap, c->axis);
	if (proj < 0 || proj > c->height)
		on_surface = 0;
	radius_at_p = (proj / c->height) * c->radius;
	dist_to_axis = vec3_length(
			vec3_sub(ap, vec3_mul_n(c->axis, proj)));
	if (fabs(dist_to_axis - radius_at_p) < SURFACE_EPS)
		on_surface = 1;
	if (fabs(proj - c->height) < SURFACE_EPS
		&& dist_to_axis <= c->radius + SURFACE_EPS)
		on_surface = 1;
	if (on_surface)
		printf("Camera on cone surface\n");
	return (on_surface);
}

int	point_on_cylinder(t_vec3 p, t_cylinder *c)
{
	t_vec3	cp;
	double	proj;
	double	rad;
	int		on_surface;

	on_surface = 0;
	cp = vec3_sub(p, c->center);
	proj = vec3_dot(cp, c->axis);
	if (proj < -c->height * 0.5 - SURFACE_EPS
		|| proj > c->height * 0.5 + SURFACE_EPS)
		return (0);
	rad = vec3_length(vec3_sub(cp, vec3_mul_n(c->axis, proj)));
	if (proj > -c->height * 0.5 + SURFACE_EPS && proj < c->height * 0.5
			- SURFACE_EPS && fabs(rad - c->radius) < SURFACE_EPS)
		on_surface = 1;
	if ((fabs(proj + c->height * 0.5) <= SURFACE_EPS || fabs(proj -
			c->height * 0.5) <= SURFACE_EPS) && rad <= c->radius + SURFACE_EPS)
		on_surface = 1;
	if (on_surface)
		printf("Camera on cylinder surface\n");
	return (on_surface);
}

int	point_on_sphere(t_vec3 p, t_sphere *s)
{
	double	dist;

	dist = vec3_length(vec3_sub(p, s->center));
	if (fabs(dist - s->radius) < SURFACE_EPS)
		printf("Camera on sphere surface\n");
	return (fabs(dist - s->radius) < SURFACE_EPS);
}

int	camera_on_object_surface(t_vec3 p, t_object *obj)
{
	double	d;

	if (obj->type == PLANE)
	{
		d = vec3_dot(vec3_sub(p, obj->geo.plane.point), obj->geo.plane.normal);
		if (fabs(d) < SURFACE_EPS)
			printf("Camera on plane surface\n");
		return (fabs(d) < SURFACE_EPS);
	}
	if (obj->type == SPHERE)
		return (point_on_sphere(p, &obj->geo.sphere));
	if (obj->type == CYLINDER)
		return (point_on_cylinder(p, &obj->geo.cylinder));
	if (obj->type == CONE)
		return (point_on_cone(p, &obj->geo.cone));
	return (0);
}

int	is_camera_on_surface(t_camera *cam, t_world *world)
{
	int	i;

	i = 0;
	while (i < world->num_objects)
	{
		if (camera_on_object_surface(cam->lookfrom, world->objects[i]))
			return (1);
		i++;
	}
	return (0);
}
