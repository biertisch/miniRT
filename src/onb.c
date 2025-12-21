/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   onb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:50:38 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 23:37:52 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_onb	onb_new(t_vec3 n)
{
	t_onb	onb;
	t_vec3	a;

	onb.axis[2] = unit_vector(n);
	if (fabs(onb.axis[2].x) > 0.9)
		a = (t_vec3){0, 1, 0};
	else
		a = (t_vec3){1, 0, 0};
	onb.axis[1] = unit_vector(vec3_cross(onb.axis[2], a));
	onb.axis[0] = vec3_cross(onb.axis[2], onb.axis[1]);
	return (onb);
}

t_vec3	onb_transform(t_onb onb, t_vec3 v)
{
	t_vec3	result;

	result = vec3_mul_n(onb.axis[0], v.x);
	result = vec3_add(result, vec3_mul_n(onb.axis[1], v.y));
	result = vec3_add(result, vec3_mul_n(onb.axis[2], v.z));
	return (result);
}

t_vec3	onb_u(t_onb onb)
{
	return (onb.axis[0]);
}

t_vec3	onb_v(t_onb onb)
{
	return (onb.axis[1]);
}

t_vec3	onb_w(t_onb onb)
{
	return (onb.axis[2]);
}
