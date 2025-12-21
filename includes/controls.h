/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:56:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/21 16:10:59 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLS_H
# define CONTROLS_H

# include "mlx.h"
# include "libft.h"
# include "vec3.h"
# include "minirt.h"

// panel dimensions
# define PANEL_W	400
# define PANEL_H	900
# define ROW_H		30
# define SLIDER_W	340
# define SLIDER_H	20
# define KNOB_W		16
# define KNOB_H		16
# define BUTTON_W	100
# define BUTTON_H	30
# define CHAR_W		12
# define CHAR_H		12
# define OBJ_Y		50
# define TRANSF_Y	250
# define TRANSF1_Y	300
# define TRANSF2_Y	490
# define TRANSF3_Y	680
# define BUTTON_Y	850
# define PADD_X		20
# define PADD_Y		20

// colors
# define WHITE		0xFFFFFF
# define LIGHT_GRAY	0xC8C8C8
# define GRAY		0x808080
# define BLACK		0x000000

// titles
# define CONTROLS	"Controls"
# define OBJECTS	"Objects"
# define TRANSFORM	"Transformations"
# define TRANSLATE	"Translation (x,y,z)"
# define ROTATE		"Rotation (x,y,z)"
# define RESIZE		"Resize (diameter,height)"
# define RESIZE2	"Resize (diameter)"
# define RENDER		"Render"
# define RESET		"Reset"

// transformation ranges
# define RANGE_TR	10
# define RANGE_RT	90

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
}	t_slider_type;

typedef struct s_data	t_data;
typedef struct s_world	t_world;
typedef struct s_object	t_object;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
}	t_rect;

typedef struct s_slider
{
	t_slider_type	type;
	double			base_value;
	double			initial_value;
	double			knob_pos;
	int				x;
	int				y;
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
	int			active_slider;
	int			dragging;
	int			drag_start_x;
	double		drag_start_norm;
	t_slider	**sliders;
	int			total;
	double		camera_initial_fov;
	t_vec3		camera_initial_origin;
	t_vec3		camera_initial_direction;
}	t_panel;

// bitmaps.c
int		get_letter(char c, int row);
int		get_symbol(char c, int row);

// controls.c
int		setup_controls(t_world *scene);
void	free_panel(t_world *scene, t_panel *panel);

// drag.c
void	begin_drag(t_panel *panel, int x);
int		hit_knob(t_panel *panel, int x, int y);
void	detect_active_slider(t_panel *panel, int x, int y);
void	move_knob(t_panel *panel, t_world *scene, int x);

// hooks.c
void	setup_controls_hooks(t_panel *panel, t_world *scene);
int		controls_mouse_press(int button, int x, int y, void *param);
int		controls_mouse_move(int x, int y, void *param);
int		controls_mouse_release(int button, int x, int y, void *param);
int		controls_mouse_scroll(int button, int x, int y, void *param);

// render.c
void	render_controls(t_panel *panel, t_world *scene);

// render_objects.c
void	render_object_list(t_panel *panel, t_world *scene);

// render_sliders.c
void	render_sliders(t_panel *panel, t_world *scene);

// render_text.c
void	draw_string(t_data *img, const char *s, t_point point, int color);
void	draw_char(t_data *img, char c, t_point point, int color);

// render_utils.c
t_rect	rectangle(t_point point, int width, int height);
void	fill_rectangle(t_panel *panel, t_rect rect, int color);
void	outline_rectangle(t_panel *panel, t_rect rect, int color);
void	draw_dashed_line(t_panel *panel, int y, int color);

// reset.c
void	reset_scene(t_panel *panel, t_world *scene);

// resize.c
void	apply_resize(t_slider *sliders, double *radius, double *height);

// rotation.c
void	apply_rotation(t_slider *sliders, t_vec3 *direction);

// scroll.c
void	select_object(t_panel *panel, int y);
void	scroll_up(t_panel *panel);
void	scroll_down(t_panel *panel);

// sliders_utils.c
void	init_sliders(t_slider **sliders, int size, t_world *scene);
void	reset_sliders(t_slider **sliders, int size);
int		free_sliders(t_slider ***sliders, int size);
void	get_block_start_and_end(char *header, int *start, int *end);
int		slider_to_axis(int slider);

// transform.c
void	check_button(t_world *scene, int x);
void	transform_scene(t_panel *panel, t_world *scene);

// translation.c
void	apply_translation(t_slider *sliders, t_vec3 *position);

// values.c
double	get_initial_value(t_world *scene, int index, t_slider_type slider);

#endif