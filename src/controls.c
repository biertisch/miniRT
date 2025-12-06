/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:58:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/06 16:38:38 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//bitmap
static int	init_punct(char c, int row)
{
	static const int punct[5][8] = {
		{0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00},
		{0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00},
		{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30},
		{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},
		{0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00},
	};

	if (c == '(')
		return (punct[0][row]);
	if (c == ')')
		return (punct[1][row]);
	if (c == ',')
		return (punct[2][row]);
	if (c == '.')
		return (punct[3][row]);
	if (c == '-')
		return (punct[4][row]);
	return (0);
}

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
	return (init_punct(c, row));
}

//render
static void	draw_char(t_data *img, char c, int x, int y, int color)
{
	int	row;
	int	col;

	row = 0;
	while (row < CHAR_H)
	{
		col = 0;
		while (col < CHAR_W)
		{
			if (get_font(c, row) & (1 << (CHAR_W - 1 - col)))
				my_mlx_pixel_put(img, x + col, y + row, color);
			col++;
		}
		row++;
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

static void	outline_rectangle(t_panel *panel, t_rect rect, int color)
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

static void	fill_rectangle(t_panel *panel, t_rect rect, int color)
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

static t_rect	rectangle(int x, int y, int width, int height)
{
	return ((t_rect){x, y, width, height});
}

//hooks
static void	scroll_down(t_panel *panel, int object_count)
{
	int	max_offset;

	max_offset = object_count - panel->visible_objs;
	if (max_offset < 0)
		max_offset = 0;
	if (panel->scroll_offset < max_offset)
		panel->scroll_offset++;
}

static void	scroll_up(t_panel *panel)
{
	if (panel->scroll_offset > 0)
		panel->scroll_offset--;
}

static void	select_object(t_panel *panel, int object_count, int y)
{
	int	y_start;
	int	row;
	int	index;

	y_start = OBJ_Y + PADD_Y + ROW_H;
	if (y < y_start)
		return ;
	row = (y - y_start) / ROW_H;
	if (row < 0 || row >= panel->visible_objs)
		return ;
	index = panel->scroll_offset + row;
	if (index >= object_count + 2)
		return ;
	panel->curr_obj = index;
}

static int	mouse_hook(int button, int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	(void)x;
	if (y < OBJ_Y || y > TRANSF_Y || (button != 1 && button != 4 && button != 5))
		return (0);
	if (button == 1)
		select_object(scene->panel, scene->num_objects, y);
	else if (button == 4)
		scroll_up(scene->panel);
	else if (button == 5)
		scroll_down(scene->panel, scene->num_objects);
	render_controls(scene->panel, scene);
	return (0);
}

//movable parts
static char	*get_type_name(t_geo_type type)
{
	static char	*type_names[] = {
		"sphere", "plane", "cilinder", "bvh node", "quad", "unknown"
	};

	return (type_names[type]);
}

static void	draw_object_row(t_panel *panel, t_world *scene, int index, int y)
{
	char	name[BUFF_SIZE];
	char	*index_str;

	index_str = ft_itoa(index);
	if (!index_str) //issue warning and communicate error
		return ;
	ft_strlcpy(name, index_str, BUFF_SIZE);
	free(index_str);
	ft_strlcat(name, " ", BUFF_SIZE);
	if (index == 0)
		ft_strlcat(name, "Camera", BUFF_SIZE);
	else if (index == 1)
		ft_strlcat(name, "Light", BUFF_SIZE);
	else
	{
		index -= 2;
		ft_strlcat(name, get_type_name(scene->objects[index]->type), BUFF_SIZE);
	}
	draw_string(panel->buffer, name, PADD_X, y, WHITE);
}

static void	draw_highlight(t_panel *panel, int row, int y)
{
	int	x;
	int	width;

	x = PADD_X - 5;
	y -= 7;
	width = panel->width - 2 * x;
	outline_rectangle(panel, rectangle(x, y, width, ROW_H), GRAY);
}

static void	render_object_list(t_panel *panel, t_world *scene)
{
	int	row;
	int	obj_index;
	int	y;

	row = 0;
	while (row < panel->visible_objs)
	{
		obj_index = panel->scroll_offset + row;
		if (obj_index >= scene->num_objects + 2) // camera + 1 light
			break ;
		y = OBJ_Y + PADD_Y + ROW_H + row * ROW_H;
		if (obj_index == panel->curr_obj)
			draw_highlight(panel, row, y);
		draw_object_row(panel, scene, obj_index, y);
		row++;
	}
}

//fixed parts
static void	render_buttons(t_panel *panel)
{
	int	x;
	int	y;

	y = BUTTON_Y + 15;
	x = PADD_X;
	fill_rectangle(panel, rectangle(x, y, BUTTON_W, BUTTON_H), LIGHT_GRAY);
	x = panel->width - PADD_X - BUTTON_W;
	fill_rectangle(panel, rectangle(x, y, BUTTON_W, BUTTON_H), LIGHT_GRAY);
	y += 10;
	x = PADD_X + (BUTTON_W / 2 - (ft_strlen(RENDER) * CHAR_W) / 2);
	draw_string(panel->buffer, RENDER, x, y, BLACK);
	x = panel->width - PADD_X - BUTTON_W + (BUTTON_W / 2 - (ft_strlen(RESET) * CHAR_W) / 2);
	draw_string(panel->buffer, RESET, x, y, BLACK);
}

static void	render_sliders(t_panel *panel)
{
	int	x;
	int	y;
	int	i;

	x = (PANEL_W - SLIDER_W) / 2;
	y = TRANSF_Y + 2 * ROW_H + PADD_Y;
	i = 0;
	while (i < 8)
	{
		fill_rectangle(panel, rectangle(x, y, SLIDER_W, SLIDER_H), GRAY);
		fill_rectangle(panel, rectangle(panel->width / 2, y + 2, KNOB_W, KNOB_H), BLACK);
		y += SLIDER_H + ROW_H;
		if (i == 2 || i == 5)
			y += ROW_H;
		i++;
	}
}

static void	render_sections(t_panel *panel)
{
	int	x;
	int	y;

	draw_dashed_line(panel, OBJ_Y, GRAY);
	draw_dashed_line(panel, TRANSF_Y, GRAY);
	draw_dashed_line(panel, BUTTON_Y, GRAY);
	x = panel->width / 2 - (ft_strlen(CONTROLS) * CHAR_W) / 2;
	y = PADD_Y;
	draw_string(panel->buffer, CONTROLS, x, y, WHITE);
	x = panel->width / 2 - (ft_strlen(OBJECTS) * CHAR_W) / 2;
	y = OBJ_Y + PADD_Y;
	draw_string(panel->buffer, OBJECTS, x,  y, WHITE);
	x = panel->width / 2 - (ft_strlen(TRANSFORM) * CHAR_W) / 2;
	y = TRANSF_Y + PADD_Y;
	draw_string(panel->buffer, TRANSFORM, x, y, WHITE);
	x = PADD_X;
	y += ROW_H;
	draw_string(panel->buffer, TRANSLATE, x, y, LIGHT_GRAY);
	y += ROW_H + 3 * (SLIDER_H + ROW_H);
	draw_string(panel->buffer, ROTATE, x, y, LIGHT_GRAY);
	y += ROW_H + 3 * (SLIDER_H + ROW_H);
	draw_string(panel->buffer, RESIZE, x, y, LIGHT_GRAY);
}

//controls
void	render_controls(t_panel *panel, t_world *scene)
{
	fill_rectangle(panel, rectangle(0, 0, panel->width, panel->height), BLACK);
	render_sections(panel);
	render_buttons(panel);
	render_object_list(panel, scene);
	render_sliders(panel);
	mlx_put_image_to_window(scene->mlx, panel->win, panel->buffer->img, 0, 0);
}

static int	create_buffer(t_panel *panel, t_world *scene)
{
	panel->buffer = malloc(sizeof(t_data));
	if (!panel->buffer)
	{
		perror("Error\nmalloc");
		return (0);
	}
	panel->buffer->img = mlx_new_image(scene->mlx, panel->width, panel->height);
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

static int	init_panel(t_world *scene)
{
	scene->panel = malloc(sizeof(*(scene->panel)));
	if (!scene->panel)
	{
		perror("Error\nmalloc");
		return (0);
	}
	scene->panel->width = PANEL_W;
	scene->panel->height = PANEL_H;
	scene->panel->curr_obj = -1;
	scene->panel->curr_slider = -1;
	scene->panel->scroll_offset = 0;
	scene->panel->visible_objs = (TRANSF_Y - OBJ_Y - ROW_H - PADD_Y) / ROW_H;
	return (1);
}

int	setup_controls(t_world *scene)
{
	t_panel	*panel;

	if (!init_panel(scene))
		return (0);
	panel = scene->panel;
	panel->win = mlx_new_window(scene->mlx, panel->width, panel->height, "Control Panel");
	if (!panel->win)
	{
		ft_putstr_fd("Error\nFailed to create window\n", STDERR_FILENO);
		return (0);
	}
	if (!create_buffer(panel, scene))
	{
		//free panel or integrate into general cleanup function
		return (0);
	}
	render_controls(panel, scene);
	mlx_mouse_hook(panel->win, mouse_hook, scene);
	return (1);
}
