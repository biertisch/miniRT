/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:57:15 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 13:06:30 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	resize(t_slider *slider)
{
	double	value;
	double	ratio;

	if (slider->knob_pos <= 0.5)
		ratio = slider->knob_pos + 0.5;
	else
		ratio = slider->knob_pos * 2;
	value = slider->base_value * ratio;
	if (value < 0.0001)
		value = 0.0001;
	else if (value > 1e6)
		value = 1e6;
	slider->base_value = value;
	slider->knob_pos = 0.5;
	return (value);
}

void	apply_resize(t_slider *sliders, double *radius, double *height)
{
	if (radius)
		*radius = resize(&sliders[RESIZE_D]) / 2;
	if (height)
		*height = resize(&sliders[RESIZE_H]);
}
