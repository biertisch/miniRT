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


int	has_object_was_clicked(t_world *world, t_ray *ray, t_interval ray_t, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;

	hit_anything = 0;
	closest_so_far = ray_t.max;
	int i = 0;
	while (i < world->num_objects)
	{
		current = world->objects[i];
		ray_t.max = closest_so_far;
		
		if (current->hit && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			world->current_obj = current;
		}
		i++;
	}
	return (hit_anything);
}

t_ray	get_ray_with_screenxy(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	pixel_sample = vec3_add(camera->pixel00_loc,
					vec3_add(vec3_multiply(camera->pixel_delta_u, pixel_x),
							 vec3_multiply(camera->pixel_delta_v, pixel_y)));
	ray_direction = vec3_subtract(pixel_sample, camera->lookfrom);
	return (rt_ray(camera->lookfrom, ray_direction));
}

int mouse_press(int button, int x, int y, void *param)
{
	t_world	*wld;
	t_ray	ray;
	t_hit_record	rec;

	wld = (t_world *)param;

	ray = get_ray_with_screenxy(x, y, &wld->camera);
	if(has_object_was_clicked(wld, &ray, (t_interval){0.001, INFINITY}, &rec))
	{
		if(wld->current_obj){
			printf("hit ojb type:%d\n", wld->current_obj->type);
		}else{
			printf("hit obj is NULL\n");
		}
	}
    // printf("Mouse button %d pressed at (%d, %d)\n", button, x, y);
    return (0);
}

void	reg_hook(t_world *wld)
{
	mlx_hook(wld->win, KeyRelease, 1L << 1, handle_pressed, wld);
	mlx_hook(wld->win, DestroyNotify, 0, handle_destroy, wld);
	mlx_mouse_hook(wld->win, mouse_hook, wld);
	mlx_hook(wld->win, 4, 1L << 2, mouse_press, wld);
	// mlx_loop_hook(wld->mlx, loop_hook, wld);
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
	wld->camera.samples_per_pixel = 1;
	wld->camera.max_depth = 3;
	wld->camera.vfov = 70.0;
	wld->camera.lookfrom = new_vec3(13,2,3);
	wld->camera.lookat = new_vec3(0,0,0);
	wld->camera.vup = new_vec3(0,1,0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.3;

	wld->spot_light.position = new_vec3(20, 0, 10);
	wld->spot_light.brightness = 1;
	wld->spot_light.light_color = get_color(1.0, 1.0, 1.0);
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
	// t_quad light_spot = new_quad(new_vec3(213,554,332), new_vec3(130, 0, 0), new_vec3(0, 0, -105), light);
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
	// t_sphere	metal_sphere = new_sphere(new_vec3(440, 300, 290), 80, metal);
	// add_object_to_world(wld, SPHERE, &metal_sphere);


	t_sphere	light_sphere = new_sphere(new_vec3(440, 300, 290), 10, light);
	add_object_to_world(wld, SPHERE, &light_sphere);

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

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.4;

	wld->spot_light.position = new_vec3(440, 410, 240);
	wld->spot_light.brightness = 1;
	wld->spot_light.light_color = get_color(1.0, 1.0, 1.0);
}

void	base_scene1(t_world *wld)
{
	// Implement the mandatory base scene setup here
	t_material	metal = get_material(METAL, get_color(0.8, 0.8, 0.9), 0.0);
	t_material	red = get_material(LAMBERTIAN, get_color(0.65, 0.05, 0.05), 0.0);
	t_material	green = get_material(LAMBERTIAN, get_color(0.12, 0.45, 0.15), 0.0);
	t_material	blue = get_material(METAL, get_color(0.0, 0.0, 0.9), 0.0);
	t_material	light = get_material(DIFFUSE_LIGHT, get_color(15, 15, 15), 0.0);

	t_cylinder	cyl1 = new_cylinder(new_vec3(443, 100, 432), new_vec3(0,1,0), 80, 200, red);
	t_cylinder	cyl2 = new_cylinder(new_vec3(143, 200, 232), new_vec3(0,1,0), 80, 400, green);
	t_cylinder	cyl3 = new_cylinder(new_vec3(243, 250, 532), new_vec3(0,1,0), 80, 150, blue);
	add_object_to_world(wld, CYLINDER, &cyl1);
	add_object_to_world(wld, CYLINDER, &cyl2);
	add_object_to_world(wld, CYLINDER, &cyl3);

	t_sphere	sphere1 = new_sphere(new_vec3(290, 50, 190), 50, metal);
	add_object_to_world(wld, SPHERE, &sphere1);
	
	t_sphere	light_sphere = new_sphere(new_vec3(440, 500, 400), 10, light);
	add_object_to_world(wld, SPHERE, &light_sphere);


	// Ground plane
	t_checker_texture *checker_tex = checker_texture_colors(0.011, get_color(0.2,0.3,0.1), get_color(0.9,0.9,0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);
	t_material	mat_ground = get_material(LAMBERTIAN, get_color(1, 1, 1), 0.0);
	t_plane		ground_plane = new_plane(new_vec3(0, -2, 0), new_vec3(0, 1, 0), material_checker);
	add_object_to_world(wld, PLANE, &ground_plane);

	wld->camera.aspect_ratio = 1;
	wld->camera.image_width = 600;
	wld->camera.samples_per_pixel = 1;//default 100
	wld->camera.max_depth = 5;//default 50
	wld->camera.vfov = 40.0;
	wld->camera.lookfrom = new_vec3(278,278,-800);
	wld->camera.lookat = new_vec3(278,278,0);
	wld->camera.vup = new_vec3(0,1,0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.4;
}

void	base_plane_sphere_scene(t_world *wld)
{
	// Implement the mandatory base scene setup here
	t_material	metal = get_material(METAL, get_color(0.8, 0, 0), 0.0);
	t_material	red = get_material(LAMBERTIAN, get_color(0, 0.35, 0.35), 0.0);

	t_material	c_sphere = get_material(LAMBERTIAN, get_color(1, 1, 1), 0.0);

	t_sphere	sphere1 = new_sphere(new_vec3(0, 1, 0), 1, c_sphere);
	add_object_to_world(wld, SPHERE, &sphere1);
	
	t_sphere	sphere2 = new_sphere(new_vec3(5, 1, 0), 1, metal);
	add_object_to_world(wld, SPHERE, &sphere2);

	t_cylinder	cyl = new_cylinder(new_vec3(-5, 1, 0), new_vec3(1, 1, 0), 1, 1, red);
	add_object_to_world(wld, CYLINDER, &cyl);

	// Ground plane
	t_checker_texture *checker_tex = checker_texture_colors(0.99, get_color(0.2,0.3,0.1), get_color(0.9,0.9,0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);

	t_material	mat_ground = get_material(LAMBERTIAN, get_color(1, 1, 1), 0.0);
	t_plane		ground_plane = new_plane(new_vec3(0, 0, 0), new_vec3(0, 1, 0), material_checker);
	add_object_to_world(wld, PLANE, &ground_plane);

	wld->camera.aspect_ratio = 1;
	wld->camera.image_width = 600;
	wld->camera.samples_per_pixel = 1;//default 100
	wld->camera.max_depth = 5;//default 50
	wld->camera.vfov = 50;
	wld->camera.lookfrom = new_vec3(0,2,-10);
	wld->camera.lookat = new_vec3(0,0,1);
	wld->camera.vup = new_vec3(0,1,0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);


	wld->spot_light.position = new_vec3(2, 4, -3);
	wld->spot_light.brightness = 0.8;
	wld->spot_light.light_color = get_color(1, 150/255, 200/255);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.3;
}

int main(void)
{
	t_world		wld;
	t_quad		lights;
	t_object	light_obj;

	ft_memset(&wld, 0, sizeof(t_world));
	
	switch (9)
	{
		case 4:
			checkered_spheres(&wld);           // Scene with checkered spheres
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
			wld.lights = light_obj;
			break;
		case 8:
			base_scene1(&wld);
			wld.spot_light.position = new_vec3(440, 800, 400);
			wld.spot_light.brightness = 1;
			wld.spot_light.light_color = get_color(1.0, 1.0, 1.0);

			break;
		case 9:
			base_plane_sphere_scene(&wld);
			break;
		default:
			checkered_spheres(&wld);
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
