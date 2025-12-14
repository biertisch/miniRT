/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drag.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:20:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 23:02:33 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

void	move_knob(t_panel *panel, t_world *scene, int x)
{
	double	delta;
	double	pos;

	if (panel->active_obj < 0 || panel->active_slider < 0)
		return ;
	delta = (double)(x - panel->drag_start_x) / (SLIDER_W - KNOB_W - 4);
	pos = panel->drag_start_norm + delta;
	if (pos < 0)
		pos = 0;
	else if (pos > 1)
		pos = 1;
	panel->sliders[panel->active_obj][panel->active_slider].knob_pos = pos;
	render_controls(panel, scene);
}

void	detect_active_slider(t_panel *panel, int x, int y)
{
	int	i;

	if (panel->active_obj < 0)
		return ;
	i = 0;
	while (i < SLIDER_COUNT)
	{
		if (x >= panel->sliders[panel->active_obj][i].x
			&& x <= panel->sliders[panel->active_obj][i].x + SLIDER_W
			&& y >= panel->sliders[panel->active_obj][i].y
			&& y < panel->sliders[panel->active_obj][i].y + SLIDER_H)
		{
			panel->active_slider = i;
			return ;
		}
		i++;
	}
	panel->active_slider = -1;
}

int	hit_knob(t_panel *panel, int x, int y)
{
	int	i;
	int	j;
	int	knob_x;

	i = panel->active_obj;
	j = panel->active_slider;
	if (i < 0 || j < 0 || j >= SLIDER_COUNT)
		return (0);
	knob_x = panel->sliders[i][j].x + 2
		+ panel->sliders[i][j].knob_pos * (SLIDER_W - KNOB_W - 4);
	return ((x >= knob_x && x <= knob_x + KNOB_W
			&& y >= panel->sliders[i][j].y + 2
		&& y <= panel->sliders[i][j].y + 2 + KNOB_H));
}

void	begin_drag(t_panel *panel, int x)
{
	int	obj;
	int	slider;

	obj = panel->active_obj;
	slider = panel->active_slider;
	if (obj < 0 || slider < 0)
		return ;
	panel->dragging = 1;
	panel->drag_start_x = x;
	panel->drag_start_norm = panel->sliders[obj][slider].knob_pos;
}
