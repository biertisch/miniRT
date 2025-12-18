/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sliders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:26:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 13:03:21 by beatde-a         ###   ########.fr       */
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
	draw_string(panel->buffer, str, (t_point){x, y}, WHITE);
	free(str);
}

static void	render_values(t_panel *panel, t_world *scene, t_slider *slider,
	int y)
{
	double	curr;
	double	min;
	double	max;

	if (slider->type >= TRANSL_X && slider->type <= TRANSL_Z)
	{
		curr = slider->base_value;
		max = curr + RANGE_TR / 2;
		min = curr - RANGE_TR / 2;
	}
	else if (slider->type >= ROTATE_X && slider->type <= ROTATE_Z)
	{
		curr = 0;
		max = RANGE_RT / 2;
		min = -RANGE_RT / 2;
	}
	else if (slider->type == RESIZE_D || slider->type == RESIZE_H)
	{
		curr = 1;
		min = 0.5;
		max = 2;
	}
	draw_value(panel, min, y, -1); // check return for failed malloc?
	draw_value(panel, curr, y, 0);
	draw_value(panel, max, y, 1);
}

static void	render_slider_block(t_panel *panel, t_world *scene, char *header,
	t_point point)
{
	int	i;
	int	obj;
	int	start;
	int	end;

	draw_string(panel->buffer, header, point, LIGHT_GRAY);
	obj = panel->active_obj;
	point.y += ROW_H;
	get_block_start_and_end(header, &start, &end);
	i = start;
	while (i <= end)
	{
		panel->sliders[obj][i].y = point.y;
		point.x = panel->sliders[obj][i].x;
		fill_rectangle(panel, rectangle(point, SLIDER_W, SLIDER_H), GRAY);
		point.x += 2 + panel->sliders[obj][i].knob_pos
			* (SLIDER_W - KNOB_W - 4);
		point.y += 2;
		fill_rectangle(panel, rectangle(point, KNOB_W, KNOB_H), BLACK);
		render_values(panel, scene, &panel->sliders[obj][i],
			point.y + SLIDER_H + 3);
		point.y += SLIDER_H + ROW_H;
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
	if (obj < 0 || obj >= scene->num_objects + 1) // light count
		return ;
	type = -1;
	if (obj >= 1) // light count
		type = scene->objects[obj - 1]->type;
	x = PADD_X;
	render_slider_block(panel, scene, TRANSLATE, (t_point){x, TRANSF1_Y});
	if (type == PLANE || type == CYLINDER || type == CONE)
		render_slider_block(panel, scene, ROTATE, (t_point){x, TRANSF2_Y});
	else if (type == SPHERE)
		render_slider_block(panel, scene, RESIZE2, (t_point){x, TRANSF2_Y});
	if (type == CYLINDER || type == CONE)
		render_slider_block(panel, scene, RESIZE, (t_point){x, TRANSF3_Y});
}
