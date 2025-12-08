/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:56:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/08 11:02:29 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLS_H
# define CONTROLS_H

# include "mlx.h"
# include "libft.h"

// panel dimensions
# define PANEL_W	300
# define PANEL_H	600
# define ROW_H		20
# define SLIDER_W	240
# define SLIDER_H	14
# define KNOB_W		10
# define KNOB_H		10
# define BUTTON_W	100
# define BUTTON_H	25
# define CHAR_W		8
# define CHAR_H		8
# define OBJ_Y		40
# define TRANSF_Y	175
# define BUTTON_Y	550
# define PADD_X		20
# define PADD_Y		15

// colors
# define WHITE		0xFFFFFF
# define LIGHT_GRAY	0xC8C8C8
# define GRAY		0x808080
# define DARK_GRAY	0x646464
# define BLACK		0x000000

// titles
# define CONTROLS	"Controls"
# define OBJECTS	"Objects"
# define TRANSFORM	"Transformations"
# define TRANSLATE	"Translation (x, y, z)"
# define ROTATE		"Rotation (x, y, z)"
# define RESIZE		"Resizing (diameter, height)"
# define RESIZE2	"Resizing (diameter)"
# define RENDER		"Render"
# define RESET		"Reset"

// transformation ranges
# define TL_RANGE	20
# define RT_RANGE	90
# define RS_RANGE	20

# define BUFF_SIZE	16

typedef enum s_sliders
{
	TRANSL_X,
	TRANSL_Y,
	TRANSL_Z,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	RESIZE_D,
	RESIZE_H,
	SLIDER_COUNT
}	t_sliders;

typedef struct s_data t_data;
typedef struct s_world t_world;
typedef struct s_object t_object;

typedef struct s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
}	t_rect;

typedef struct s_slider
{
	double	base_value;
	double	knob_pos;
	int		x;
	int		y;
}	t_slider;

typedef struct s_panel
{
	void		*win;
	t_data		*buffer;
	int			width;
	int			height;
	int			active_obj;
	int			scroll_offset;
	int			visible_objs;
	int 		active_slider;
	int			dragging;
	int			drag_start_x;
	double		drag_start_norm;
	t_slider	**sliders;
}	t_panel;

int		setup_controls(t_world *scene);
void	render_controls(t_panel *panel, t_world *scene);
void	init_panel(t_panel *panel, int object_count);
int		slider_to_axis(int slider);
t_vec3	*get_object_position(t_object *obj);

#endif