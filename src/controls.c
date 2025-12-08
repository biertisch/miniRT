/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:58:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/08 11:52:25 by beatde-a         ###   ########.fr       */
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

	if (!s || !img)
		return ;
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

//transformations
static double	commit_slider(t_slider *slider, int range)
{
	double	value;

	value = slider->base_value + (slider->knob_pos - 0.5) * range;
	slider->base_value = value;
	slider->knob_pos = 0.5;
	return (value);
}

static void	transform_objects(t_slider **sliders, t_object **objects, int object_count)
{
	int	i;

	i = 0;
	while (i < object_count)
	{
		if (objects[i]->type == CYLINDER)
		{
			objects[i]->geo.cylinder.center.x = commit_slider(&sliders[i][TRANSL_X], TL_RANGE);
			objects[i]->geo.cylinder.center.y = commit_slider(&sliders[i][TRANSL_Y], TL_RANGE);
			objects[i]->geo.cylinder.center.z = commit_slider(&sliders[i][TRANSL_Z], TL_RANGE);
			// add rotation
			objects[i]->geo.cylinder.radius = commit_slider(&sliders[i][RESIZE_D], RS_RANGE) / 2;
			objects[i]->geo.cylinder.height = commit_slider(&sliders[i][RESIZE_H], RS_RANGE);
		}
		else if (objects[i]->type == SPHERE)
		{
			objects[i]->geo.sphere.center.x = commit_slider(&sliders[i][TRANSL_X], TL_RANGE);
			objects[i]->geo.sphere.center.y = commit_slider(&sliders[i][TRANSL_Y], TL_RANGE);
			objects[i]->geo.sphere.center.z = commit_slider(&sliders[i][TRANSL_Z], TL_RANGE);
			// add rotation
			objects[i]->geo.sphere.radius = commit_slider(&sliders[i][RESIZE_D], RS_RANGE) / 2;
		}
		else if (objects[i]->type == PLANE)
		{
			objects[i]->geo.plane.point.x = commit_slider(&sliders[i][TRANSL_X], TL_RANGE);
			objects[i]->geo.plane.point.y = commit_slider(&sliders[i][TRANSL_Y], TL_RANGE);
			objects[i]->geo.plane.point.z = commit_slider(&sliders[i][TRANSL_Z], TL_RANGE);
			// add rotation
		}
		i++;
	}
}

static void	transform_camera(t_slider *sliders, t_camera *camera)
{
	camera->center.x = sliders[TRANSL_X].base_value + (sliders[TRANSL_X].knob_pos - 0.5) * TL_RANGE;
	camera->center.y = sliders[TRANSL_Y].base_value + (sliders[TRANSL_Y].knob_pos - 0.5) * TL_RANGE;
	camera->center.z = sliders[TRANSL_Z].base_value + (sliders[TRANSL_Z].knob_pos - 0.5) * TL_RANGE;
	sliders[TRANSL_X].base_value = camera->center.x;
	sliders[TRANSL_X].knob_pos = 0.5;
	sliders[TRANSL_Y].base_value = camera->center.y;
	sliders[TRANSL_Y].knob_pos = 0.5;
	sliders[TRANSL_Z].base_value = camera->center.z;
	sliders[TRANSL_Z].knob_pos = 0.5;
	// add rotation
	// do I need camera_initialize?
}

static void	render_scene(t_panel *panel, t_world *scene)
{
	transform_camera(panel->sliders[0], &scene->camera);
	//transform light
	transform_objects(panel->sliders + 2, scene->objects, scene->num_objects);
	camera_render(&scene->camera, scene);
	render_controls(panel, scene);
}

//sliders
static void	move_knob(t_panel *panel, t_world *scene, int x)
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

static void	detect_active_slider(t_panel *panel, int x, int y)
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

static int	hit_knob(t_panel *panel, int x, int y)
{
	int	i;
	int	j;
	int	knob_x;

	i = panel->active_obj;
	j = panel->active_slider;
	if (i < 0 || j < 0 || j >= SLIDER_COUNT)
		return (0);
	knob_x = panel->sliders[i][j].x + 2 + panel->sliders[i][j].knob_pos * (SLIDER_W - KNOB_W - 4);
	return ((x >= knob_x
		&& x <= knob_x + KNOB_W
		&& y >= panel->sliders[i][j].y + 2
		&& y <= panel->sliders[i][j].y + 2 + KNOB_H));
}

static void	begin_drag(t_panel *panel, int x)
{
	if (panel->active_obj < 0 || panel->active_slider < 0)
		return ;
	panel->dragging = 1;
	panel->drag_start_x = x;
	panel->drag_start_norm = panel->sliders[panel->active_obj][panel->active_slider].knob_pos;
}

//scroll & select
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
	panel->active_obj = index;
}

//hooks
static int	mouse_release_hook(int button, int x, int y, void *param)
{
	t_panel	*panel;

	panel = (t_panel *)param;
	if (button == 1)
	{
		panel->dragging = 0;
		panel->active_slider = -1;
	}
	return (0);
}

static int	mouse_move_hook(int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	detect_active_slider(scene->panel, x, y);
	if (scene->panel->dragging)
		move_knob(scene->panel, scene, x);
	return (0);
}

static int	mouse_hook(int button, int x, int y, void *param)
{
	t_world	*scene;

	scene = (t_world *)param;
	if (button == 1)
	{
		detect_active_slider(scene->panel, x, y);
		if (scene->panel->active_obj != -1 && scene->panel->active_slider != -1 && hit_knob(scene->panel, x, y))
			begin_drag(scene->panel, x);
		else if (y >= OBJ_Y && y < TRANSF_Y)
			select_object(scene->panel, scene->num_objects, y);
		else if (y >= BUTTON_Y + PADD_Y && y < BUTTON_Y + PADD_Y + BUTTON_H)
		{
			if (x >= PADD_X && x < PADD_X + BUTTON_W)
				render_scene(scene->panel, scene);
			else if (x >= PANEL_W - PADD_X - BUTTON_W && x < PANEL_W - PADD_X)
				init_panel(scene->panel, scene->num_objects);
		}
	}
	else if (button == 4 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_up(scene->panel);
	else if (button == 5 && (y >= OBJ_Y && y < TRANSF_Y))
		scroll_down(scene->panel, scene->num_objects);
	render_controls(scene->panel, scene);
	return (0);
}

//object_list
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
		if (obj_index == panel->active_obj)
			draw_highlight(panel, row, y);
		draw_object_row(panel, scene, obj_index, y);
		row++;
	}
}

//sliders
int	slider_to_axis(int slider)
{
	if (slider == TRANSL_X || slider == ROTATE_X)
		return (0);
	if (slider == TRANSL_Y || slider == ROTATE_Y)
		return (1);
	if (slider == TRANSL_Z || slider == ROTATE_Z)
		return (2);
	return (-1);
}

t_vec3	*get_object_position(t_object *obj)
{
	if (obj->type == PLANE)
		return (&obj->geo.plane.point);
	if (obj->type == SPHERE)
		return (&obj->geo.sphere.center);
	if (obj->type == CYLINDER)
		return (&obj->geo.cylinder.center);
	return (NULL);
}

static double	get_object_value(t_object *obj, int slider)
{
	t_vec3	*pos;
	int		axis;

	axis = slider_to_axis(slider);
	if (axis != -1)
	{
		pos = get_object_position(obj);
		if (!pos)
			return (0);
		return (((double *)pos)[axis]);
	}
	if (slider == RESIZE_D)
	{
		if (obj->type == SPHERE)
			return (obj->geo.sphere.radius * 2);
		if (obj->type == CYLINDER)
			return (obj->geo.cylinder.radius * 2);
	}
	if (slider == RESIZE_H && obj->type == CYLINDER)
		return (obj->geo.cylinder.height);
	return (0);
}

static double	get_light_value(t_world *scene, int slider)
{
	int	axis;

	axis = slider_to_axis(slider);
	if (axis == -1)
		return (0);
	return (0); // update when light struct is updated
}

static double	get_camera_value(t_world *scene, int slider)
{
	int	axis;

	axis = slider_to_axis(slider);
	if (axis == -1)
		return (0);
	return (((double *)&scene->camera.center)[axis]);
}

static double	get_base_value(t_world *scene, int index, int slider)
{
	if (index < 0)
		return (0);
	if (index == 0)
		return (get_camera_value(scene, slider));
	if (index == 1)
		return round(get_light_value(scene, slider));
	return (get_object_value(scene->objects[index - 2], slider));
	return (0);
}

static void draw_value(t_panel *panel, int value, int y, int pos)
{
	char	*str;
	int		x;

	str = ft_itoa(value);
	if (!str) // issue warning?
		return ;
	if (pos == 0)
		x = PANEL_W / 2 - (ft_strlen(str) * CHAR_W) / 2;
	else if (pos == -1)
		x = PADD_X;
	else
		x = PANEL_W - PADD_X - ft_strlen(str) * CHAR_W;
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
	draw_value(panel, min, y, -1);
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

	draw_string(panel->buffer, TRANSLATE, x,  y, LIGHT_GRAY);
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

static void	render_sliders(t_panel *panel, t_world *scene)
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

//fixed: sections and buttons
static void	render_buttons(t_panel *panel)
{
	int	x;
	int	y;

	y = BUTTON_Y + PADD_Y;
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
}

//controls
void	render_controls(t_panel *panel, t_world *scene)
{
	fill_rectangle(panel, rectangle(0, 0, panel->width, panel->height), BLACK);
	render_sections(panel);
	render_buttons(panel);
	render_object_list(panel, scene);
	render_sliders(panel, scene);
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

static int	free_sliders(t_slider **sliders, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(sliders[i]);
		i++;
	}
	free(sliders);
	return (0);
}

static void	init_sliders(t_slider **sliders, int object_count)
{
	int	i;
	int	j;
	int	y;

	i = 0;
	while (i < object_count)
	{
		y = TRANSF_Y + 2 * ROW_H + PADD_Y;
		j = 0;
		while (j < SLIDER_COUNT)
		{
			sliders[i][j].x = (PANEL_W - SLIDER_W) / 2;
			sliders[i][j].y = y;
			sliders[i][j].knob_pos = 0.5;
			y += SLIDER_H + ROW_H;
			if (j == TRANSL_Z || j == ROTATE_Z)
				y += ROW_H + 5;
			j++;
		}
		i++;
	}
}

void	init_panel(t_panel *panel, int object_count)
{
	panel->width = PANEL_W;
	panel->height = PANEL_H;
	panel->active_obj = -1;
	panel->scroll_offset = 0;
	panel->visible_objs = (TRANSF_Y - OBJ_Y - ROW_H - PADD_Y) / ROW_H;
	panel->active_slider = -1;
	panel->dragging = 0;
	panel->drag_start_x = PANEL_W / 2;
	panel->drag_start_norm = 0.5;
	init_sliders(panel->sliders, object_count);
}

int	allocate_panel(t_world *scene)
{
	int	i;

	scene->panel = malloc(sizeof(*(scene->panel)));
	if (!scene->panel)
	{
		perror("Error\nmalloc");
		return (0);
	}
	scene->panel->sliders = malloc(sizeof(t_slider *) * scene->num_objects);
	if (!scene->panel->sliders)
	{
		perror("Error\nmalloc");
		return (0);
	}
	i = 0;
	while (i < scene->num_objects)
	{
		scene->panel->sliders[i] = malloc(sizeof(t_slider) * SLIDER_COUNT);
		if (!scene->panel->sliders[i])
		{
			perror("Error\nmalloc");
			return (free_sliders(scene->panel->sliders, i));
		}
		i++;
	}
	return (1);
}

int	setup_controls(t_world *scene)
{
	t_panel	*panel;

	if (!allocate_panel(scene))
		return (0);
	init_panel(scene->panel, scene->num_objects);
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
	mlx_hook(panel->win, 6, 1L << 6, mouse_move_hook, scene);
	mlx_hook(panel->win, 5, 1L << 3, mouse_release_hook, panel);
	mlx_mouse_hook(panel->win, mouse_hook, scene);
	return (1);
}
