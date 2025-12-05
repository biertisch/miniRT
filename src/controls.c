/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:58:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/05 20:30:08 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//bitmap
static int	init_letter_n_to_z(char c, int row)
{
	static const int letters[13][8] = {
		{0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
		{0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
		{0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
		{0x3C,0x66,0x66,0x66,0x6E,0x6C,0x36,0x00},
		{0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00},
		{0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
		{0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00},
		{0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
		{0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
		{0x63,0x63,0x36,0x36,0x1C,0x1C,0x08,0x00},
		{0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
		{0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00},
		{0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}
	};

	return (letters[c - 'N'][row]);
}

static int	init_letter_a_to_m(char c, int row)
{
	static const int letters[13][8] = {
		{0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},
		{0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
		{0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
		{0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
		{0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},
		{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00},
		{0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00},
		{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
		{0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
		{0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00},
		{0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
		{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
		{0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}
	};

	return (letters[c - 'A'][row]);
}

static int	init_letter(char c, int row)
{
	char	upper;

	if (ft_isalpha(c))
	{
		upper = ft_toupper(c);
		if (upper >= 'A' && upper <= 'M')
			return (init_letter_a_to_m(upper, row));
		return (init_letter_n_to_z(upper, row));
	}
	return (0);
}

static int	init_digit(char c, int row)
{
	static const int digits[10][8] = {
		{0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00},
		{0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
		{0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00},
		{0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
		{0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x1E,0x00},
		{0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
		{0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00},
		{0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00},
		{0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
		{0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}
	};

	if (ft_isdigit(c))
		return (digits[c - '0'][row]);
	return (0);
}

static int	get_font(char c, int row)
{
	if (ft_isdigit(c))
		return (init_digit(c, row));
	if (ft_isalpha(c))
		return (init_letter(c, row));
	return (0);
}

//render functions
static void	draw_char(t_data *img, char c, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < CHAR_H)
	{
		j = 0;
		while (j < CHAR_W)
		{
			if (get_font(c, i) & (1 << (CHAR_W - 1 - j)))
				my_mlx_pixel_put(img, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

static void	draw_string(t_data *img, const char *s, int x, int y, int color)
{
	int	i;

	i = 0;
	while (s[i])
	{
		draw_char(img, s[i], x + i * 8, y, color);
		i++;
	}
}

static void draw_dashed_line(t_panel *panel, int y, int color)
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

static void	fill_rectangle(t_panel *panel, t_rect rect)
{
	int i;
	int	j;

	if (rect.x < 0 || rect.x + rect.width > panel->width || rect.y < 0 || rect.y + rect.height > panel->height)
		return ;
	i = 0;
	while (i < rect.height)
	{
		j = 0;
		while (j < rect.width)
		{
			my_mlx_pixel_put(panel->buffer, rect.x + j, rect.y + i, rect.color);
			j++;
		}
		i++;
	}
}

static t_rect	rectangle(int x, int y, int width, int height, int color)
{
	t_rect rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	rect.color = color;
	return (rect);
}

static void	render_titles(t_panel *panel)
{
	int	padding;
	int	x;
	int	y;

	padding = 20;
	x = panel->width / 2 - (ft_strlen(CONTROLS) * CHAR_W) / 2;
	y = padding;
	draw_string(panel->buffer, CONTROLS, x, y, LIGHT_GRAY);
}

static void	render_buttons(t_panel *panel)
{
	int	padding;
	int	x;
	int	y;

	padding = 30;
	y = 565;
	x = padding;
	fill_rectangle(panel, rectangle(x, y, BUTTON_W, BUTTON_H, GRAY));
	x = panel->width - padding - BUTTON_W;
	fill_rectangle(panel, rectangle(x, y, BUTTON_W, BUTTON_H, GRAY));
}

static void	render_sliders(t_panel *panel)
{
	int	x;
	int	y;
	int	i;

	x = (panel->width - SLIDER_W) / 2;
	y = 220;
	i = 0;
	while (i < 8)
	{
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H, GRAY));
		fill_rectangle(panel, rectangle(panel->width / 2, y + 2, KNOB_W, KNOB_H, BLACK));
		y += 35;
		if (i == 2 || i == 5)
			y += 35;
		i++;
	}
}

static void	render_separators(t_panel *panel)
{
	draw_dashed_line(panel, 40, GRAY);
	draw_dashed_line(panel, 160, GRAY);
	draw_dashed_line(panel, 560, GRAY);
}

static void	render_controls(t_panel *panel, t_world *wld)
{
	fill_rectangle(panel, rectangle(0, 0, panel->width, panel->height, BLACK));
	render_separators(panel);
	render_sliders(panel);
	render_buttons(panel);
	render_titles(panel);
	mlx_put_image_to_window(wld->mlx, panel->win, panel->buffer->img, 0, 0);
}

static int	create_buffer(t_panel *panel, t_world *wld)
{
	panel->buffer = malloc(sizeof(t_data));
	if (!panel->buffer)
	{
		perror("Error\nmalloc");
		return (0);
	}
	panel->buffer->img = mlx_new_image(wld->mlx, panel->width, panel->height);
	if (!panel->buffer->img)
	{
		ft_putstr_fd("Error\nFailed to create image buffer\n", STDERR_FILENO);
		return (0);
	}
	panel->buffer->addr = mlx_get_data_addr(panel->buffer->img, &panel->buffer->bits_per_pixel, &panel->buffer->line_length, &panel->buffer->endian);
	if (!panel->buffer->addr)
	{
		ft_putstr_fd("Error\nFailed to access image buffer\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	setup_controls(t_world *wld)
{
	t_panel panel; //integrate into t_world

	panel.width = CTRL_W;
	panel.height = CTRL_H;
	panel.win = mlx_new_window(wld->mlx, panel.width, panel.height, "Control Panel");
	if (!panel.win)
	{
		ft_putstr_fd("Error\nFailed to create window\n", STDERR_FILENO);
		return (0);
	}
	if (!create_buffer(&panel, wld)) //free panel->win
		return (0);
	render_controls(&panel, wld);
	return (1);
}
