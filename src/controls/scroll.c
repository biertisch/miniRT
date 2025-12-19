/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:22:46 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/18 22:18:55 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

void	scroll_down(t_panel *panel, int object_count)
{
	int	max_offset;

	max_offset = object_count + 1 - panel->visible_objs;
	if (max_offset < 0)
		max_offset = 0;
	if (panel->scroll_offset < max_offset)
		panel->scroll_offset++;
}

void	scroll_up(t_panel *panel)
{
	if (panel->scroll_offset > 0)
		panel->scroll_offset--;
}

void	select_object(t_panel *panel, int object_count, int y)
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
