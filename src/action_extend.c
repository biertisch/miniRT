/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_extend.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:21:31 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 14:06:39 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static	void	resize_obj(t_world *wld, float scale)
{
	if (wld->current_obj)
	{
		if (wld->current_obj->type == SPHERE)
		{
			wld->current_obj->geo.sphere.radius *= scale;
			camera_render(&wld->camera, wld);
		}
		else if (wld->current_obj->type == CYLINDER)
		{
			wld->current_obj->geo.cylinder.radius *= scale;
			wld->current_obj->geo.cylinder.height *= scale;
			camera_render(&wld->camera, wld);
		}
		else if (wld->current_obj->type == CONE)
		{
			wld->current_obj->geo.cone.radius *= scale;
			wld->current_obj->geo.cone.height *= scale;
			camera_render(&wld->camera, wld);
		}
		else
			printf("Resize not supported for this object type.\n");
	}
	else
		printf("Use mouse to select object first.\n");
}

void	extend_action(int keycode, t_world *wld)
{
	printf("Key pressed: %d\n", keycode);
	if (keycode == 'p')
		print_info(wld);
	else if (keycode == 'h')
		print_help();
	else if (keycode == '+')
		resize_obj(wld, 1.1f);
	else if (keycode == '-')
		resize_obj(wld, 0.9f);
	else if (keycode == 'r')
	{
		wld->camera_auto.in_rot = !wld->camera_auto.in_rot;
		if (wld->camera_auto.in_rot)
			printf("Auto-rotation ON 🔄\n");
		else
			printf("Auto-rotation OFF ⏸️\n");
	}
}

int	loop(void *param)
{
	t_world	*wld;

	wld = (t_world *)param;
	if (!wld->camera_auto.in_rot)
		return (0);
	else
	{
		if (!wld->camera_auto.inited)
		{
			init_auto_c(&wld->camera_auto, &wld->camera);
			wld->camera_auto.inited = 1;
		}
		wld->camera.initialized = 0;
		update_orbit_camera(&wld->camera, &wld->camera_auto);
		camera_light_initialize(wld);
		camera_render(&wld->camera, wld);
	}
	return (0);
}
