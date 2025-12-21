/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_extend.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:21:31 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 15:27:19 by bliu             ###   ########.fr       */
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
	}
	else
		printf("Use mouse to select object first.\n");
}

void	extend_action(int keycode, t_world *wld)
{
	if (keycode == 'p')
		printf("📷 Info:\nAt: (%.2f, %.2f, %.2f)\nTarget: (%.2f, %.2f, %.2f)\n",
			wld->camera.lookfrom.x, wld->camera.lookfrom.y,
			wld->camera.lookfrom.z, wld->camera.forword.x,
			wld->camera.forword.y, wld->camera.forword.z);
	else if (keycode == '=')
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
