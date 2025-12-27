/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:48:08 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 11:00:46 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	calculate_specular(t_phong *phong, t_ray p2l, t_hit_record *rec,
	t_s_light light)
{
	t_vec3			reflect_dir;

	reflect_dir = vec3_sub(vec3_mul_n(rec->normal, 2.0
				* vec3_dot(p2l.direction, rec->normal)), p2l.direction);
	min_vl_clamp(&phong->cos_rv, vec3_dot(vec3_norm(reflect_dir),
			vec3_mul_n(rec->ray_in.direction, -1)), 0.0);
	phong->specular = color_multi_num(light.color, pow(phong->cos_rv,
				SPECULAR_FACTOR) * phong->atn * light.brightness * 2);
}

t_color	phong_of_light(t_phong *phong, t_hit_record *rec,
			t_world *world, t_s_light light)
{
	t_ray	p2l;

	if (in_shadow(rec->p, world, light.position))
		return (color(0.0, 0.0, 0.0));
	if (DEBUG)
		return (normal_to_color(rec->g_norm));
	rec->normal = rec->g_norm;
	if (vec3_dot(vec3_mul_n(rec->ray_in.direction, -1), rec->g_norm) < 0)
		rec->normal = vec3_mul_n(rec->g_norm, -1);
	p2l = ray(rec->p, vec3_sub(light.position, rec->p));
	phong->atn = attenuation(vec3_length(vec3_sub(light.position, rec->p)));
	min_vl_clamp(&phong->cos_nl, vec3_dot(rec->normal, p2l.direction), 0.0);
	phong->bright = light.brightness * phong->cos_nl;
	phong->diffuse = color_multi_num(color_mult_color(rec->orig_color,
				light.color), phong->bright * phong->atn * 4);
	phong->specular = color(0.0, 0.0, 0.0);
	if (phong->cos_nl > 0.0)
		calculate_specular(phong, p2l, rec, light);
	return (color_add(phong->diffuse, phong->specular));
}
