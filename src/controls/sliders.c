/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sliders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:38:05 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/15 12:30:09 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	get_object_height(t_object *obj)
{
	if (obj->type == CYLINDER)
		return (obj->geo.cylinder.height);
	else if (obj->type == CONE)
		return (obj->geo.cone.height);
	return (0);
}

static double	get_object_diameter(t_object *obj)
{
	if (obj->type == SPHERE)
		return (obj->geo.sphere.radius * 2);
	else if (obj->type == CYLINDER)
		return (obj->geo.cylinder.radius * 2);
	else if (obj->type == CONE)
		return (obj->geo.cone.radius * 2);
	return (0);
}

static double	get_object_orientation(t_object *obj, int axis)
{
	if (obj->type == PLANE)
		return (((double *)&obj->geo.plane.normal)[axis]);
	else if (obj->type == CYLINDER)
		return (((double *)&obj->geo.cylinder.axis)[axis]);
	else if (obj->type == CONE)
		return (((double *)&obj->geo.cone.axis)[axis]);
	return (0);
}

static double	get_object_position(t_object *obj, int axis)
{
	if (obj->type == PLANE)
		return (((double *)&obj->geo.plane.point)[axis]);
	else if (obj->type == SPHERE)
		return (((double *)&obj->geo.sphere.center)[axis]);
	else if (obj->type == CYLINDER)
		return (((double *)&obj->geo.cylinder.center)[axis]);
	else if (obj->type == CONE)
		return (((double *)&obj->geo.cone.apex)[axis]);
	return (0);
}

double	get_base_value(t_world *scene, int index, int slider)
{
	t_object	*obj;
	int			axis;

	if (index < 0 || index > scene->num_objects + 1) // spot light
		return (0);
	axis = slider_to_axis(slider);
	if (index == 0) // spot light
	{
		if (slider >= TRANSL_X && slider <= TRANSL_Z)
			return (((double *)&scene->spot_light.position)[axis]);
		return (0);
	}
	obj = scene->objects[index - 1];
	if (slider >= TRANSL_X && slider <= TRANSL_Z)
		return (get_object_position(obj, axis));
	else if (slider >= ROTATE_X && slider <= ROTATE_Z)
		return (get_object_orientation(obj, axis));
	else if (slider == RESIZE_D)
		return (get_object_diameter(obj));
	else if (slider == RESIZE_H)
		return (get_object_height(obj));
	return (0);
}
