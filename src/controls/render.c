/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:30:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/15 09:44:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static void	render_buttons(t_panel *panel)
{
	int	b1_x;
	int	b2_x;
	int	y;

	y = BUTTON_Y + PADD_Y / 2;
	b1_x = (panel->width / 2 - BUTTON_W) / 2;
	fill_rectangle(panel, rectangle(b1_x, y, BUTTON_W, BUTTON_H), LIGHT_GRAY);
	b2_x = b1_x + panel->width / 2;
	fill_rectangle(panel, rectangle(b2_x, y, BUTTON_W, BUTTON_H), LIGHT_GRAY);
	y += (BUTTON_H - CHAR_H) / 2 ;
	b1_x += (BUTTON_W - ft_strlen(RENDER) * CHAR_W) / 2;
	draw_string(panel->buffer, RENDER, b1_x, y, BLACK);
	b2_x += (BUTTON_W - ft_strlen(RESET) * CHAR_W) / 2;
	draw_string(panel->buffer, RESET, b2_x, y, BLACK);
}

static void	render_sections(t_panel *panel)
{
	int	x;
	int	y;

	draw_dashed_line(panel, OBJ_Y, GRAY);
	draw_dashed_line(panel, TRANSF_Y, GRAY);
	draw_dashed_line(panel, BUTTON_Y, GRAY);
	x = (panel->width - ft_strlen(CONTROLS) * CHAR_W) / 2;
	y = PADD_Y;
	draw_string(panel->buffer, CONTROLS, x, y, WHITE);
	x = (panel->width - ft_strlen(OBJECTS) * CHAR_W) / 2;
	y = OBJ_Y + PADD_Y;
	draw_string(panel->buffer, OBJECTS, x, y, WHITE);
	x = (panel->width - ft_strlen(TRANSFORM) * CHAR_W) / 2;
	y = TRANSF_Y + PADD_Y;
	draw_string(panel->buffer, TRANSFORM, x, y, WHITE);
}

void	render_controls(t_panel *panel, t_world *scene)
{
	fill_rectangle(panel, rectangle(0, 0, panel->width, panel->height), BLACK);
	render_sections(panel);
	render_buttons(panel);
	render_object_list(panel, scene);
	render_sliders(panel, scene);
	mlx_put_image_to_window(scene->mlx, panel->win, panel->buffer->img, 0, 0);
}
