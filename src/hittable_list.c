/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:42:05 by bliu              #+#    #+#             */
/*   Updated: 2026/01/02 00:26:48 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	assign_hit(t_object *new_object)
{
	if (new_object->type == SPHERE)
		new_object->hit = sphere_hit;
	else if (new_object->type == PLANE)
		new_object->hit = plane_hit;
	else if (new_object->type == CYLINDER)
		new_object->hit = cylinder_hit;
	else if (new_object->type == QUAD)
		new_object->hit = quad_hit;
	else if (new_object->type == CONE)
		new_object->hit = cone_hit;
	else if (new_object->type == TRIANGLE)
		new_object->hit = triangle_hit;
	else
		return (0);
	return (1);
}

static int	assign_geometry(t_object *new_object, void *geo)
{
	if (new_object->type == SPHERE)
		new_object->geo.sphere = *(t_sphere *)geo;
	else if (new_object->type == PLANE)
		new_object->geo.plane = *(t_plane *)geo;
	else if (new_object->type == CYLINDER)
		new_object->geo.cylinder = *(t_cylinder *)geo;
	else if (new_object->type == QUAD)
		new_object->geo.quad = *(t_quad *)geo;
	else if (new_object->type == CONE)
		new_object->geo.cone = *(t_cone *)geo;
	else if (new_object->type == TRIANGLE)
		new_object->geo.triangle = *(t_triangle *)geo;
	else
		return (0);
	return (1);
}

void	add_object_to_world(t_world *world, t_geo_type geo_type, void *geo)
{
	t_object	*new_object;

	if (world->num_objects >= MAX_OBJS)
	{
		ft_putstr_fd("Error\nObject limit reached\n", STDERR_FILENO);
		return ;
	}
	new_object = malloc(sizeof(t_object));
	if (!new_object)
		error_exit(world, "Error\nmalloc");
	new_object->type = geo_type;
	new_object->tex_type = world->tex_type;
	if (!assign_geometry(new_object, geo) || !assign_hit(new_object))
	{
		free(new_object);
		ft_putstr_fd("Error\nUnsupport geometry type\n", STDERR_FILENO);
		return ;
	}
	world->objects[world->num_objects] = new_object;
	world->num_objects++;
}
double	get_time_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((double)ts.tv_sec * 1000.0
		+ (double)ts.tv_nsec / 1e6);
}


int	world_hit(t_world *world, t_ray *ray, t_interval ray_t, t_hit_record *rec)
{
	// t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	// t_object		*current;
	// int				i;

	*rec = (t_hit_record){0};
	hit_anything = 0;
	closest_so_far = ray_t.max;
	// i = 0;
	t_triangle	*hit_tri = NULL;
	double		hit_t = closest_so_far;
	if (world->bvh_triangle_root)
		hit_tri = hit_bvh(world->bvh_triangle_root, ray, ray_t.min, ray_t.max, &hit_t);
	if (hit_tri)
	{
		if (triangle_hit2(ray, (t_interval){ray_t.min, closest_so_far},
			hit_tri, rec))
		{
			hit_anything = 1;
			closest_so_far = rec->t;
		}
	}
	return (hit_anything);
}

int	world_hit_old(t_world *world, t_ray *ray, t_interval ray_t, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;
	int				i;

	*rec = (t_hit_record){0};
	hit_anything = 0;
	closest_so_far = ray_t.max;
	i = 0;
	while (i < world->num_objects)
	{
		current = world->objects[i];
		ray_t.max = closest_so_far;
		temp_rec = (t_hit_record){0};
		if (current->hit && current->type != TRIANGLE && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		if (world->bvh_triangle_root)
		{
			double hit_t = closest_so_far;
			double start_time = get_time_ms();
			t_triangle *hit_tri = hit_bvh(world->bvh_triangle_root, ray, ray_t.min, ray_t.max, &hit_t);
			double end_time = get_time_ms();
			printf("BVH hit time: %.3f ms;ray origin:(%.2f, %.2f, %.2f) direction:(%.2f, %.2f, %.2f)\n", end_time - start_time,ray->origin.x,ray->origin.y,ray->origin.z,ray->direction.x,ray->direction.y,ray->direction.z);
			if (hit_tri)
			{
				hit_anything = 1;
				closest_so_far = hit_t;
				triangle_hit2(ray, (t_interval){ray_t.min, closest_so_far},
					hit_tri, rec);
			}
		}
		i++;
	}
	return (hit_anything);
}
/*

double	default_hitable_pdf_value(t_pdf *self, t_vec3 origin, t_vec3 direction)
{
	(void)self;
	(void)direction;
	(void)origin;
	return (0.0);
}

t_vec3	default_hitable_pdf_generate(t_pdf *self, t_vec3 origin)
{
	(void)self;
	(void)origin;
	return (new_vec3(1, 0, 0));
}

int	world_hit_old(t_object *objects, t_ray *ray, t_interval ray_t,
	t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;

	hit_anything = 0;
	closest_so_far = ray_t.max;
	current = objects;
	while (current != NULL)
	{
		ray_t.max = closest_so_far;

		if (current->hit && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		current = current->next;
	}
	return (hit_anything);
}*/