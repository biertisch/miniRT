/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:58:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 15:16:54 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_panel(t_world *scene, t_panel *panel)
{
	if (!scene || !panel)
		return ;
	if (scene->mlx && panel->win)
		mlx_destroy_window(scene->mlx, panel->win);
	free_sliders(&panel->sliders, panel->total_count);
	if (panel->buffer->img)
		mlx_destroy_image(scene->mlx, panel->buffer->img);
	free(panel->buffer);
	free(panel);
}

static int	create_buffer(t_panel *panel, t_world *scene)
{
	panel->buffer = malloc(sizeof(t_data));
	if (!panel->buffer)
		error_exit(scene, "Error\nmalloc");
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

static void	init_panel(t_panel *panel, t_world *scene)
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
	panel->camera_initial_fov = scene->camera.vfov;
	panel->camera_initial_origin = scene->camera.lookfrom;
	panel->camera_initial_direction = scene->camera.forword;
	init_sliders(panel->sliders, panel->total_count, scene);
}

static void	allocate_panel(t_world *scene)
{
	int	i;

	scene->panel = malloc(sizeof(*(scene->panel)));
	if (!scene->panel)
		error_exit(scene, "Error\nmalloc");
	scene->panel->object_count = scene->num_objects;
	scene->panel->light_count = scene->num_lights;
	scene->panel->total_count = scene->num_objects + scene->num_lights;
	scene->panel->sliders = ft_calloc(sizeof(t_slider *), scene->panel->total_count);
	if (!scene->panel->sliders)
		error_exit(scene, "Error\nmalloc");
	i = 0;
	while (i < scene->panel->total_count)
	{
		scene->panel->sliders[i] = ft_calloc(sizeof(t_slider), SLIDER_COUNT);
		if (!scene->panel->sliders[i])
		{
			free_sliders(&scene->panel->sliders, i);
			error_exit(scene, "Error\nmalloc");
		}
		i++;
	}
}

int	setup_controls(t_world *scene)
{
	t_panel	*panel;

	allocate_panel(scene);
	init_panel(scene->panel, scene);
	panel = scene->panel;
	panel->win = mlx_new_window(
			scene->mlx, panel->width, panel->height, "Control Panel");
	if (!panel->win)
	{
		ft_putstr_fd("Error\nFailed to create window\n", STDERR_FILENO);
		return (0);
	}
	if (!create_buffer(panel, scene))
		return (0);
	render_controls(panel, scene);
	setup_controls_hooks(panel, scene);
	return (1);
}
