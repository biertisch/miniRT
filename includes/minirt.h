/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:01:34 by bliu              #+#    #+#             */
/*   Updated: 2025/12/30 02:15:19 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "mlx.h"
# include "libft.h"
# include "vec3.h"
# include "color.h"
# include "parser.h"
# include "controls.h"

# define WIDTH 800
# define DEPTH 5
# define RT_INFINITY 1e8
# define ROT_SPEED 0.11f
# define ESC 65307
# define ENTER 65293
# define STEP_ANGLE 1.0f
# define STEP_MOVE 1.1f
# define MAX_PITCH_ANGLE 89.0f
# define SURFACE_EPS 1e-5
# define SPECULAR_FACTOR 64.0
# define MAX_OBJS 25000
# define MAX_LIGHTS 5
# define BUMP_SCALE 0.3

# ifndef DEBUG
#  define DEBUG 0
# endif

enum
{
	KeyPress = 2,
	KeyRelease = 3,
	ButtonPress = 4,
	ButtonRelease = 5,
	MotionNotify = 6,
	EnterNotify = 7,
	LeaveNotify = 8,
	FocusIn = 9,
	FocusOut = 10,
	KeymapNotify = 11,
	Expose = 12,
	GraphicsExpose = 13,
	NoExpose = 14,
	VisibilityNotify = 15,
	CreateNotify = 16,
	DestroyNotify = 17,
	ConfigureNotify = 22
};

typedef enum e_direction
{
	FORWARD_BACKWARD,
	LEFT_RIGHT,
	UP_DOWN
}	t_direction;

typedef struct s_panel		t_panel;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_camera_auto
{
	double	angle;
	double	speed;
	double	radius;
	double	height;
	t_vec3	center;
	int		in_rot;
	int		inited;
}	t_cam_auto;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_bump_tex
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_bump_tex;

typedef enum e_tex_type
{
	SOLID_COLOR,
	PICTURE,
	CHECKER,
	BUMP_FUNC
}	t_tex_type;

//texture_begin
typedef struct s_texture	t_texture;

struct s_texture
{
	t_color			(*value)(t_texture * self, double u, double v, t_vec3 p);
	t_tex_type		type;
};

typedef struct s_soldid_color_tex
{
	t_texture	base;
	t_color		albedo;
}	t_solid_color_tex;

//use a picture as texture
typedef struct s_pic_tex
{
	t_texture	base;
	t_color		albedo;
	t_bump_tex	pic_tex;
	t_bump_tex	bump_tex;
}	t_pic_tex;

t_pic_tex			picture_texture(t_bump_tex tex);

typedef struct s_checker_texture
{
	t_texture	base;
	double		inv_scale;
	t_texture	*even;
	t_texture	*odd;
}	t_checker_tex;

t_checker_tex		checker_texture(double s, t_texture *even, t_texture *odd);

//texture_end

typedef enum e_material_type
{
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
	DIFFUSE_LIGHT
}	t_mat_type;

typedef struct s_mat_lamb
{
	t_texture	*tex;
}	t_mat_lamb;

typedef struct s_mat_metal
{
	double		fuzz;
	t_texture	*tex;
}	t_mat_metal;

typedef struct s_mat_dielect
{
	double		ref_idx;
	t_texture	*tex;
}	t_mat_dielect;

typedef struct s_diffuse_light
{
	t_texture	*tex;
}	t_diffuse_light;

typedef union u_mat_data
{
	t_mat_lamb		lamb;
	t_mat_metal		metal;
	t_mat_dielect	dielect;
	t_diffuse_light	diffuse_light;
}	t_mat_data;

typedef struct s_material	t_material;
typedef struct s_hit_record	t_hit_record;

struct s_material
{
	t_mat_type	type;
	t_mat_data	data;
	t_color		(*emitted)(t_material * self, t_ray r_in, t_hit_record * rec);
};

typedef struct s_tbn
{
	t_vec3	ct;
	t_vec3	cb;
	t_vec3	cn;
}	t_tbn;

typedef struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef struct s_onb
{
	t_vec3	axis[3];
}	t_onb;

typedef struct s_pdf		t_pdf;
struct	s_pdf
{
	double	(*value)(t_pdf *self, t_vec3 direction);
	t_vec3	(*generate)(t_pdf * self);
};

typedef struct s_roots_holder
{
	double	a;
	double	b;
	double	c;
	double	disc;
	double	sqrt_disc;
	double	rt1;
	double	rt2;
}	t_roots_holder;

typedef struct s_cosine_pdf
{
	t_pdf		base;
	t_onb		uvw;
}	t_cosine_pdf;

typedef enum e_geometry_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	BVH_NODE,
	QUAD,
	CONE,
	TRIANGLE
}	t_geo_type;

typedef struct s_quad
{
	t_vec3		q;
	t_vec3		u;
	t_vec3		v;
	t_vec3		w;
	t_material	mat;
	t_vec3		normal;
	double		d;
	double		area;
}	t_quad;

typedef struct s_plane
{
	t_vec3		point;
	t_vec3		normal;
	t_material	mat;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3		center;
	t_vec3		axis;
	double		radius;
	double		height;
	t_material	mat;
}	t_cylinder;

typedef struct s_sphere
{
	t_vec3		center;
	double		radius;
	t_material	mat;
}	t_sphere;

typedef struct s_cone
{
	t_vec3		apex;
	t_vec3		axis;
	double		radius;
	double		height;
	t_material	mat;
}	t_cone;

typedef struct s_triangle
{
	t_vec3		p1;
	t_vec3		p2;
	t_vec3		p3;
	t_vec3		normal;
	t_material	mat;
}	t_triangle;


typedef struct s_object		t_object;

typedef union u_geo_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_quad		quad;
	t_cone		cone;
	t_triangle	triangle;
}	t_geo_data;

typedef struct s_hitable_pdf
{
	t_pdf		base;
	t_object	*objects;
	t_vec3		origin;
}	t_hitable_pdf;

typedef struct s_camera
{
	double	aspect_ratio;
	int		img_w;
	int		img_h;
	t_vec3	pix00_loc;
	t_vec3	pix_delta_u;
	t_vec3	pix_delta_v;
	int		max_depth;
	double	vfov;
	t_vec3	lookfrom;
	t_vec3	forword;
	double	pitch;
	double	yaw;
	t_vec3	vup;
	t_vec3	u;
	t_vec3	v;
	t_vec3	w;
	int		initialized;
}	t_camera;

struct s_object
{
	t_geo_data		geo;
	t_geo_type		type;
	int				(*hit)(t_ray *ray, t_interval ray_t, t_object obj,
			t_hit_record *record);
	double			(*pdf_value)(t_object obj, t_vec3 origin, t_vec3 direction);
	t_vec3			(*random)(t_object obj, t_vec3 origin);
	t_tex_type		tex_type;
	struct s_object	*next;
};

struct s_hit_record
{
	t_vec3		p;
	t_vec3		normal;
	t_vec3		g_norm;
	t_material	mat;
	t_color		orig_color;
	t_ray		ray_in;
	double		t;
	double		u;
	double		v;
	int			front_face;
	int			is_d_side;
};

typedef struct s_spot_light
{
	t_vec3		position;
	double		brightness;
	t_color		color;
	int			is_skipped;
}	t_s_light;

typedef struct s_phong
{
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	// t_color		o_color;
	double		bright;
	double		cos_nl;
	double		cos_rv;
	double		atn;
}	t_phong;

typedef struct s_world
{
	void		*mlx;
	void		*win;
	t_object	*objects[MAX_OBJS];
	t_object	*bvh_root;
	int			num_objects;
	int			num_lights;
	t_camera	camera;
	t_cam_auto	camera_auto;
	t_color		ambient;
	double		ambient_ratio;
	t_s_light	*lights[MAX_LIGHTS];
	t_object	*current_obj;
	t_panel		*panel;
	t_tex_type	tex_type;
	t_pic_tex	pic_c_tex;
}	t_world;

//action_extend.c
void				extend_action(int keycode, t_world *wld);
int					loop(void *param);

//vec3.c;
double				vec3_length_squared(t_vec3 vec);
double				vec3_length(t_vec3 vec);
t_vec3				vec3_norm(t_vec3 vec);
t_vec3				unit_vector(t_vec3 vec);

//vec3_op.c
t_vec3				vec3_sub(t_vec3 a, t_vec3 b);
t_vec3				vec3_add(t_vec3 a, t_vec3 b);
t_vec3				vec3_cross(t_vec3 a, t_vec3 b);
double				vec3_dot(t_vec3 a, t_vec3 b);
t_vec3				vec3_mul_n(t_vec3 a, double scalar);

//main.c
t_world				*world(void);
void				free_all_the_world(t_world *wld);

// color.c
t_color				color(double r, double g, double b);
t_color				norm_color(t_color color);
void				write_color(t_data *img, int x, int y, t_color color);
t_color				color_add(t_color a, t_color b);
t_color				color_multi_num(t_color color, double scalar);
t_color				color_mult_color(t_color a, t_color b);
t_color				color_clamp(t_color v, double min, double max);
void				my_mlx_pixel_put(t_data *data, int x, int y, int color);
t_color				blend_colors(t_color c1, t_color c2, double t);

//debug_info.c
void				output_camera_info(t_camera *c);
t_color				normal_to_color(t_vec3 n);

//ray.c
t_ray				ray(t_vec3 origin, t_vec3 direction);
t_vec3				ray_at(t_ray *ray, double t);

//phong.c
t_color				phong_of_light(t_phong *phong, t_hit_record *rec,
						t_world *world, t_s_light light);

//hittable_list.c
void				add_object_to_world(t_world *world, t_geo_type geo_type,
						void *sphere);
int					world_hit(t_world *world, t_ray *ray, t_interval ray_t,
						t_hit_record *rec);
//hook_manager.c
void				reg_hook(t_world *wld);
int					handle_destroy(void *param);

//quad.c
t_quad				new_quad(t_vec3 Q, t_vec3 u, t_vec3 v, t_material mat);
int					quad_hit(t_ray *ray, t_interval ray_t, t_object obj,
						t_hit_record *record);

//sphere.c
int					sphere_hit(t_ray *ray, t_interval ray_t, t_object obj,
						t_hit_record *record);
t_sphere			new_sphere(t_vec3 center, double radius, t_material mat);
void				set_face_normal(t_ray *ray, t_vec3 outward_normal,
						t_hit_record *record);

//plane.c
int					plane_hit(t_ray *ray, t_interval ray_t, t_object obj,
						t_hit_record *record);
t_plane				new_plane(t_vec3 point, t_vec3 normal, t_material mat);

//trinagle.c
int					triangle_hit(t_ray *ray, t_interval ray_t, t_object obj,
						t_hit_record *record);

//cylinder.c
int					cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj,
						t_hit_record *record);
void				change_cynormal_according_bump(t_cylinder *c,
						t_hit_record *rec, int face_hit);

//cylinder_utils.c
void				cylinder_uv(t_cylinder *c, t_hit_record *rec, int face_hit);
int					cylinder_cap_check_v1(t_ray *ray, t_interval *ray_t,
						t_cylinder *cy, t_hit_record *rec);

//root_calc.c
int					calc_cylinder_side_roots(t_ray *ray, t_cylinder *cy,
						double *t1, double *t2);
int					calc_sphere_roots(t_sphere *s, t_ray *ray, t_interval ray_t,
						double *root);

//interval.c
t_interval			new_interval(double min, double max);
int					interval_surrounds(t_interval *interval, double value);
double				interval_clamp(t_interval *interval, double value);
int					interval_contains(t_interval *interval, double value);
//camera.c
// t_color	ray_color(t_ray *ray, int depth, t_world *world, t_object lights);
void				camera_light_initialize(t_world *wld);
void				camera_render(t_camera *camera, t_world *wld);
t_color				ray_color_v3(t_ray *ray, int depth, t_world *world);

//camera_utils.c
t_ray				get_ray(int pixel_x, int pixel_y, t_camera *camera);
t_vec3				choose_vup(t_vec3 forward);
t_color				metal_reflection_color(t_phong *phong, t_hit_record *rec,
						int depth, t_world *world);
int					in_shadow(t_vec3 point, t_world *world, t_vec3 light_pos);
double				attenuation(double d);

//rt_utils.c
double				degrees_to_radians(double degrees);
double				random_double(void);
double				random_double_range(double min, double max);
int					random_int(int min, int max);

//material.c
t_material			get_material(t_mat_type type, t_color albedo, double fuzz);
t_material			get_material_texture(t_mat_type type, t_texture *tex,
						double fuzz_or_refidx);

//action.c
void				do_action(int keycode, t_world *wld);

//texture_solid_color.c
t_solid_color_tex	solid_color_texture(t_color color);
t_color				solid_color_value(t_texture *texture, double u,
						double v, t_vec3 p);
t_solid_color_tex	*solid_color_texture_ptr(t_color color);

//texture_checker.c
t_checker_tex		*checker_texture_colors(double scale,
						t_color even_color, t_color odd_color);
t_color				checker_texture_value(t_texture *texture,
						double u, double v, t_vec3 p);

//texture.c
t_color				texture_value(t_texture *tex, double u, double v, t_vec3 p);
void				load_default_textures(t_world *wld);

//diffuse_light.c
t_diffuse_light		new_diffuse_light(t_texture *tex);
t_diffuse_light		new_diffuse_light_color(t_color color);
t_color				diffuse_light_emitted(t_material *self, t_ray rin,
						t_hit_record *rec);

//onb.c
t_onb				onb_new(t_vec3 n);
t_vec3				onb_transform(t_onb onb, t_vec3 v);
t_vec3				onb_u(t_onb onb);
t_vec3				onb_v(t_onb onb);
t_vec3				onb_w(t_onb onb);

//pdf.c
t_cosine_pdf		cosine_pdf_new(t_vec3 w);
t_hitable_pdf		hitable_pdf_new(t_object *obj, t_vec3 origin);

//cone.c
int					cone_hit(t_ray *ray, t_interval ray_t,
						t_object obj, t_hit_record *record);
//cone_quadratic.c
int					solve_cone_quadratic(t_ray *ray, t_cone *cone,
						double *t1, double *t2);
void				change_cone_normal_according_bump(t_cone *cone,
						t_hit_record *rec, t_tex_type tex_type);

//scene.c
int					use_test_scene(t_world *wld, int scene_no);

//mouseselect.c
int					mouse_press(int button, int x, int y, void *param);

//worldfree.c
void				free_all_the_world(t_world *wld);

//surfacechecker.c
int					is_camera_on_surface(t_camera *cam, t_world *world);

//camera_animation.c
void				init_auto_c(t_cam_auto *o, t_camera *c);
void				update_orbit_camera(t_camera *cam, t_cam_auto *o);
void				min_vl_clamp(double *value, double tob, double min);

//bump.c
t_vec3				apply_bump_f(t_tbn tbn, double u, double v,
						double (*height)(double, double));
double				sine_bump(double u, double v);
double				turbulence(double u, double v);
// void				apply_sphere_bump(t_hit_record *rec, t_sphere *sp);
// t_bump_tex			load_texture(void *mlx, char *path);

//bump_map.c
t_vec3				bump_tangent_normal(t_bump_tex *tex, double u,
						double v, double strength);
t_vec3				apply_bump_map(t_tbn tbn, t_vec3 Nt);

//bump_tbn.c
t_tbn				sphere_tbn(t_vec3 normal);
t_tbn				plane_tbn(t_vec3 normal);
t_tbn				cylinder_tbn(t_vec3 p, t_vec3 c, t_vec3 axis);
t_tbn				cone_tbn(t_vec3 p, t_cone *cone);

//texture_picture.c
t_bump_tex			load_xpm(void *mlx, char *path);
t_pic_tex			picture_texture(t_bump_tex tex);
t_color				texture_map_value(t_texture *self, double u,
						double v, t_vec3 p);

//info.c
void				print_info(t_world *wld);
void				print_help(void);

#endif