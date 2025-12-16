/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:16:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/16 10:58:46 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	reset_panel(t_panel *panel, int object_count)
{
	panel->active_slider = -1;
	panel->dragging = 0;
	panel->drag_start_x = PANEL_W / 2;
	panel->drag_start_norm = 0.5;
	init_slider_type(panel->sliders, object_count);
}

int	controls_mouse_release_hook(int button, int x, int y, void *param)
{
	t_panel	*panel;

	panel = (t_panel *)param;
	if (button == 1)
	{
		panel->dragging = 0;
		panel->active_slider = -1;
	}
	return (0);
}

int	controls_mouse_move_hook(int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	detect_active_slider(scene->panel, x, y);
	if (scene->panel->dragging)
		move_knob(scene->panel, scene, x);
	return (0);
}

int	controls_mouse_hook(int button, int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (button == 1)
	{
		detect_active_slider(scene->panel, x, y);
		if (scene->panel->active_obj != -1 && scene->panel->active_slider != -1
			&& hit_knob(scene->panel, x, y))
			begin_drag(scene->panel, x);
		else if (y >= OBJ_Y && y < TRANSF_Y)
			select_object(scene->panel, scene->num_objects, y);
		else if (y >= BUTTON_Y + PADD_Y && y < BUTTON_Y + PADD_Y + BUTTON_H)
		{
			if (x >= PADD_X && x < PADD_X + BUTTON_W)
				transform_scene(scene->panel, scene);
			else if (x >= PANEL_W - PADD_X - BUTTON_W && x < PANEL_W - PADD_X)
				reset_panel(scene->panel, scene->num_objects);
		}
	}
	else if (button == 4 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_up(scene->panel);
	else if (button == 5 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_down(scene->panel, scene->num_objects);
	render_controls(scene->panel, scene);
	return (0);
}
