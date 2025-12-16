/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:55:23 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/16 11:55:46 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	rotate_z(t_vec3 *result, t_vec3 base, double c, double s)
{
	result->x = base.x * c - base.y * s;
	result->y = base.x * s + base.y * c;
	result->z = base.z;
}

static void	rotate_y(t_vec3 *result, t_vec3 base, double c, double s)
{
	result->x = base.x * c + base.z *s;
	result->y = base.y;
	result->z = -base.x * s + base.z * c;
}

static void	rotate_x(t_vec3 *result, t_vec3 base, double c, double s)
{
	result->x = base.x;
	result->y = base.y * c - base.z * s;
	result->z = base.y * s + base.z * c;
}

static t_vec3	rotate(t_slider *slider, t_vec3 base)
{
	t_vec3	result;
	double	angle;
	double	c;
	double	s;

	angle = (slider->knob_pos - 0.5) * RANGE_RT;
	c = cos(angle);
	s = sin(angle);
	if (slider->type == ROTATE_X)
		rotate_x(&result, base, c, s);
	else if (slider->type == ROTATE_Y)
		rotate_y(&result, base, c, s);
	else if (slider->type == ROTATE_Z)
		rotate_z(&result, base, c, s);
	slider->knob_pos = 0.5;
	result = vec3_normalize(result);
	return (result);
}

void	apply_rotation(t_slider *sliders, t_vec3 *direction)
{
	*direction = rotate(&sliders[ROTATE_X], *direction);
	*direction = rotate(&sliders[ROTATE_Y], *direction);
	*direction = rotate(&sliders[ROTATE_Z], *direction);
}
