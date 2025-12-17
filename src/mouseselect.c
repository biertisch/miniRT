/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouseselect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 12:09:14 by bliu              #+#    #+#             */
/*   Updated: 2025/12/17 17:10:56 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	has_object_was_clicked(t_world *world, t_ray *ray,
	t_interval ray_t, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;
	int				i;

	hit_anything = 0;
	closest_so_far = ray_t.max;
	i = 0;
	while (i < world->num_objects)
	{
		current = world->objects[i];
		ray_t.max = closest_so_far;
		if (current->hit && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			world->current_obj = current;
		}
		i++;
	}
	return (hit_anything);
}

t_ray	get_ray_with_screenxy(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	pixel_sample = vec3_add(camera->pix00_loc,
			vec3_add(vec3_mul_n(camera->pix_delta_u, pixel_x),
				vec3_mul_n(camera->pix_delta_v, pixel_y)));
	ray_direction = vec3_sub(pixel_sample, camera->lookfrom);
	return (rt_ray(camera->lookfrom, ray_direction));
}

int	mouse_press(int button, int x, int y, void *param)
{
	t_world			*wld;
	t_ray			ray;
	t_hit_record	rec;

	wld = (t_world *)param;
	ray = get_ray_with_screenxy(x, y, &wld->camera);
	if (has_object_was_clicked(wld, &ray, (t_interval){0.001, INFINITY}, &rec))
	{
		if (wld->current_obj)
			printf("hit ojb type:%d\n", wld->current_obj->type);
		else
			printf("hit obj is NULL\n");
	}
	return (0);
}
