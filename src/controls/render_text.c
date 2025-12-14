/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:08:09 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 18:37:49 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static int	get_char(char c, int row)
{
	if (ft_isdigit((unsigned char)c))
		return (get_digit(c, row));
	if (ft_isalpha((unsigned char)c))
		return (get_letter(c, row));
	return (get_punct(c, row));
}

void	draw_char(t_data *img, char c, int x, int y, int color)
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
				my_mlx_pixel_put(img, x + col, y + row, color);
			col++;
		}
		row++;
	}
}

void	draw_string(t_data *img, const char *s, int x, int y, int color)
{
	int	i;

	if (!s || !img)
		return ;
	i = 0;
	while (s[i])
	{
		draw_char(img, s[i], x + i * CHAR_W, y, color);
		i++;
	}
}
