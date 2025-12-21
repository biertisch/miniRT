/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worldfree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 13:47:21 by bliu              #+#    #+#             */
/*   Updated: 2025/12/20 19:24:14 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	error_exit(t_world *wld, char *message)
{
	perror(message); // replace with ft_putstr_fd if needed for non-system failures
	free_all_the_world(wld);
	exit(EXIT_FAILURE);
}

void	free_and_set_null(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_object(t_object *obj)
{
	if (obj->type == SPHERE && obj->geo.sphere.mat.data.lamb.tex)
		free_and_set_null((void **)&obj->geo.sphere.mat.data.lamb.tex);
	else if (obj->type == PLANE && obj->geo.plane.mat.data.lamb.tex)
		free_and_set_null((void **)&obj->geo.plane.mat.data.lamb.tex);
	else if (obj->type == CYLINDER && obj->geo.cylinder.mat.data.lamb.tex)
		free_and_set_null((void **)&obj->geo.cylinder.mat.data.lamb.tex);
	else if (obj->type == CONE && obj->geo.cone.mat.data.lamb.tex)
		free_and_set_null((void **)&obj->geo.cone.mat.data.lamb.tex);
	else if (obj->type == QUAD && obj->geo.quad.mat.data.lamb.tex)
		free_and_set_null((void **)&obj->geo.quad.mat.data.lamb.tex);
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
		if (wld->panel)
			free_panel(wld, wld->panel);
		if (wld->mlx)
		{
			mlx_destroy_display(wld->mlx);
			free(wld->mlx);
		}
		free_objects(wld);
	}
}
