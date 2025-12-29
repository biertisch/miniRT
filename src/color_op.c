/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:08:44 by bliu              #+#    #+#             */
/*   Updated: 2025/12/26 17:08:52 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	color_add(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r + b.r;
	result.g = a.g + b.g;
	result.b = a.b + b.b;
	return (result);
}

t_color	color_multi_num(t_color color, double scalar)
{
	t_color	result;

	result.r = color.r * scalar;
	result.g = color.g * scalar;
	result.b = color.b * scalar;
	return (result);
}

t_color	color_mult_color(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r * b.r;
	result.g = a.g * b.g;
	result.b = a.b * b.b;
	return (result);
}

t_color	color_clamp(t_color v, double min, double max)
{
	if (v.r < min)
		v.r = min;
	if (v.r > max)
		v.r = max;
	if (v.g < min)
		v.g = min;
	if (v.g > max)
		v.g = max;
	if (v.b < min)
		v.b = min;
	if (v.b > max)
		v.b = max;
	return (v);
}

t_color	blend_colors(t_color c1, t_color c2, double t)
{
	t_color	blended;

	blended.r = (1 - t) * c1.r + t * c2.r;
	blended.g = (1 - t) * c1.g + t * c2.g;
	blended.b = (1 - t) * c1.b + t * c2.b;
	return (blended);
}
