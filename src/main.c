#include "minirt.h"


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	get_color_from_img(t_data *data, int x, int y)
{
	char	*src;

	src = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)src);
}

void free_all_the_world(t_world *wld)
{
	if (wld)
	{
		// if (wld->b_w.img)
		// 	mlx_destroy_image(wld->mlx, wld->b_w.img);
		// if (wld->b_c.img)
		// 	mlx_destroy_image(wld->mlx, wld->b_c.img);
		// if (wld->b_ec.img)
		// 	mlx_destroy_image(wld->mlx, wld->b_ec.img);
		// if (wld->b_eo.img)
		// 	mlx_destroy_image(wld->mlx, wld->b_eo.img);
		// if (wld->b_s.img)
		// 	mlx_destroy_image(wld->mlx, wld->b_s.img);
		if (wld->mlx && wld->win)
			mlx_destroy_window(wld->mlx, wld->win);
		if (wld->mlx)
		{
			mlx_destroy_display(wld->mlx);
			free(wld->mlx);
		}
	}
}

int	handle_destroy(void *param)
{
	t_world *wld;

	wld = (t_world *)param;
	printf("MAIN❌\n");
	free_all_the_world(wld);
	exit(0);
	return (0);
}

int	mouse_hook(int keycode, void *param)
{
	t_world *wld;

	wld = (t_world *)param;
	(void)wld;
	printf("Hello from key_hook![%d]\n", keycode);
	return (0);
}

void	reg_hook(t_world *wld)
{
	mlx_hook(wld->win, KeyRelease, 1L << 1, handle_pressed, wld);
	mlx_hook(wld->win, DestroyNotify, 0, handle_destroy, wld);
	mlx_mouse_hook(wld->win, mouse_hook, wld);
	// mlx_loop_hook(wld->mlx, loop_hook, wld);
}

void	scene_1_checker_ground(t_world *wld)
{
	// t_material	material_ground = get_material(LAMBERTIAN,get_color(0.5,0.5,0.5),0.0);
	// t_sphere	sphere_ground = new_sphere((t_vec3){0,-1000,0}, 1000.0, material_ground);
	// add_object_to_world(wld, SPHERE, &sphere_ground);

    t_material material1 = get_material(DIELECTRIC,get_color(0,0,0),1.5);
	t_sphere	sphere1 = new_sphere((t_vec3){1,1.5,0}, 1.5, material1);
    add_object_to_world(wld, SPHERE, &sphere1);

	t_material material2 = get_material(LAMBERTIAN,get_color(0.1,0.8,0.1),0.0);
	t_sphere	sphere2 = new_sphere((t_vec3){-4,2.5,0}, 2.5, material2);
	add_object_to_world(wld, SPHERE, &sphere2);


	t_material material21 = get_material(LAMBERTIAN,get_color(0.8,0.1,0.1),0.0);
	t_sphere	sphere21 = new_sphere((t_vec3){-4,2.5,-7}, 2.5, material21);
	add_object_to_world(wld, SPHERE, &sphere21);

	t_material material22 = get_material(LAMBERTIAN,get_color(0.1,0.1,0.9),0.0);
	t_sphere	sphere22 = new_sphere((t_vec3){-4,2.5,-14}, 2.5, material22);
	add_object_to_world(wld, SPHERE, &sphere22);

	t_material material3 = get_material(METAL,get_color(0.75,0.75,0.75),0.0);
	t_sphere	sphere3 = new_sphere((t_vec3){4,1,0}, 1, material3);
    add_object_to_world(wld, SPHERE, &sphere3);

	t_sphere	sphere3_1 = new_sphere((t_vec3){4,3.3,0}, 0.3, material3);
    add_object_to_world(wld, SPHERE, &sphere3_1);
	t_sphere	sphere3_2 = new_sphere((t_vec3){4,-0.3,0}, 0.3, material3);
    add_object_to_world(wld, SPHERE, &sphere3_2);
	t_sphere	sphere3_3 = new_sphere((t_vec3){4,2.3,0}, 0.3, material3);
    add_object_to_world(wld, SPHERE, &sphere3_3);


	t_checker_texture	*checker_tex = checker_texture_colors(0.32, get_color(0.2,0.3,0.1), get_color(0.9,0.9,0.9));
	t_material	material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);

	t_sphere	sphere_ground = new_sphere((t_vec3){0,-1000,0}, 1000.0, material_checker);
	add_object_to_world(wld, SPHERE, &sphere_ground);


	wld->camera.aspect_ratio = 16.0 / 9.0;
	
	wld->camera.image_width = 200;
	wld->camera.samples_per_pixel = 5;//default 100
	wld->camera.max_depth = 5;//default 50

	wld->camera.vfov = 35.0;
	wld->camera.lookfrom = new_vec3(9,11,12);
	wld->camera.lookat = new_vec3(8.54, 10.56, 11.22);
	wld->camera.vup = new_vec3(0,1,0);

	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}
void	scene_2(t_world *wld)
{
	// t_material	mat_ground = get_material(LAMBERTIAN, get_color(0.8, 0.8, 0.0), 0.0);
	// t_material	mat_center = get_material(LAMBERTIAN, get_color(0.1, 0.2, 0.5), 0.0);
	// t_material	mat_left = get_material(DIELECTRIC, get_color(0.8, 0.8, 0.8), 1.5);
	// // t_material	mat_bubble = get_material(DIELECTRIC, get_color(0.8, 0.8, 0.8), 1.0/1.5);
	// t_material	mat_right = get_material(METAL, get_color(0.8, 0.6, 0.2), 1.0);

	// t_sphere	s_ground = new_sphere((t_vec3){0, -100.5, -1}, 100, mat_ground);
	// t_sphere	s_c = new_sphere((t_vec3){0, 0, -1.2}, 0.5, mat_center);
	// t_sphere	s_left = new_sphere((t_vec3){-1, 0, -1}, 0.5, mat_left);
	// // t_sphere	s_left_inner = new_sphere((t_vec3){-1, 0, -1}, 0.4, mat_bubble);
	// t_sphere	s_right = new_sphere((t_vec3){1, 0, -1}, 0.5, mat_right);

	// add_object(&wld, &s_ground);
	// add_object(&wld, &s_c);
	// add_object(&wld, &s_left);
	// // add_object(&wld, &s_left_inner);
	// add_object(&wld, &s_right);
	// // t_sphere	sphere3 = new_sphere((t_vec3){1, 0, -1}, 0.3);
	// // add_object(&wld, &sphere3);

	wld->camera.aspect_ratio = 16.0 / 9.0;
	
	wld->camera.image_width = 400;
	wld->camera.samples_per_pixel = 10;//default 100
	wld->camera.max_depth = 5;//default 50

	wld->camera.vfov = 90.0;
	wld->camera.lookfrom = new_vec3(-2,2,1);
	wld->camera.lookat = new_vec3(0,0,-1);
	wld->camera.vup = new_vec3(0,1,0);
}

void	scene_with_plane_and_cylinder(t_world *wld)
{
	wld->background = get_color(0.7, 0.8, 1.0);
	/*
	// Ground plane
	t_material	mat_ground = get_material(LAMBERTIAN, get_color(0.5, 0.5, 0.5), 0.0);
	t_plane		ground_plane = new_plane(new_vec3(0, -2, 0), new_vec3(0, 1, 0), mat_ground);
	add_object_to_world(wld, PLANE, &ground_plane);

	// Vertical wall plane
	t_material	mat_wall = get_material(METAL, get_color(0.8, 0.3, 0.3), 0.1);
	t_plane		wall_plane = new_plane(new_vec3(0, 0, -8), new_vec3(0, 0, 1), mat_wall);
	add_object_to_world(wld, PLANE, &wall_plane);
*/
	// Metal cylinder
	t_material	mat_cylinder = get_material(METAL, get_color(0.7, 0.7, 0.9), 0.7);
	t_cylinder	cylinder1 = new_cylinder(new_vec3(-2, -1, -3), new_vec3(0, 1, 0), 0.8, 3.0, mat_cylinder);
	add_object_to_world(wld, CYLINDER, &cylinder1);

	// Dielectric cylinder (glass-like)
	t_material	mat_glass = get_material(DIELECTRIC, get_color(0.7, 0.7, 0.9), 1.5);
	t_cylinder	cylinder2 = new_cylinder(new_vec3(2, -1, -4), new_vec3(0.3, 1, 0.1), 0.6, 2.5, mat_glass);
	add_object_to_world(wld, CYLINDER, &cylinder2);

	// // Add some spheres for comparison
	// t_material	mat_sphere1 = get_material(LAMBERTIAN, get_color(0.4, 0.8, 0.4), 0.0);
	// t_sphere	sphere1 = new_sphere(new_vec3(0, 0, -3), 0.8, mat_sphere1);
	// add_object_to_world(wld, SPHERE, &sphere1);

	// t_material	mat_sphere2 = get_material(METAL, get_color(0.9, 0.6, 0.1), 0.0);
	// t_sphere	sphere2 = new_sphere(new_vec3(-1, 1, -2), 0.5, mat_sphere2);
	// add_object_to_world(wld, SPHERE, &sphere2);

	// Camera setup
	wld->camera.aspect_ratio = 16.0 / 9.0;
	wld->camera.image_width = 400;
	wld->camera.samples_per_pixel = 20;
	wld->camera.max_depth = 10;
	wld->camera.vfov = 25.0;
	wld->camera.lookfrom = new_vec3(4, 4, 4);
	wld->camera.lookat = new_vec3(0, 0, 0);
	wld->camera.vup = new_vec3(0, 1, 0);
	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}

void	checkered_spheres(t_world *wld)
{
	t_checker_texture *checker_tex = checker_texture_colors(0.99, get_color(0.2,0.3,0.1), get_color(0.9,0.9,0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);
	t_sphere sphere1 = new_sphere((t_vec3){0,-10,0}, 10.0, material_checker);
	t_sphere sphere2 = new_sphere((t_vec3){0,10,0}, 10.0, material_checker);

	add_object_to_world(wld, SPHERE, &sphere1);
	add_object_to_world(wld, SPHERE, &sphere2);

	wld->camera.aspect_ratio = 16.0 / 9.0;
	wld->camera.image_width = 400;
	wld->camera.samples_per_pixel = 10;//default 100
	wld->camera.max_depth = 5;//default 50
	wld->camera.vfov = 20.0;
	wld->camera.lookfrom = new_vec3(13,2,3);
	wld->camera.lookat = new_vec3(0,0,0);
	wld->camera.vup = new_vec3(0,1,0);

	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}

void	scene_quad(t_world *wld)
{
	t_material	left_red = get_material(LAMBERTIAN, get_color(1.0, 0.2, 0.2), 0.0);
	t_material	back_green = get_material(LAMBERTIAN, get_color(.2, 1.0, 0.2), 0.0);
	t_material	right_blue = get_material(LAMBERTIAN, get_color(0.2, .2, 1.0), 0.0);
	t_material	upper_orange = get_material(LAMBERTIAN, get_color(1.0, 0.5, 0.0), 0.0);
	t_material	lower_teal = get_material(LAMBERTIAN, get_color(.2, .8, 0.8), 0.0);

	t_quad	quad_left = new_quad(new_vec3(-3, -2, 5), new_vec3(0, 0, -4), new_vec3(0, 4, 0), left_red);
	t_quad	quad_back = new_quad(new_vec3(-2, -2, -0), new_vec3(4, 0, 0), new_vec3(0, 4, 0), back_green);
	t_quad	quad_right = new_quad(new_vec3(3, -2, 1), new_vec3(0, 0, 4), new_vec3(0, 4, 0), right_blue);
	t_quad	quad_upper = new_quad(new_vec3(-2, 3, 1), new_vec3(4, 0, 0), new_vec3(0, 0, 4), upper_orange);
	t_quad	quad_lower = new_quad(new_vec3(-2, -3, 5), new_vec3(4, 0, 0), new_vec3(0, 0, -4), lower_teal);

	add_object_to_world(wld, QUAD, &quad_left);
	add_object_to_world(wld, QUAD, &quad_back);
	add_object_to_world(wld, QUAD, &quad_right);
	add_object_to_world(wld, QUAD, &quad_upper);
	add_object_to_world(wld, QUAD, &quad_lower);

	wld->camera.aspect_ratio = 1;
	wld->camera.image_width = 400;
	wld->camera.samples_per_pixel = 10;//default 100
	wld->camera.max_depth = 5;//default 50
	wld->camera.vfov = 80.0;
	wld->camera.lookfrom = new_vec3(0,0,10);
	wld->camera.lookat = new_vec3(0,0,0);
	wld->camera.vup = new_vec3(0,1,0);

	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}

void	simple_light_scene(t_world *wld)
{
	t_texture	*checker_tex = (t_texture *)checker_texture_colors(0.8, get_color(0.3,0.3,0.3), get_color(0.1,0.1,0.1));
	t_material	lamber_mat = get_material_texture(LAMBERTIAN, checker_tex, 0.0);

	t_sphere	ground = new_sphere((t_vec3){0,-1000,0}, 1000, lamber_mat);
	t_sphere	sphere1 = new_sphere((t_vec3){0,2,0}, 2, lamber_mat);
	add_object_to_world(wld, SPHERE, &ground);
	add_object_to_world(wld, SPHERE, &sphere1);

	// Light source material
	t_texture	*light_tex = (t_texture *)solid_color_texture_ptr(get_color(14,14,14));
	t_material	light_mat = get_material_texture(DIFFUSE_LIGHT, light_tex, 0.0);
	t_quad		light_quad = new_quad(new_vec3(3,1,-2), new_vec3(2,0,0), new_vec3(0,2,0), light_mat);
	t_sphere	light_sphere = new_sphere((t_vec3){0,7,0}, 2, light_mat);
	add_object_to_world(wld, QUAD, &light_quad);
	add_object_to_world(wld, SPHERE, &light_sphere);

	wld->camera.aspect_ratio = 16.0 / 9.0;
	wld->camera.image_width = 400;
	wld->camera.samples_per_pixel = 100;//default 100
	wld->camera.max_depth = 50;//default 50
	wld->camera.vfov = 20.0;
	wld->camera.lookfrom = new_vec3(26,3,6);
	wld->camera.lookat = new_vec3(0,2,0);
	wld->camera.vup = new_vec3(0,1,0);

	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}

void	cornel_box_scene(t_world *wld)
{
	t_material	red = get_material(LAMBERTIAN, get_color(0.65, 0.05, 0.05), 0.0);
	t_material	white = get_material(LAMBERTIAN, get_color(0.73, 0.73, 0.73), 0.0);
	t_material	green = get_material(LAMBERTIAN, get_color(0.12, 0.45, 0.15), 0.0);
	t_material	light = get_material(DIFFUSE_LIGHT, get_color(15, 15, 15), 0.0);
	t_material	glass = get_material(DIELECTRIC, get_color(.0, .0, .0), 1.5);
	t_material	metal = get_material(METAL, get_color(0.8, 0.8, 0.9), 0.0);

	t_quad	left = new_quad(new_vec3(555, 0 , 0), new_vec3(0, 555, 0), new_vec3( 0, 0, 555), green);
	t_quad	right = new_quad(new_vec3(0, 0 , 0), new_vec3(0, 555, 0), new_vec3(0, 0, 555), red);

	// t_quad	light_quad = new_quad(new_vec3(343, 554, 332), new_vec3(-230, 0, 0), new_vec3(0, 0, -205), light);
	t_quad	light_quad = new_quad(new_vec3(213,554,227), new_vec3(130, 0, 0), new_vec3(0, 0, 105), light);
	// t_quad	light_quad2 = new_quad(new_vec3(454, 478, 554), new_vec3(-180, 0, 0), new_vec3(0, -180, 0), light);
	// add_object_to_world(wld, QUAD, &light_quad2);
	t_quad	ceiling = new_quad(new_vec3(0, 0 , 0), new_vec3(555, 0, 0), new_vec3(0, 0, 555), white);
	t_quad	floor = new_quad(new_vec3(555, 555, 555), new_vec3(-555, 0, 0), new_vec3(0, 0, -555), white);
	t_quad	back = new_quad(new_vec3(0, 0, 555), new_vec3(555, 0, 0), new_vec3(0, 555, 0), metal);


	// t_material	cyl_material = get_material(LAMBERTIAN, get_color(.0, 1.0, .0), 1.5);
	t_cylinder	cyl1 = new_cylinder(new_vec3(443, 100, 432), new_vec3(0,1,0), 80, 200, red);
	t_cylinder	cyl2 = new_cylinder(new_vec3(143, 200, 232), new_vec3(0,1,0), 80, 400, green);
	add_object_to_world(wld, CYLINDER, &cyl1);
	add_object_to_world(wld, CYLINDER, &cyl2);

	t_sphere	sphere1 = new_sphere(new_vec3(290, 50, 190), 50, white);
	add_object_to_world(wld, SPHERE, &sphere1);
	t_sphere	glass_sphere = new_sphere(new_vec3(390, 120, 90), 80, glass);
	add_object_to_world(wld, SPHERE, &glass_sphere);
	t_sphere	metal_sphere = new_sphere(new_vec3(440, 300, 290), 80, metal);
	add_object_to_world(wld, SPHERE, &metal_sphere);

	add_object_to_world(wld, QUAD, &left);
	add_object_to_world(wld, QUAD, &right);
	add_object_to_world(wld, QUAD, &light_quad);
	add_object_to_world(wld, QUAD, &ceiling);
	add_object_to_world(wld, QUAD, &back);
	add_object_to_world(wld, QUAD, &floor);

	wld->camera.aspect_ratio = 1;
	wld->camera.image_width = 600;
	wld->camera.samples_per_pixel = 1;//default 100
	wld->camera.max_depth = 5;//default 50
	wld->camera.vfov = 40.0;
	wld->camera.lookfrom = new_vec3(278,278,-800);
	wld->camera.lookat = new_vec3(278,278,0);
	wld->camera.vup = new_vec3(0,1,0);

	wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
}

int main(void)
{
	t_world		wld;
	t_quad		lights;
	t_object	light_obj;

	memset(&wld, 0, sizeof(t_world));
	// wld.background = get_color(0.70, 0.80, 1.00); // Light blue background
	wld.background = get_color(0, 0, 0); // Black background

	// scene_1_checker_ground(&wld);           // Original scene with spheres
	// scene_with_plane_and_cylinder(&wld);  // New scene with planes and cylinders
	
	switch (7)
	{
		case 1:
			scene_1_checker_ground(&wld);
			break;
		case 2:
			scene_2(&wld);
			break;
		case 3:
			scene_with_plane_and_cylinder(&wld);
			break;
		case 4:
			checkered_spheres(&wld);           // Scene with checkered spheres
			break;
		case 5:
			scene_quad(&wld);                  // Scene with quadrilaterals
			break;
		case 6:
			simple_light_scene(&wld);          // Scene with simple light sources
			break;
		case 7:
			cornel_box_scene(&wld);            // Cornell box scene
			t_material	light_mat = get_material(LAMBERTIAN, get_color(15, 15, 15), 0.0);
			lights = new_quad(new_vec3(343, 554, 332), new_vec3(-130, 0, 0), new_vec3(0, 0, -105), light_mat);
			light_obj.type = QUAD;
			light_obj.geo.quad = lights;
			light_obj.hit = quad_hit;
			light_obj.pdf_value = quad_pdf_value;
			light_obj.random = quad_random;
			light_obj.bbox = lights.bbox;
			wld.lights = light_obj;
			break;
		default:
			scene_1_checker_ground(&wld);
			break;
	}

	camera_initialize(&wld.camera);
	wld.mlx = mlx_init();
	wld.win = mlx_new_window(wld.mlx, wld.camera.image_width, wld.camera.image_height, "MiniRT");
	reg_hook(&wld);
	printf("Starting render...\n");
	camera_render(&(wld.camera), &wld);
	mlx_loop(wld.mlx);
	return (0);
}
