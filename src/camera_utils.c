/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:13:48 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 16:28:00 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	attenuation(double d)
{
	double	constant;
	double	linear;
	double	quadratic;

	constant = 1.0;
	linear = 0.1;
	quadratic = 0.01;
	return (1.0 / (constant + linear * d + quadratic * d * d));
}

t_ray	get_ray(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_point;
	t_vec3	ray_direction;

	pixel_point = vec3_add(camera->pix00_loc,
			vec3_add(vec3_mul_n(camera->pix_delta_u, pixel_x),
				vec3_mul_n(camera->pix_delta_v, pixel_y)));
	ray_direction = vec3_norm(vec3_sub(pixel_point, camera->lookfrom));
	return (rt_ray(camera->lookfrom, ray_direction));
}

t_vec3	choose_vup(t_vec3 forward)
{
	t_vec3	vup;

	vup = (t_vec3){0.0, 1.0, 0.0};
	if (fabs(vec3_dot(forward, vup)) >= 0.999)
		vup = (t_vec3){0.0, 0.0, 1.0};
	return (vup);
}

t_color	metal_reflection_color(t_phong *phong, t_hit_record *rec,
			int depth, t_world *world)
{
	t_color	reflected_color;
	t_color	final_color;
	t_vec3	c_i;
	t_vec3	c_r;
	t_ray	reflect_ray;

	reflected_color = get_color(0, 0, 0);
	final_color = color_add(phong->ambient, phong->specular);
	c_i = vec3_norm(rec->ray_in.direction);
	c_r = vec3_sub(c_i, vec3_mul_n(rec->normal, 2.0
				* vec3_dot(c_i, rec->normal)));
	reflect_ray.origin = vec3_add(rec->p, vec3_mul_n(rec->normal, 1e-4));
	reflect_ray.direction = vec3_norm(c_r);
	reflected_color = ray_color_v3(&reflect_ray, depth - 1, world);
	final_color = blend_colors(final_color, reflected_color, 0.8);
	return (final_color);
}

int	in_shadow(t_vec3 point, t_world *world, t_vec3 light_pos)
{
	t_vec3			to_light;
	t_ray			shadow_r;
	t_hit_record	temp_rec;
	double			distance_to_light;

	to_light = vec3_sub(light_pos, point);
	distance_to_light = vec3_length(to_light);
	shadow_r.direction = vec3_norm(to_light);
	shadow_r.origin = vec3_add(point, vec3_mul_n(shadow_r.direction, 1e-4));
	if (world_hit(world, &shadow_r, new_interval(0.001,
				distance_to_light - 1e-4), &temp_rec))
		return (1);
	return (0);
}
