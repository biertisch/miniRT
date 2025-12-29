/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sliders_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:36:30 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/28 12:37:03 by beatde-a         ###   ########.fr       */
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

void	get_block_start_and_end(char *header, int *start, int *end)
{
	if (ft_strcmp(header, TRANSLATE) == 0)
	{
		*start = TRANSL_X;
		*end = TRANSL_Z;
	}
	else if (ft_strcmp(header, ROTATE) == 0)
	{
		*start = ROTATE_X;
		*end = ROTATE_Z;
	}
	else if (ft_strcmp(header, RESIZE) == 0)
	{
		*start = RESIZE_D;
		*end = RESIZE_H;
	}
	else if (ft_strcmp(header, RESIZE2) == 0)
	{
		*start = RESIZE_D;
		*end = RESIZE_D;
	}
}

int	free_sliders(t_slider ***sliders, int size)
{
	int	i;

	if (!sliders || !*sliders)
		return (0);
	i = 0;
	while (i < size)
	{
		free((*sliders)[i]);
		i++;
	}
	free(*sliders);
	*sliders = NULL;
	return (0);
}

void	reset_sliders(t_slider **sliders, int size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < SLIDER_COUNT)
		{
			if (j == ROTATE_X)
				sliders[i][j].rotation = mat3_identity();
			sliders[i][j].base_value = sliders[i][j].initial_value;
			sliders[i][j].knob_pos = 0.5;
			j++;
		}
		i++;
	}
}

// slider.y is defined when rendering, depending on object type
void	init_sliders(t_slider **sliders, int size, t_world *scene)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < SLIDER_COUNT)
		{
			sliders[i][j].type = j;
			sliders[i][j].initial_value = get_initial_value(scene, i, j);
			sliders[i][j].base_value = sliders[i][j].initial_value;
			if (j == ROTATE_X)
				sliders[i][j].rotation = mat3_identity();
			sliders[i][j].x = (PANEL_W - SLIDER_W) / 2;
			sliders[i][j].knob_pos = 0.5;
			j++;
		}
		i++;
	}
}
