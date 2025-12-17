/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:08:09 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/16 12:53:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static int	get_digit(char c, int row)
{
	static const int	digits[10][12] = {
	{0x03F0, 0x07F8, 0x0C1C, 0x0C3C, 0x0C3C, 0x0C3C,
		0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000, 0x0000},
	{0x0180, 0x0380, 0x0780, 0x0F80, 0x0180, 0x0180,
		0x0180, 0x0FFC, 0x0FFC, 0x0000, 0x0000, 0x0000},
	{0x03F0, 0x07F8, 0x0C0C, 0x0018, 0x0030, 0x0060,
		0x00C0, 0x0FF8, 0x0FF8, 0x0000, 0x0000, 0x0000},
	{0x03F0, 0x07F8, 0x000C, 0x0018, 0x03F0, 0x07F8,
		0x000C, 0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000},
	{0x0060, 0x00E0, 0x01E0, 0x03E0, 0x07B0, 0x0F30,
		0x0FFC, 0x0FFC, 0x0060, 0x0060, 0x0000, 0x0000},
	{0x0FF8, 0x0FF8, 0x0C00, 0x0FF0, 0x0FFC, 0x000C,
		0x000C, 0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000},
	{0x03F0, 0x07F8, 0x0C1C, 0x0C00, 0x0FF0, 0x0FFC,
		0x0C1C, 0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000},
	{0x0FFC, 0x0FFC, 0x0018, 0x0030, 0x0060, 0x00C0,
		0x0180, 0x0300, 0x0600, 0x0000, 0x0000, 0x0000},
	{0x03F0, 0x07F8, 0x0C1C, 0x0C1C, 0x03F0, 0x07F8,
		0x0C1C, 0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000},
	{0x03F0, 0x07F8, 0x0C1C, 0x0C1C, 0x07F8, 0x03F0,
		0x001C, 0x0C1C, 0x07F8, 0x03F0, 0x0000, 0x0000}
	};

	return (digits[c - '0'][row]);
}

static int	get_char(char c, int row)
{
	if (ft_isdigit((unsigned char)c))
		return (get_digit(c, row));
	if (ft_isalpha((unsigned char)c))
		return (get_letter(c, row));
	return (get_symbol(c, row));
}

void	draw_char(t_data *img, char c, t_point point, int color)
{
	int	row;
	int	col;

	row = 0;
	while (row < CHAR_H)
	{
		col = 0;
		while (col < CHAR_W)
		{
			if (get_char(c, row) & (1 << (CHAR_W - 1 - col)))
				my_mlx_pixel_put(img, point.x + col, point.y + row, color);
			col++;
		}
		row++;
	}
}

void	draw_string(t_data *img, const char *s, t_point point, int color)
{
	int	i;

	if (!s || !img)
		return ;
	i = 0;
	while (s[i])
	{
		draw_char(img, s[i], (t_point){point.x + i * CHAR_W, point.y}, color);
		i++;
	}
}
