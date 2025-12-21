/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:24:16 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 15:20:40 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

static char	*get_type_name(t_geo_type type)
{
	static char	*type_names[] = {
		"sphere", "plane", "cylinder", "bvh_node", "quad", "cone"
	};

	return (type_names[type]);
}

static void	draw_object_row(t_panel *panel, t_world *scene, int index, int y)
{
	char	name[BUFF_SIZE];
	char	*index_str;

	index_str = ft_itoa(index);
	if (!index_str)
		error_exit(scene, "Error\nmalloc");
	ft_strlcpy(name, index_str, BUFF_SIZE);
	free(index_str);
	ft_strlcat(name, " ", BUFF_SIZE);
	if (index < panel->light_count)
		ft_strlcat(name, "Light", BUFF_SIZE);
	else
		ft_strlcat(
			name,
			get_type_name(scene->objects[index - panel->light_count]->type),
			BUFF_SIZE);
	draw_string(panel->buffer, name, (t_point){PADD_X, y}, WHITE);
}

static void	draw_highlight(t_panel *panel, int row, int y)
{
	int	x;
	int	width;

	x = PADD_X - 5;
	y -= (ROW_H - CHAR_H) / 2;
	width = panel->width - 2 * x;
	outline_rectangle(panel, rectangle((t_point){x, y}, width, ROW_H), GRAY);
}

void	render_object_list(t_panel *panel, t_world *scene)
{
	int	row;
	int	obj_index;
	int	y;

	row = 0;
	while (row < panel->visible_objs)
	{
		obj_index = panel->scroll_offset + row;
		if (obj_index >= panel->total_count)
			break ;
		y = OBJ_Y + PADD_Y + ROW_H + row * ROW_H;
		if (obj_index == panel->active_obj)
			draw_highlight(panel, row, y);
		draw_object_row(panel, scene, obj_index, y);
		row++;
	}
}
