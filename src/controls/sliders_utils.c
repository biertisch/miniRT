/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sliders_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:36:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/16 10:59:40 by beatde-a         ###   ########.fr       */
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

// slider.y is defined when rendering, depending on object type
void	init_slider_type(t_slider **sliders, int object_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < object_count + 1) // local light
	{
		j = 0;
		while (j < SLIDER_COUNT)
		{
			sliders[i][j].type = j;
			sliders[i][j].x = (PANEL_W - SLIDER_W) / 2;
			sliders[i][j].knob_pos = 0.5;
			j++;
		}
		i++;
	}
}
