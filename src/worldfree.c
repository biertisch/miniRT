/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worldfree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 13:47:21 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 12:14:11 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_and_set_null(void **ptr)
{
	t_texture		*tex;
	t_checker_tex	*ct;

	if (ptr && *ptr)
	{
		tex = (t_texture *)(*ptr);
		if (tex->type == CHECKER)
		{
			ct = (t_checker_tex *)tex;
			free_and_set_null((void **)&ct->even);
			free_and_set_null((void **)&ct->odd);
		}
		if (tex->type != PICTURE)
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

static void	free_lights(t_world *wld)
{
	int	i;

	i = 0;
	while (i < wld->num_lights)
	{
		free(wld->lights[i]);
		wld->lights[i] = NULL;
		i++;
	}
}

void	free_all_the_world(t_world *wld)
{
	if (wld)
	{
		if (wld->mlx && wld->pic_c_tex.pic_tex.img)
			mlx_destroy_image(wld->mlx, wld->pic_c_tex.pic_tex.img);
		if (wld->mlx && wld->pic_c_tex.bump_tex.img)
			mlx_destroy_image(wld->mlx, wld->pic_c_tex.bump_tex.img);
		if (wld->mlx && wld->win)
			mlx_destroy_window(wld->mlx, wld->win);
		if (wld->panel)
			free_panel(wld, wld->panel);
		if (wld->mlx)
		{
			mlx_destroy_display(wld->mlx);
			free(wld->mlx);
		}
		wld->mlx = NULL;
		wld->win = NULL;
		wld->pic_c_tex.pic_tex.img = NULL;
		wld->pic_c_tex.bump_tex.img = NULL;
		wld->panel = NULL;
		free_lights(wld);
		free_objects(wld);
	}
}
