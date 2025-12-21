/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:14:25 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 15:41:50 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	reset_direction(t_slider *sliders, t_vec3 *direction)
{
	direction->x = sliders[ROTATE_X].initial_value;
	direction->y = sliders[ROTATE_Y].initial_value;
	direction->z = sliders[ROTATE_Z].initial_value;
}

static void	reset_size(t_slider *sliders, double *radius, double *height)
{
	if (radius)
		*radius = sliders[RESIZE_D].initial_value / 2;
	if (height)
		*height = sliders[RESIZE_H].initial_value;
}

static void	reset_position(t_slider	*sliders, t_vec3 *position)
{
	position->x = sliders[TRANSL_X].initial_value;
	position->y = sliders[TRANSL_Y].initial_value;
	position->z = sliders[TRANSL_Z].initial_value;
}

static void	reset_object(t_slider *sliders, t_object *object)
{
	if (object->type == PLANE)
	{
		reset_position(sliders, &object->geo.plane.point);
		reset_direction(sliders, &object->geo.plane.normal);
	}
	else if (object->type == SPHERE)
	{
		reset_position(sliders, &object->geo.sphere.center);
		reset_size(sliders, &object->geo.sphere.radius, NULL);
	}
	else if (object->type == CYLINDER)
	{
		reset_position(sliders, &object->geo.cylinder.center);
		reset_direction(sliders, &object->geo.cylinder.axis);
		reset_size(sliders, &object->geo.cylinder.radius,
			&object->geo.cylinder.height);
	}
	else if (object->type == CONE)
	{
		reset_position(sliders, &object->geo.cone.apex);
		reset_direction(sliders, &object->geo.cone.axis);
		reset_size(sliders, &object->geo.cone.radius,
			&object->geo.cone.height);
	}
}

void	reset_scene(t_panel *panel, t_world *scene)
{
	int	i;
	int	j;

	panel->active_slider = -1;
	panel->dragging = 0;
	panel->drag_start_x = PANEL_W / 2;
	panel->drag_start_norm = 0.5;
	reset_sliders(panel->sliders, panel->total_count);
	i = 0;
	while (i < panel->light_count)
	{
		reset_position(panel->sliders[i], &scene->lights[i]->position);
		i++;
	}
	j = 0;
	while (i < panel->total_count && j < panel->object_count)
	{
		reset_object(panel->sliders[i], scene->objects[j]);
		i++;
		j++;
	}
	scene->camera.vfov = panel->camera_initial_fov;
	scene->camera.lookfrom = panel->camera_initial_origin;
	scene->camera.forword = panel->camera_initial_direction;
	scene->camera.initialized = 0;
	camera_light_initialize(scene);
	camera_render(&scene->camera, scene);
}
