/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sliders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:26:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 23:07:32 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	draw_value(t_panel *panel, int value, int y, int pos)
{
	char	*str;
	int		x;

	str = ft_itoa(value);
	if (!str) // issue warning?
		return ;
	if (pos == 0)
		x = PANEL_W / 2 - (ft_strlen(str) * CHAR_W * 2) / 2;
	else if (pos == -1)
		x = PADD_X;
	else
		x = PANEL_W - PADD_X - ft_strlen(str) * CHAR_W * 2;
	draw_string(panel->buffer, str, x, y, WHITE);
	free(str);
}

static void	render_values(t_panel *panel, t_world *scene, int slider, int y)
{
	int		curr;
	int		max;
	int		min;

	panel->sliders[panel->active_obj][slider].base_value = get_base_value(scene, panel->active_obj, slider);
	curr = round(panel->sliders[panel->active_obj][slider].base_value);
	max = curr + TL_RANGE / 2;
	min = curr - TL_RANGE / 2;
	if ((slider == RESIZE_D || slider == RESIZE_H) && min < 0)
		min = 0;
	if (slider == ROTATE_X || slider == ROTATE_Y || slider == ROTATE_Z)
	{
		max = curr + RT_RANGE / 2;
		min = curr - RT_RANGE / 2;
	}
	y += SLIDER_H + 3;
	draw_value(panel, min, y, -1); // check return for failed malloc
	draw_value(panel, curr, y, 0);
	draw_value(panel, max, y, 1);
}

static void	render_resizing(t_panel *panel, t_world *scene, int x, int y)
{
	int	obj;
	int	type;
	int	i;
	int	count;

	obj = panel->active_obj;
	type = scene->objects[obj - 2]->type;
	if (type == CYLINDER)
	{
		draw_string(panel->buffer, RESIZE, x, y, LIGHT_GRAY);
		count = SLIDER_COUNT;
	}
	else
	{
		draw_string(panel->buffer, RESIZE2, x, y, LIGHT_GRAY);
		count = SLIDER_COUNT - 1;
	}
	i = 6;
	while (i < count)
	{
		x = panel->sliders[obj][i].x;
		y = panel->sliders[obj][i].y;
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H), GRAY);
		x += 2 + panel->sliders[obj][i].knob_pos * (SLIDER_W - KNOB_W - 4);
		fill_rectangle(panel, rectangle(x, y + 2, KNOB_W, KNOB_H), BLACK);
		render_values(panel, scene, i, y);
		i++;
	}
}

static void	render_rotation(t_panel *panel, t_world *scene, int x, int y)
{
	int	i;
	int	obj;

	draw_string(panel->buffer, ROTATE, x, y, LIGHT_GRAY);
	obj = panel->active_obj;
	i = 3;
	while (i < 6)
	{
		x = panel->sliders[obj][i].x;
		y = panel->sliders[obj][i].y;
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H), GRAY);
		x += 2 + panel->sliders[obj][i].knob_pos * (SLIDER_W - KNOB_W - 4);
		fill_rectangle(panel, rectangle(x, y + 2, KNOB_W, KNOB_H), BLACK);
		render_values(panel, scene, i, y);
		i++;
	}
}

static void	render_translation(t_panel *panel, t_world *scene, int x, int y)
{
	int	i;
	int	obj;

	draw_string(panel->buffer, TRANSLATE, x, y, LIGHT_GRAY);
	obj = panel->active_obj;
	i = 0;
	while (i < 3)
	{
		x = panel->sliders[obj][i].x;
		y = panel->sliders[obj][i].y;
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H), GRAY);
		x += 2 + panel->sliders[obj][i].knob_pos * (SLIDER_W - KNOB_W - 4);
		fill_rectangle(panel, rectangle(x, y + 2, KNOB_W, KNOB_H), BLACK);
		render_values(panel, scene, i, y);
		i++;
	}
}

// remove rotation from spheres and lights
void	render_sliders(t_panel *panel, t_world *scene)
{
	int	obj;
	int	type;
	int	x;
	int	y;

	obj = panel->active_obj;
	if (obj < 0 || obj >= scene->num_objects + 2)
		return ;
	type = -1;
	if (obj >= 2)
		type = scene->objects[obj - 2]->type;
	x = PADD_X;
	y = TRANSF_Y + PADD_Y + ROW_H;
	render_translation(panel, scene, x, y);
	y += ROW_H + 3 * (SLIDER_H + ROW_H) + 5;
	render_rotation(panel, scene, x, y);
	if (type != -1 && (type == CYLINDER || type == SPHERE))
	{
		y += ROW_H + 3 * (SLIDER_H + ROW_H) + 5;
		render_resizing(panel, scene, x, y);
	}
}
