/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:56:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/05 20:25:32 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLS_H
# define CONTROLS_H

# include "mlx.h"
# include "libft.h"

# define CTRL_W		300
# define CTRL_H		600
# define ROW_H		20
# define SLIDER_W	240
# define SLIDER_H	14
# define KNOB_W		10
# define KNOB_H		10
# define BUTTON_W	100
# define BUTTON_H	25
# define CHAR_W		8
# define CHAR_H		8
# define WHITE		0xFFFFFF
# define LIGHT_GRAY	0xC8C8C8
# define GRAY		0x808080
# define DARK_GRAY	0x646464
# define BLACK		0x000000
# define CONTROLS	"CONTROLS"
# define OBJECTS	"Objects"
# define TRANSFORM	"Transformations"
# define TRANSLATE	"Translation"
# define ROTATE		"Rotation"
# define RESIZE		"Resizing"
# define RENDER		"Render"
# define RESET		"Reset"

typedef struct s_data t_data;
typedef struct s_world t_world;

typedef struct s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rect;

typedef struct s_panel
{
	void	*win;
	int		width;
	int		height;
	t_data	*buffer;
}	t_panel;

int	setup_controls(t_world *wld);

#endif