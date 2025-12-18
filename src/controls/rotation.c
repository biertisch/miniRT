/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:55:23 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 21:50:50 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static t_vec3	rotate_z(t_vec3 base, double a)
{
	double	c;
	double	s;

	c = cos(a);
	s = sin(a);
	return ((t_vec3){
		base.x * c - base.y * s,
		base.x * s + base.y * c,
		base.z
	});
}

static t_vec3	rotate_y(t_vec3 base, double a)
{
	double	c;
	double	s;

	c = cos(a);
	s = sin(a);
	return ((t_vec3){
		base.x * c + base.z * s,
		base.y,
		-base.x * s + base.z * c
	});
}

static t_vec3	rotate_x(t_vec3 base, double a)
{
	double	c;
	double	s;

	c = cos(a);
	s = sin(a);
	return ((t_vec3){
		base.x,
		base.y * c - base.z * s,
		base.y * s + base.z * c
	});
}

// base_value is initialized to 0 for rotations
static void	commit_rotation(t_slider *slider)
{
	double	delta;

	delta = degrees_to_radians(-(slider->knob_pos - 0.5) * RANGE_RT);
	slider->base_value += delta;
	slider->knob_pos = 0.5;
}

void	apply_rotation(t_slider *sliders, t_vec3 *direction)
{
	t_vec3	tmp;
	int		i;

	tmp.x = sliders[ROTATE_X].initial_value;
	tmp.y = sliders[ROTATE_Y].initial_value;
	tmp.z = sliders[ROTATE_Z].initial_value;
	i = ROTATE_X;
	while (i <= ROTATE_Z)
		commit_rotation(&sliders[i++]);
	tmp = rotate_x(tmp, sliders[ROTATE_X].base_value);
	tmp = rotate_y(tmp, sliders[ROTATE_Y].base_value);
	tmp = rotate_z(tmp, sliders[ROTATE_Z].base_value);
	*direction = vec3_normalize(tmp);
}
