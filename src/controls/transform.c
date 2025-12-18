/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:06 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 13:05:39 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	transform_object(t_slider *sliders, t_object *object)
{
	if (object->type == PLANE)
	{
		apply_translation(sliders, &object->geo.plane.point);
		apply_rotation(sliders, &object->geo.plane.normal);
	}
	else if (object->type == SPHERE)
	{
		apply_translation(sliders, &object->geo.sphere.center);
		apply_resize(sliders, &object->geo.sphere.radius, NULL);
	}
	else if (object->type == CYLINDER)
	{
		apply_translation(sliders, &object->geo.cylinder.center);
		apply_rotation(sliders, &object->geo.cylinder.axis);
		apply_resize(sliders, &object->geo.cylinder.radius,
			&object->geo.cylinder.height);
	}
	else if (object->type == CONE)
	{
		apply_translation(sliders, &object->geo.cone.apex);
		apply_rotation(sliders, &object->geo.cone.axis);
		apply_resize(sliders, &object->geo.cone.radius,
			&object->geo.cone.height);
	}
}

void	transform_scene(t_panel *panel, t_world *scene)
{
	int	i;

	apply_translation(panel->sliders[0], &scene->spot_light.position);
	i = 0;
	while (i < scene->num_objects)
	{
		transform_object(panel->sliders[i + 1], scene->objects[i]); // light count
		i++;
	}
	camera_render(&scene->camera, scene);
}

void	check_button(t_world *scene, int x)
{
	int	b1_x;
	int	b2_x;

	b1_x = (scene->panel->width / 2 - BUTTON_W) / 2;
	b2_x = b1_x + scene->panel->width / 2;
	if (x >= b1_x && x < b1_x + BUTTON_W)
		transform_scene(scene->panel, scene);
	else if (x >= b2_x && x < b2_x + BUTTON_W)
		reset_scene(scene->panel, scene);
}
