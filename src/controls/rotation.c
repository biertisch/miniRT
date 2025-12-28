/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:55:23 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/28 12:29:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static t_mat3	apply_rotation_x(t_mat3 base, double a)
{
	double	c;
	double	s;

	if (a == 0)
		return (base);
	c = cos(a);
	s = sin(a);
	return (mat3_mult((t_mat3){
			.m = {{1, 0, 0}, {0, c, -s}, {0, s, c}}
		}, base));
}

static t_mat3	apply_rotation_y(t_mat3 base, double a)
{
	double	c;
	double	s;

	if (a == 0)
		return (base);
	c = cos(a);
	s = sin(a);
	return (mat3_mult((t_mat3){
			.m = {{c, 0, s}, {0, 1, 0}, {-s, 0, c}}
		}, base));
}

static t_mat3	apply_rotation_z(t_mat3 base, double a)
{
	double	c;
	double	s;

	if (a == 0)
		return (base);
	c = cos(a);
	s = sin(a);
	return (mat3_mult((t_mat3){
			.m = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}}
		}, base));
}

static double	commit_rotation(t_slider *slider)
{
	double	delta;

	delta = -(slider->knob_pos - 0.5) * RANGE_RT * M_PI / 180;
	slider->base_value = delta;
	slider->knob_pos = 0.5;
	return (delta);
}

void	apply_rotation(t_slider *s, t_vec3 *dir)
{
	t_mat3	*rot;

	rot = &s[ROTATE_X].rotation;
	if (commit_rotation(&s[ROTATE_X]) != 0)
		*rot = apply_rotation_x(*rot, s[ROTATE_X].base_value);
	if (commit_rotation(&s[ROTATE_Y]) != 0)
		*rot = apply_rotation_y(*rot, s[ROTATE_Y].base_value);
	if (commit_rotation(&s[ROTATE_Z]) != 0)
		*rot = apply_rotation_z(*rot, s[ROTATE_Z].base_value);
	*dir = vec3_norm(
			mat3_mult_vec3(*rot, (t_vec3){
				s[ROTATE_X].initial_value,
				s[ROTATE_Y].initial_value,
				s[ROTATE_Z].initial_value
			}));
}

// Version 2 - with incremental changes instead of matrices
// static t_vec3	rotate_z(t_vec3 base, double a)
// {
// 	double	c;
// 	double	s;

// 	c = cos(a);
// 	s = sin(a);
// 	return ((t_vec3){
// 		base.x * c - base.y * s,
// 		base.x * s + base.y * c,
// 		base.z
// 	});
// }

// static t_vec3	rotate_y(t_vec3 base, double a)
// {
// 	double	c;
// 	double	s;

// 	c = cos(a);
// 	s = sin(a);
// 	return ((t_vec3){
// 		base.x * c + base.z * s,
// 		base.y,
// 		-base.x * s + base.z * c
// 	});
// }

// static t_vec3	rotate_x(t_vec3 base, double a)
// {
// 	double	c;
// 	double	s;

// 	c = cos(a);
// 	s = sin(a);
// 	return ((t_vec3){
// 		base.x,
// 		base.y * c - base.z * s,
// 		base.y * s + base.z * c
// 	});
// }

// void	apply_rotation(t_slider *sliders, t_vec3 *direction)
// {
// 	t_vec3	tmp;
// 	int		i;

// 	tmp = *direction;
// 	i = ROTATE_X;
// 	while (i <= ROTATE_Z)
// 		commit_rotation(&sliders[i++]);
// 	tmp = rotate_x(tmp, sliders[ROTATE_X].base_value);
// 	tmp = rotate_y(tmp, sliders[ROTATE_Y].base_value);
// 	tmp = rotate_z(tmp, sliders[ROTATE_Z].base_value);
// 	*direction = vec3_norm(tmp);
// }