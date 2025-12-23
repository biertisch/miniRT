/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:56:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/23 14:58:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	translate(t_slider *slider)
{
	double	value;
	double	delta;

	delta = (slider->knob_pos - 0.5) * RANGE_TR;
	value = slider->base_value + delta;
	slider->base_value = value;
	slider->knob_pos = 0.5;
	return (value);
}

void	apply_translation(t_slider *sliders, t_vec3 *position)
{
	position->x = translate(&sliders[TRANSL_X]);
	position->y = translate(&sliders[TRANSL_Y]);
	position->z = translate(&sliders[TRANSL_Z]);
}
