/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sliders_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:36:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/14 15:46:08 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

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

int	free_sliders(t_slider **sliders, int size)
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

void	init_sliders(t_slider **sliders, int object_count)
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
