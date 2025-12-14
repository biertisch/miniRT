/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:30:57 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 16:13:57 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

void	draw_dashed_line(t_panel *panel, int y, int color)
{
	int	x;
	int	dash;
	int	gap;
	int	count;

	if (y < 0 || y >= panel->height)
		return ;
	dash = 8;
	gap = 4;
	x = gap;
	while (x < panel->width)
	{
		count = 0;
		while (count < dash && x < panel->width)
		{
			my_mlx_pixel_put(panel->buffer, x, y, color);
			x++;
			count++;
		}
		x += gap;
	}
}

void	outline_rectangle(t_panel *panel, t_rect rect, int color)
{
	int row;
	int	col;

	if (rect.x < 0 || rect.x + rect.width > panel->width || rect.y < 0 || rect.y + rect.height > panel->height)
		return ;
	row = 0;
	while (row < rect.height)
	{
		col = 0;
		while (col < rect.width)
		{
			if (row == 0 || row == rect.height - 1 || col == 0 || col == rect.width - 1)
				my_mlx_pixel_put(panel->buffer, rect.x + col, rect.y + row, color);
			col++;
		}
		row++;
	}
}

void	fill_rectangle(t_panel *panel, t_rect rect, int color)
{
	int row;
	int	col;

	if (rect.x < 0 || rect.x + rect.width > panel->width || rect.y < 0 || rect.y + rect.height > panel->height)
		return ;
	row = 0;
	while (row < rect.height)
	{
		col = 0;
		while (col < rect.width)
		{
			my_mlx_pixel_put(panel->buffer, rect.x + col, rect.y + row, color);
			col++;
		}
		row++;
	}
}

t_rect	rectangle(int x, int y, int width, int height)
{
	return ((t_rect){x, y, width, height});
}
