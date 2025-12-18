/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:56:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 13:06:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	translate(t_slider *slider, double dir)
{
	double	value;
	double	delta;

	delta = (slider->knob_pos - 0.5) * RANGE_TR * dir;
	value = slider->base_value + delta;
	slider->base_value = value;
	slider->knob_pos = 0.5;
	return (value);
}

void	apply_translation(t_slider *sliders, t_vec3 *position)
{
	position->x = translate(&sliders[TRANSL_X], -1);
	position->y = translate(&sliders[TRANSL_Y], 1);
	position->z = translate(&sliders[TRANSL_Z], 1);
}
