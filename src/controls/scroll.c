/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:22:46 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 15:15:47 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

void	scroll_down(t_panel *panel)
{
	int	max_offset;

	max_offset = panel->total_count - panel->visible_objs;
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

void	select_object(t_panel *panel, int y)
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
	if (index >= panel->total_count)
		return ;
	panel->active_obj = index;
}
