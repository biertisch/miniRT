#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "mlx.h"
# include "vec3.h"
# include "color.h"
# include "parser.h"
# include "controls.h"

# define WIDTH 400
# define HEIGHT 600
# define RT_INFINITY 1e8
# define ROT_SPEED 0.05

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
	DestroyNotify = 17
};

// typedef	struct s_film
// {
// 	int		width;
// 	int		height;
// 	int		data[WIDTH][HEIGHT];
// }	t_film;

// typedef struct s_vec2
// {
// 	double	x;
// 	double	y;
// }	t_vec2;

typedef struct s_mat3 {
    double m[3][3];
} t_mat3;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

//texture_begin
typedef struct s_texture t_texture;
// typedef struct s_texture_vtable t_texture_vtable;

// struct s_texture_vtable
// {
// 	t_color	(*value)(t_texture self, double u, double v, t_vec3 p);
// };

struct s_texture
{
	t_color	(*value)(t_texture *self, double u, double v, t_vec3 p);
};

t_color	texture_value(t_texture *self, double u, double v, t_vec3 p);

typedef struct s_soldid_color_tex
{
	t_texture	base;
	t_color		albedo;
}	t_solid_color_tex;

typedef struct s_checker_texture
{
	t_texture	base;
	double		inv_scale;
	t_texture	*even;
	t_texture	*odd;
}	t_checker_texture;

t_checker_texture	checker_texture(double scale, t_texture *even, t_texture *odd);

/*
typedef enum e_texture_type
{
	SOLID_COLOR,
	CHECKER
} t_tex_type;

typedef struct s_solid_color_texture
{
	t_color		albedo;
}	t_solid_color_texture;

typedef struct s_checker_texture
{
	double		inv_scale;
	t_solid_color_texture	even;
	t_solid_color_texture	odd;
}	t_checker_texture;

typedef union u_texture_data
{
	t_solid_color_texture	solid_color;
	t_checker_texture	checker;
}	t_texture_data;

typedef struct	s_texture
{
	t_tex_type	type;
	t_texture_data	data;
	t_color		(*value)(struct s_texture texture, double u, double v, t_vec3 p);
}  t_texture;
*/
//texture_end

typedef enum e_material_type
{
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
	DIFFUSE_LIGHT
} t_mat_type;

typedef struct s_mat_limb
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
} t_mat_data;

typedef struct s_material t_material;
typedef struct s_hit_record t_hit_record;

struct s_material
{
	t_mat_type	type;
	t_mat_data	data;
	t_color		(*emitted)(t_material *self,t_ray r_in, t_hit_record *rec, double u, double v, t_vec3 p);
	double		(*scattering_pdf)(t_ray *ray_in, t_hit_record *rec, t_ray *scattered);
};

typedef struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef	struct	s_aabb
{
	t_interval	x;
	t_interval	y;
	t_interval	z;
}	t_aabb;

typedef struct s_onb
{
	t_vec3	axis[3];
}	t_onb;


typedef	struct	s_pdf t_pdf;
struct	s_pdf
{
	double	(*value)(t_pdf *self, t_vec3 direction);
	t_vec3	(*generate)(t_pdf *self);
};

typedef struct s_phere_pdf
{
	t_pdf		base;
}	t_sphere_pdf;

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
	QUAD
} t_geo_type;

typedef struct s_quad
{
	t_vec3		Q;
	t_vec3		u;
	t_vec3		v;
	t_vec3		w;
	t_material	mat;
	t_aabb		bbox;
	t_vec3		normal;
	double		D;
	double		area;
} t_quad;

typedef struct s_plane {
    t_vec3 point;
    t_vec3 normal;
    t_material mat;
	t_aabb		bbox;
} t_plane;

typedef struct s_cylinder {
	t_vec3		center;
	t_vec3		axis;
	double		radius;
	double		height;
	t_material	mat;
	t_aabb		bbox;
} t_cylinder;

typedef struct s_sphere
{
	t_vec3		center;
	double		radius;
	t_material	mat;
	t_aabb		bbox;
}	t_sphere;

typedef struct s_object t_object;

typedef	struct s_bvh_node
{
	t_aabb		bbox;
	t_object	*left;
	t_object	*right;
}	t_bvh_node;

typedef union u_geo_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_bvh_node	bvh_node;
	t_quad		quad;
}	t_geo_data;


struct s_hit_record
{
	t_vec3	p;
	t_vec3	normal;
	t_material	mat;
	double	t;
	double	u;
	double	v;
	int		front_face;
};

typedef	struct s_hitable_pdf
{
	t_pdf		base;
	t_object	*objects;
	t_vec3		origin;
}	t_hitable_pdf;

typedef struct s_camera
{
	t_vec3	center;
	double	aspect_ratio;
	int		image_width;
	int		image_height;
	t_vec3	pixel00_loc;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	int		samples_per_pixel;
	double	pixel_samples_scale;
	int		max_depth;
	double	vfov;
	t_vec3	lookfrom;
	t_vec3	lookat;
	t_vec3	vup;
	t_vec3	u;
	t_vec3	v;
	t_vec3	w;
	int		sqrt_spp;
	double	recip_sqrt_spp;
	// t_film	*film;
}	t_camera;

// typedef struct s_scene
// {
// 	t_camera	*camera;
// 	t_sphere	*sphere;
// 	int			num_spheres;
// 	int			width;
// 	int			hight;
// }	t_scene;

struct s_object
{
	t_geo_data		geo;
	t_geo_type		type;
	int 			(*hit)(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record);
	t_aabb			bbox;
	double 			(*pdf_value)(t_object obj, t_vec3 origin, t_vec3 direction);
	t_vec3 			(*random)(t_object obj, t_vec3 origin);
	struct s_object	*next;
};

typedef struct s_world
{
	void		*mlx;
	void		*win;
	t_object	*objects[50];
	t_object	*bvh_root;
	int			num_objects;
	t_camera	camera;
	t_color		background;
	t_object	lights;
	t_panel		*panel;
}	t_world;

//vec3.c;
t_vec3	new_vec3(double x, double y, double z);
t_vec3	vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
double	vec3_length_squared(t_vec3 vec);
double	vec3_length(t_vec3 vec);
t_vec3	vec3_normalize(t_vec3 vec);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_multiply(t_vec3 a, double scalar);
t_vec3	unit_vector(t_vec3 vec);
t_vec3	random_unit_vec3();
t_vec3	random_on_hemisphere(t_vec3 normal);
int		vec3_near_zero(t_vec3 vec);
t_vec3	vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3	vec3_refract(t_vec3 uv, t_vec3 n, double etai_over_etat);
t_vec3	random_cosine_direction();


// t_camera	*new_camera(t_vec3 *origin, t_vec3 *direction, double fovy, t_film *film);

// t_scene	*new_scene(t_camera *camera, t_sphere *spheres, int num_spheres, int width, int height);

// int sphere_intersect(t_camera *cam, t_vec3 *ray, t_sphere *sphere);
// t_vplane *get_view_plane(double width, double height, double fovy);
// void ray_tracing(void *mlx, void *window, t_scene *scene);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

//main.c
void free_all_the_world(t_world *wld);

// color.c
t_color	get_color(double r, double g, double b);
void	write_color(t_data *img, int x, int y, t_color color);
t_color	color_add(t_color a, t_color b);
t_color	color_multiply_number(t_color color, double scalar);
t_color	color_multiply_vector(t_color a, t_color b);

//ray.c
t_ray	rt_ray(t_vec3 origin, t_vec3 direction);
t_vec3	ray_at(t_ray *ray, double t);

//hittable_list.c
void	add_object_to_world(t_world *world, t_geo_type geo_type, void *sphere);
int		world_hit(t_object *world, t_ray *ray, t_interval ray_t, t_hit_record *rec);

//quad.c
t_quad	new_quad(t_vec3 Q, t_vec3 u, t_vec3 v, t_material mat);
int	quad_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record);
double	quad_pdf_value(t_object obj, t_vec3 origin, t_vec3 direction);
t_vec3	quad_random(t_object obj, t_vec3 origin);

//sphere.c
int			sphere_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record);
t_sphere	new_sphere(t_vec3 center, double radius, t_material mat);
void		set_face_normal(t_ray *ray, t_vec3 outward_normal, t_hit_record *record);

//plane.c
int			plane_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record);
t_plane		new_plane(t_vec3 point, t_vec3 normal, t_material mat);

//cylinder.c
int			cylinder_hit(t_ray *ray, t_interval ray_t, t_object obj, t_hit_record *record);
t_cylinder	new_cylinder(t_vec3 center, t_vec3 axis, double radius, double height, t_material mat);

//interval.c
t_interval	new_interval(double min, double max);
int	interval_surrounds(t_interval *interval, double value);
double	interval_clamp(t_interval *interval, double value);
t_interval	interval_union(t_interval *a, t_interval *b);
double	interval_size(t_interval *interval);
t_interval	interval_expand(t_interval *interval, double delta);
int	interval_contains(t_interval *interval, double value);

//camera.c
t_color	ray_color(t_ray *ray, int depth, t_world *world, t_object lights);
void	camera_initialize(t_camera *camera);
void	camera_render(t_camera *camera, t_world *wld);

//rt_utils.c
double	degrees_to_radians(double degrees);
double	random_double(void);
double	random_double_range(double min, double max);
int		random_int(int min, int max);

//material.c
t_material get_material(t_mat_type type, t_color albedo, double fuzz);
t_material	get_material_texture(t_mat_type type, t_texture *tex, double fuzz_or_refidx);
int	lambertian_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf);
int	metal_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf);
int	dielectric_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf);

//action.c
int	handle_pressed(int keycode, void *wld);

//aabb.c
t_aabb	get_aabb(t_vec3 a, t_vec3 b);
int		aabb_hit(t_aabb *box, t_ray *ray, t_interval ray_t);
t_interval	aabb_axis_interval(t_aabb *aabb, int axis_index);
t_aabb	get_aabb_surrounding(t_aabb *a, t_aabb *b);
int	aabb_longest_axis(t_aabb *aabb);
t_aabb	aabb_empty(void);

//bvh_node.c
t_object	*bvh_from_objects(t_object **objects, int start, int end);
int		bvh_node_hit(t_ray *ray, t_interval ray_t, t_object obj,  t_hit_record *record);

//texture_solid_color.c
t_solid_color_tex	solid_color_texture(t_color color);
t_color	solid_color_value(t_texture *texture, double u, double v, t_vec3 p);
t_solid_color_tex	*solid_color_texture_ptr(t_color color);

//texture_checker.c
t_checker_texture	*checker_texture_colors(double scale, t_color even_color, t_color odd_color);
t_color	checker_texture_value(t_texture *texture, double u, double v, t_vec3 p);

//diffuse_light.c
t_diffuse_light	new_diffuse_light(t_texture *tex);
t_diffuse_light	new_diffuse_light_color(t_color color);
t_color	diffuse_light_emitted(t_material *self, t_ray rin,t_hit_record *rec, double u, double v, t_vec3 p);

//onb.c
t_onb	onb_new(t_vec3 n);
t_vec3	onb_transform(t_onb onb, t_vec3 v);
t_vec3	onb_u(t_onb onb);
t_vec3	onb_v(t_onb onb);
t_vec3	onb_w(t_onb onb);

//pdf.c
t_cosine_pdf	cosine_pdf_new(t_vec3 w);
t_hitable_pdf	hitable_pdf_new(t_object *obj, t_vec3 origin);

#endif