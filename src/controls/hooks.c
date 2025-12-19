/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:16:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/19 18:16:32 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

int	controls_key_release(int keycode, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (keycode == ESC)
	{
		free_all_the_world(scene);
		exit(0);
	}
	else if (keycode == ENTER)
	{
		transform_scene(scene->panel, scene);
		render_controls(scene->panel, scene);
	}
	else
		do_action(keycode, scene);
	return (0);
}

int	controls_mouse_move(int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (!scene->panel->dragging)
		detect_active_slider(scene->panel, x, y);
	if (scene->panel->dragging)
		move_knob(scene->panel, scene, x);
	return (0);
}

int	controls_mouse_release(int button, int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (button == 1)
	{
		scene->panel->dragging = 0;
		scene->panel->active_slider = -1;
		if (y >= BUTTON_Y + PADD_Y / 2 && y < BUTTON_Y + PADD_Y / 2 + BUTTON_H)
			check_button(scene, x);
		if (y >= OBJ_Y && y < TRANSF_Y)
			select_object(scene->panel, y);
	}
	render_controls(scene->panel, scene);
	return (0);
}

int	controls_mouse_hook(int button, int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (button == 1)
	{
		if (!scene->panel->dragging)
			detect_active_slider(scene->panel, x, y);
		if (scene->panel->active_obj != -1 && scene->panel->active_slider != -1
			&& hit_knob(scene->panel, x, y))
			begin_drag(scene->panel, x);
	}
	else if (button == 4 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_up(scene->panel);
	else if (button == 5 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_down(scene->panel);
	render_controls(scene->panel, scene);
	return (0);
}

void	setup_controls_hooks(t_panel *panel, t_world *scene)
{
	mlx_hook(panel->win, KeyRelease, 1L << 1, controls_key_release, scene);
	mlx_hook(panel->win, DestroyNotify, 0, handle_destroy, scene);
	mlx_hook(panel->win, MotionNotify, 1L << 6, controls_mouse_move, scene);
	mlx_hook(panel->win, ButtonRelease, 1L << 3, controls_mouse_release, scene);
	mlx_mouse_hook(panel->win, controls_mouse_hook, scene);
}
