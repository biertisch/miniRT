#include "minirt.h"

void checkered_spheres(t_world *wld)
{
	t_checker_texture *checker_tex = checker_texture_colors(0.99, get_color(0.2, 0.3, 0.1), get_color(0.9, 0.9, 0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);
	t_sphere sphere1 = new_sphere((t_vec3){0, -10, 0}, 10.0, material_checker);
	t_sphere sphere2 = new_sphere((t_vec3){0, 10, 0}, 10.0, material_checker);

	add_object_to_world(wld, SPHERE, &sphere1);
	add_object_to_world(wld, SPHERE, &sphere2);

	wld->camera.aspect_ratio = 16.0 / 9.0;
	wld->camera.img_w = 400;
	// wld->camera.samples_per_pixel = 1;
	wld->camera.max_depth = 3;
	wld->camera.vfov = 70.0;
	wld->camera.lookfrom = new_vec3(13, 2, 3);
	wld->camera.forword = new_vec3(0, 0, 1);
	wld->camera.vup = new_vec3(0, 1, 0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.3;

	wld->spot_light.position = new_vec3(20, 0, 10);
	wld->spot_light.brightness = 1;
	wld->spot_light.light_color = get_color(1.0, 1.0, 1.0);
}

void cornel_box_scene(t_world *wld)
{
	t_material red = get_material(LAMBERTIAN, get_color(0.65, 0.05, 0.05), 0.0);
	t_material white = get_material(LAMBERTIAN, get_color(0.73, 0.73, 0.73), 0.0);
	t_material green = get_material(LAMBERTIAN, get_color(0.12, 0.45, 0.15), 0.0);
	t_material light = get_material(DIFFUSE_LIGHT, get_color(15, 15, 15), 0.0);
	t_material glass = get_material(DIELECTRIC, get_color(.0, .0, .0), 1.5);
	t_material metal = get_material(METAL, get_color(0.8, 0.8, 0.9), 0.0);

	t_quad left = new_quad(new_vec3(555, 0, 0), new_vec3(0, 555, 0), new_vec3(0, 0, 555), green);
	t_quad right = new_quad(new_vec3(0, 0, 0), new_vec3(0, 555, 0), new_vec3(0, 0, 555), red);

	// t_quad	light_quad = new_quad(new_vec3(343, 554, 332), new_vec3(-230, 0, 0), new_vec3(0, 0, -205), light);
	t_quad light_quad = new_quad(new_vec3(213, 554, 227), new_vec3(130, 0, 0), new_vec3(0, 0, 105), light);
	// t_quad light_spot = new_quad(new_vec3(213,554,332), new_vec3(130, 0, 0), new_vec3(0, 0, -105), light);
	// t_quad	light_quad2 = new_quad(new_vec3(454, 478, 554), new_vec3(-180, 0, 0), new_vec3(0, -180, 0), light);
	// add_object_to_world(wld, QUAD, &light_quad2);
	t_quad ceiling = new_quad(new_vec3(0, 0, 0), new_vec3(555, 0, 0), new_vec3(0, 0, 555), white);
	t_quad floor = new_quad(new_vec3(555, 555, 555), new_vec3(-555, 0, 0), new_vec3(0, 0, -555), white);
	t_quad back = new_quad(new_vec3(0, 0, 555), new_vec3(555, 0, 0), new_vec3(0, 555, 0), metal);

	// t_material	cyl_material = get_material(LAMBERTIAN, get_color(.0, 1.0, .0), 1.5);
	t_cylinder cyl1 = new_cylinder(new_vec3(443, 100, 432), new_vec3(0, 1, 0), 80, 200, red);
	t_cylinder cyl2 = new_cylinder(new_vec3(143, 200, 232), new_vec3(0, 1, 0), 80, 400, green);
	add_object_to_world(wld, CYLINDER, &cyl1);
	add_object_to_world(wld, CYLINDER, &cyl2);

	t_sphere sphere1 = new_sphere(new_vec3(290, 50, 190), 50, white);
	add_object_to_world(wld, SPHERE, &sphere1);
	t_sphere glass_sphere = new_sphere(new_vec3(390, 120, 90), 80, glass);
	add_object_to_world(wld, SPHERE, &glass_sphere);
	// t_sphere	metal_sphere = new_sphere(new_vec3(440, 300, 290), 80, metal);
	// add_object_to_world(wld, SPHERE, &metal_sphere);

	t_sphere light_sphere = new_sphere(new_vec3(440, 300, 290), 10, light);
	add_object_to_world(wld, SPHERE, &light_sphere);

	add_object_to_world(wld, QUAD, &left);
	add_object_to_world(wld, QUAD, &right);
	add_object_to_world(wld, QUAD, &light_quad);
	add_object_to_world(wld, QUAD, &ceiling);
	add_object_to_world(wld, QUAD, &back);
	add_object_to_world(wld, QUAD, &floor);

	wld->camera.aspect_ratio = 1;
	wld->camera.img_w = 600;
	// wld->camera.samples_per_pixel = 1;//default 100
	wld->camera.max_depth = 5; // default 50
	wld->camera.vfov = 40.0;
	wld->camera.lookfrom = new_vec3(278, 278, -800);
	wld->camera.forword = vec3_norm(vec3_sub(new_vec3(278, 278, 0), wld->camera.lookfrom));
	wld->camera.vup = new_vec3(0, 1, 0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.4;

	wld->spot_light.position = new_vec3(440, 410, 240);
	wld->spot_light.brightness = 1;
	wld->spot_light.light_color = get_color(1.0, 1.0, 1.0);
}

void base_scene1(t_world *wld)
{
	// Implement the mandatory base scene setup here
	t_material metal = get_material(METAL, get_color(0.8, 0.8, 0.9), 0.0);
	t_material red = get_material(LAMBERTIAN, get_color(0.65, 0.05, 0.05), 0.0);
	t_material green = get_material(LAMBERTIAN, get_color(0.12, 0.45, 0.15), 0.0);
	t_material blue = get_material(METAL, get_color(0.0, 0.0, 0.9), 0.0);
	t_material light = get_material(DIFFUSE_LIGHT, get_color(15, 15, 15), 0.0);

	t_cylinder cyl1 = new_cylinder(new_vec3(443, 100, 432), new_vec3(0, 1, 0), 80, 200, red);
	t_cylinder cyl2 = new_cylinder(new_vec3(143, 200, 232), new_vec3(0, 1, 0), 80, 400, green);
	t_cylinder cyl3 = new_cylinder(new_vec3(243, 250, 532), new_vec3(0, 1, 0), 80, 150, blue);
	add_object_to_world(wld, CYLINDER, &cyl1);
	add_object_to_world(wld, CYLINDER, &cyl2);
	add_object_to_world(wld, CYLINDER, &cyl3);

	t_sphere sphere1 = new_sphere(new_vec3(290, 50, 190), 50, metal);
	add_object_to_world(wld, SPHERE, &sphere1);

	t_sphere light_sphere = new_sphere(new_vec3(440, 500, 400), 10, light);
	add_object_to_world(wld, SPHERE, &light_sphere);

	// Ground plane
	t_checker_texture *checker_tex = checker_texture_colors(0.011, get_color(0.2, 0.3, 0.1), get_color(0.9, 0.9, 0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);
	t_material mat_ground = get_material(LAMBERTIAN, get_color(1, 1, 1), 0.0);
	t_plane ground_plane = new_plane(new_vec3(0, -2, 0), new_vec3(0, 1, 0), material_checker);
	add_object_to_world(wld, PLANE, &ground_plane);

	wld->camera.aspect_ratio = 1;
	wld->camera.img_w = 1024;
	// wld->camera.samples_per_pixel = 1;//default 100
	wld->camera.max_depth = 5; // default 50
	wld->camera.vfov = 40.0;
	wld->camera.lookfrom = new_vec3(278, 278, -800);
	wld->camera.forword = vec3_norm(vec3_sub(new_vec3(278, 278, 0), wld->camera.lookfrom));
	wld->camera.vup = new_vec3(0, 1, 0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);

	wld->ambient = get_color(1, 1, 1);
	wld->ambient_ratio = 0.4;
}

void base_plane_sphere_scene(t_world *wld)
{
	// Implement the mandatory base scene setup here
	t_material metal = get_material(METAL, get_color(255, 255 * .85, 255 * .31), 0.0);
	t_material red = get_material(LAMBERTIAN, get_color(0, 255 * 0.35, 255 * 0.35), 0.0);
	t_material c_sphere = get_material(LAMBERTIAN, get_color(255, 255, 255), 0.0);

	t_sphere sphere1 = new_sphere(new_vec3(0, 1, 0), 1, c_sphere);
	add_object_to_world(wld, SPHERE, &sphere1);

	t_sphere sphere2 = new_sphere(new_vec3(5, 1, 0), 1, metal);
	add_object_to_world(wld, SPHERE, &sphere2);

	t_cylinder cyl = new_cylinder(new_vec3(-5, 1, 0), new_vec3(1, 1, 0), 1, 1, red);
	add_object_to_world(wld, CYLINDER, &cyl);

	// Ground plane
	t_checker_texture *checker_tex = checker_texture_colors(0.99, get_color(0.2, 0.3, 0.1), get_color(0.9, 0.9, 0.9));
	t_material material_checker = get_material_texture(LAMBERTIAN, (t_texture *)checker_tex, 0.0);

	t_sphere checker_sphere1 = new_sphere(new_vec3(0, 3, 0), 1, material_checker);
	add_object_to_world(wld, SPHERE, &checker_sphere1);

	t_material mat_ground = get_material(LAMBERTIAN, get_color(1, 1, 1), 0.0);
	t_plane ground_plane = new_plane(new_vec3(0, 0, 0), new_vec3(0, 1, 0), material_checker);
	add_object_to_world(wld, PLANE, &ground_plane);

	wld->camera.vfov = 30;
	wld->camera.lookfrom = new_vec3(0, 2, -10);
	wld->camera.forword = vec3_norm(vec3_sub(new_vec3(0, 0, 1), wld->camera.lookfrom));
	wld->camera.vup = new_vec3(0, 1, 0);

	// wld->bvh_root = bvh_from_objects(wld->objects, 0, wld->num_objects);
	t_sphere sphere1_lightpos = new_sphere(new_vec3(2, 5, -3), .5, c_sphere);
	add_object_to_world(wld, SPHERE, &sphere1_lightpos);
	wld->spot_light.position = new_vec3(2, 4, -3);
	wld->spot_light.brightness = 0.8;
	wld->spot_light.light_color = get_color(255, 255, 255);
	wld->ambient = get_color(255, 255, 255);
	wld->ambient_ratio = 0.3;
}

int	use_test_scene(t_world *wld, int scene_id)
{
	switch (scene_id)
	{
		case 4:
			checkered_spheres(wld);
			return (1);
			break;
		case 8:
			base_scene1(wld);
			return (1);
			break;
		case 9:
			base_plane_sphere_scene(wld);
			return (1);
			break;
		default:
			return (0);
			break;
	}
}
