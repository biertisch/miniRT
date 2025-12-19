/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worldfree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 13:47:21 by bliu              #+#    #+#             */
/*   Updated: 2025/12/17 18:13:34 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_object(t_object *obj)
{
	if (obj->type == SPHERE && obj->geo.sphere.mat.data.lamb.tex)
	{
		free(obj->geo.sphere.mat.data.lamb.tex);
		obj->geo.sphere.mat.data.lamb.tex = NULL;
	}
	else if (obj->type == PLANE && obj->geo.plane.mat.data.lamb.tex)
	{
		free(obj->geo.plane.mat.data.lamb.tex);
		obj->geo.plane.mat.data.lamb.tex = NULL;
	}
	else if (obj->type == CYLINDER && obj->geo.cylinder.mat.data.lamb.tex)
	{
		free(obj->geo.cylinder.mat.data.lamb.tex);
		obj->geo.cylinder.mat.data.lamb.tex = NULL;
	}
	else if (obj->type == CONE && obj->geo.cone.mat.data.lamb.tex)
	{
		free(obj->geo.cone.mat.data.lamb.tex);
		obj->geo.cone.mat.data.lamb.tex = NULL;
	}
	else if (obj->type == QUAD && obj->geo.quad.mat.data.lamb.tex)
	{
		free(obj->geo.quad.mat.data.lamb.tex);
		obj->geo.quad.mat.data.lamb.tex = NULL;
	}
	free(obj);
}

void	free_objects(t_world *wld)
{
	if (wld->num_objects > 0)
	{
		while (0 < wld->num_objects)
		{
			if (wld->objects[wld->num_objects - 1])
				free_object(wld->objects[wld->num_objects - 1]);
			wld->objects[wld->num_objects - 1] = NULL;
			wld->num_objects--;
		}
	}
}

void	free_all_the_world(t_world *wld)
{
	if (wld)
	{
		if (wld->mlx && wld->win)
			mlx_destroy_window(wld->mlx, wld->win);
		if (wld->mlx)
		{
			mlx_destroy_display(wld->mlx);
			free(wld->mlx);
		}
		if (wld->panel)
		{
			free_sliders(&wld->panel->sliders, wld->num_objects + 1);
			free(wld->panel);
		}
		free_objects(wld);
	}
}
