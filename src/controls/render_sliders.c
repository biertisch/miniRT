/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sliders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:26:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/15 12:21:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	draw_value(t_panel *panel, double value, int y, int pos)
{
	char	*str;
	int		len;
	int		x;

	str = ft_ftoa(value, 1);
	if (!str) // issue warning?
		return ;
	len = ft_strlen(str);
	if (pos == 0)
		x = (PANEL_W - len * CHAR_W) / 2;
	else if (pos == -1)
		x = PADD_X;
	else
		x = PANEL_W - PADD_X - len * CHAR_W;
	draw_string(panel->buffer, str, x, y, WHITE);
	free(str);
}

static void	render_values(t_panel *panel, t_world *scene, int slider, int y)
{
	double	curr;
	double	max;
	double	min;

	panel->sliders[panel->active_obj][slider].base_value
		= get_base_value(scene, panel->active_obj, slider);
	if (slider >= TRANSL_X && slider <= TRANSL_Z)
	{
		curr = panel->sliders[panel->active_obj][slider].base_value;
		max = curr + RANGE_TR / 2;
		min = curr - RANGE_TR / 2;
	}
	else if (slider >= ROTATE_X && slider <= ROTATE_Z)
	{
		curr = 0;
		max = RANGE_RT / 2;
		min = RANGE_RT / 2;
	}
	else if (slider == RESIZE_D || slider == RESIZE_H)
	{
		curr = 1;
		min = 0.5;
		max = 2;
	}
	y += SLIDER_H + 3;
	draw_value(panel, min, y, -1); // check return for failed malloc
	draw_value(panel, curr, y, 0);
	draw_value(panel, max, y, 1);
}

static void	get_block_start_and_end(char *header, int *start, int *end)
{
	if (ft_strcmp(header, TRANSLATE) == 0)
	{
		*start = TRANSL_X;
		*end = TRANSL_Z;
	}
	else if (ft_strcmp(header, ROTATE) == 0)
	{
		*start = ROTATE_X;
		*end = ROTATE_Z;
	}
	else if (ft_strcmp(header, RESIZE) == 0)
	{
		*start = RESIZE_D;
		*end = RESIZE_H;
	}
	else if (ft_strcmp(header, RESIZE2) == 0)
	{
		*start = RESIZE_D;
		*end = RESIZE_H;
	}
}

static void	render_slider_block(t_panel *panel, t_world *scene, char *header, int x, int y)
{
	int	i;
	int	obj;
	int	start;
	int	end;

	draw_string(panel->buffer, header, x, y, LIGHT_GRAY);
	obj = panel->active_obj;
	y += ROW_H;
	get_block_start_and_end(header, &start, &end);
	i = start;
	while (i <= end)
	{
		panel->sliders[obj][i].y = y;
		x = panel->sliders[obj][i].x;
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H), GRAY);
		x += 2 + panel->sliders[obj][i].knob_pos * (SLIDER_W - KNOB_W - 4);
		fill_rectangle(panel, rectangle(x, y + 2, KNOB_W, KNOB_H), BLACK);
		render_values(panel, scene, i, y);
		y += SLIDER_H + ROW_H;
		i++;
	}
}

void	render_sliders(t_panel *panel, t_world *scene)
{
	int	obj;
	int	type;
	int	x;
	int	y;

	obj = panel->active_obj;
	if (obj < 0 || obj >= scene->num_objects + 1) // local light
		return ;
	type = -1;
	if (obj >= 1) // local light
		type = scene->objects[obj - 1]->type;
	x = PADD_X;
	render_slider_block(panel, scene, TRANSLATE, x, TRANSF1_Y);
	if (type == PLANE || type == CYLINDER || type == CONE)
		render_slider_block(panel, scene, ROTATE, x, TRANSF2_Y);
	else if (type == SPHERE)
		render_slider_block(panel, scene, RESIZE2, x, TRANSF2_Y);
	if (type == CYLINDER || type == CONE)
		render_slider_block(panel, scene, RESIZE, x, TRANSF3_Y);
}
