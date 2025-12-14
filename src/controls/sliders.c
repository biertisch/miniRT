/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sliders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:38:05 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 16:15:02 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	get_object_value(t_object *obj, int slider)
{
	t_vec3	*pos;
	int		axis;

	axis = slider_to_axis(slider);
	if (axis != -1)
	{
		pos = get_object_position(obj);
		if (!pos)
			return (0);
		return (((double *)pos)[axis]);
	}
	if (slider == RESIZE_D)
	{
		if (obj->type == SPHERE)
			return (obj->geo.sphere.radius * 2);
		if (obj->type == CYLINDER)
			return (obj->geo.cylinder.radius * 2);
	}
	if (slider == RESIZE_H && obj->type == CYLINDER)
		return (obj->geo.cylinder.height);
	return (0);
}

static double	get_light_value(t_world *scene, int slider)
{
	int	axis;

	axis = slider_to_axis(slider);
	if (axis == -1)
		return (0);
	return (0); // update when light struct is updated
}

static double	get_camera_value(t_world *scene, int slider)
{
	int	axis;

	axis = slider_to_axis(slider);
	if (axis == -1)
		return (0);
	return (((double *)&scene->camera.lookfrom)[axis]);
}

double	get_base_value(t_world *scene, int index, int slider)
{
	if (index < 0)
		return (0);
	if (index == 0)
		return (get_camera_value(scene, slider));
	if (index == 1)
		return round(get_light_value(scene, slider));
	return (get_object_value(scene->objects[index - 2], slider));
	return (0);
}
