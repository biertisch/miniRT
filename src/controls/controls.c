/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:58:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/16 10:58:46 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// free panel

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
	panel->buffer->addr = mlx_get_data_addr(
			panel->buffer->img, &panel->buffer->bits_per_pixel,
			&panel->buffer->line_length, &panel->buffer->endian);
	if (!panel->buffer->addr)
	{
		ft_putstr_fd("Error\nFailed to access image buffer\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

static void	init_panel(t_panel *panel, int object_count)
{
	panel->width = PANEL_W;
	panel->height = PANEL_H;
	panel->active_obj = -1;
	panel->scroll_offset = 0;
	panel->visible_objs = (TRANSF_Y - OBJ_Y - ROW_H) / ROW_H;
	panel->active_slider = -1;
	panel->dragging = 0;
	panel->drag_start_x = PANEL_W / 2;
	panel->drag_start_norm = 0.5;
	init_slider_type(panel->sliders, object_count);
}

static int	allocate_panel(t_world *scene)
{
	int	i;

	scene->panel = malloc(sizeof(*(scene->panel)));
	if (!scene->panel)
		return (perror("Error\nmalloc"), 0);
	scene->panel->sliders = malloc(sizeof(t_slider *) * scene->num_objects + 1); // local light
	if (!scene->panel->sliders)
		return (perror("Error\nmalloc"), 0);
	i = 0;
	while (i < scene->num_objects + 1) // local light
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
	panel->win = mlx_new_window(
			scene->mlx, panel->width, panel->height, "Control Panel");
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
	mlx_hook(panel->win, 6, 1L << 6, controls_mouse_move_hook, scene);
	mlx_hook(panel->win, 5, 1L << 3, controls_mouse_release_hook, panel);
	mlx_mouse_hook(panel->win, controls_mouse_hook, scene);
	return (1);
}
