/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:06 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/15 11:45:41 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static double	commit_slider(t_slider *slider, int range)
{
	double	value;

	value = slider->base_value + (slider->knob_pos - 0.5) * range;
	slider->base_value = value;
	slider->knob_pos = 0.5;
	return (value);
}

static void	transform_objects(t_slider **sliders, t_object **objects, int object_count)
{
	int	i;

	i = 0;
	while (i < object_count)
	{
		if (objects[i]->type == CYLINDER)
		{
			objects[i]->geo.cylinder.center.x = commit_slider(&sliders[i][TRANSL_X], RANGE_TR);
			objects[i]->geo.cylinder.center.y = commit_slider(&sliders[i][TRANSL_Y], RANGE_TR);
			objects[i]->geo.cylinder.center.z = commit_slider(&sliders[i][TRANSL_Z], RANGE_TR);
			// rotate axis vector along x, y, and z
			objects[i]->geo.cylinder.radius = commit_slider(&sliders[i][RESIZE_D], RANGE_RS) / 2;
			objects[i]->geo.cylinder.height = commit_slider(&sliders[i][RESIZE_H], RANGE_RS);
		}
		else if (objects[i]->type == SPHERE)
		{
			objects[i]->geo.sphere.center.x = commit_slider(&sliders[i][TRANSL_X], RANGE_TR);
			objects[i]->geo.sphere.center.y = commit_slider(&sliders[i][TRANSL_Y], RANGE_TR);
			objects[i]->geo.sphere.center.z = commit_slider(&sliders[i][TRANSL_Z], RANGE_TR);
			objects[i]->geo.sphere.radius = commit_slider(&sliders[i][RESIZE_D], RANGE_RS) / 2;
		}
		else if (objects[i]->type == PLANE)
		{
			objects[i]->geo.plane.point.x = commit_slider(&sliders[i][TRANSL_X], RANGE_TR);
			objects[i]->geo.plane.point.y = commit_slider(&sliders[i][TRANSL_Y], RANGE_TR);
			objects[i]->geo.plane.point.z = commit_slider(&sliders[i][TRANSL_Z], RANGE_TR);
			// rotate normal vector along x, y, and z
		}
		i++;
	}
}

void	transform_scene(t_panel *panel, t_world *scene)
{
	//transform light
	transform_objects(panel->sliders + 2, scene->objects, scene->num_objects);
	camera_render(&scene->camera, scene);
	render_controls(panel, scene);
}
