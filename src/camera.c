#include "minirt.h"

t_color	blend_colors(t_color c1, t_color c2, double t)
{
	t_color blended;

	blended.r = (1 - t) * c1.r + t * c2.r;
	blended.g = (1 - t) * c1.g + t * c2.g;
	blended.b = (1 - t) * c1.b + t * c2.b;
	return (blended);
}

double compute_attenuation(double d)
{
    double constant = 1.0;
    double linear = 0.1;
    double quadratic = 0.01;

    return 1.0 / (constant + linear * d + quadratic * d * d);
}

t_color ray_color_new(t_ray *ray, int depth, t_world *world)
{
	t_hit_record	rec;
	t_hit_record	temp_rec;
	t_ray			scattered;
	t_ray			r_2light;
	t_color			ambient;
	t_color			color_from_emission;
	double			cos_nl;
	t_color			diffuse;

	if (depth <= 0)
		return (get_color(0.0, 0.0, 0.0));

	ambient = color_multiply_number(world->ambient, world->ambient_ratio);
	ambient = color_multiply_vector(ambient, world->spot_light.light_color);

	if (world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec, rec.u, rec.v, rec.p);
		// printf("Emitted color: R=%f, G=%f, B=%f\n", color_from_emission.r, color_from_emission.g, color_from_emission.b);
		r_2light = rt_ray(rec.p, vec3_subtract(world->spot_light.position, rec.p));
		
		
		if (world_hit(world->bvh_root, &r_2light, new_interval(0.001, RT_INFINITY), &temp_rec))
		{
			// if (rec.mat.type == METAL || rec.mat.type == DIELECTRIC)
			// 	return (color_multiply_vector(color_from_emission, ray_color_new(&scattered, depth - 1, world)));
			// else
				return color_multiply_vector(world->ambient,color_multiply_number(color_from_emission, world->ambient_ratio));
		}
		else
		{
			cos_nl = vec3_dot(rec.normal,vec3_normalize(r_2light.direction));
			if (cos_nl < 0)
				cos_nl = 0;
			
			diffuse = color_multiply_vector(color_from_emission, world->spot_light.light_color);
			diffuse = color_multiply_number(diffuse, (1 + world->ambient_ratio));
			// if (rec.mat.type == METAL || rec.mat.type == DIELECTRIC)
			// 	return (color_multiply_vector(color_from_emission, ray_color_new(&scattered, depth - 1, world)));
			// else
			// diffuse = color_multiply_number(diffuse, compute_attenuation(vec3_length(vec3_subtract(world->spot_light.position, rec.p))));
				return color_add(diffuse,color_multiply_vector(world->ambient,color_multiply_number(color_from_emission, world->ambient_ratio)));
			// diffuse = color_multiply_number(diffuse, cos_nl * compute_attenuation(vec3_length(vec3_subtract(world->spot_light.position, rec.p))));
			// // printf("Diffuse color: R=%f, G=%f, B=%f\n", diffuse.r, diffuse.g, diffuse.b);
			// return (color_add(ambient, diffuse));
			// if (cos_nl < 0)
			// 	return (ambient);
			// else
			// 	return (color_add(color_from_emission, color_multiply_number(world->spot_light.light_color, cos_nl * world->spot_light.brightness)));
		}
	}
	else
		return (ambient);
}

t_color	ray_color(t_ray *ray, int depth, t_world *world, t_object lights)
{
	t_hit_record	rec;
	// t_vec3			unit_direction;
	t_ray			scattered;
	t_color			attenuation;
	double			pdf_value;
	t_color			color_from_emission;
	t_color			color_from_scatter;

	if (depth <= 0)
		return (get_color(0.0, 0.0, 0.0));
	// ray_t = new_interval(0.001, RT_INFINITY);
	/*
	if (world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		t_ray	scattered;
		t_color	attenuation;
		if (rec.mat.type == LAMBERTIAN)
		{
			if (lambertian_scatter(ray, &rec, &attenuation, &scattered))
			{
				return (color_multiply_vector(ray_color(&scattered, depth - 1, world), attenuation));
			}
		}
		else if (rec.mat.type == METAL)
		{
			if (metal_scatter(ray, &rec, &attenuation, &scattered))
			{
				return (color_multiply_vector(ray_color(&scattered, depth - 1, world), attenuation));
			}
		}
		else if (rec.mat.type == DIELECTRIC)
		{
			if (dielectric_scatter(ray, &rec, &attenuation, &scattered))
			{
				return (color_multiply_vector(ray_color(&scattered, depth - 1, world), attenuation));
			}
		}
		return ((t_color){0.0, 0.0, 0.0});
		// direction = vec3_add(rec.normal, random_unit_vec3());
		// r = rt_ray(rec.p, direction);
		// return (color_multiply(ray_color(&r, depth-1, world), 0.5));
	}
	
	unit_direction = vec3_normalize(ray->direction);
	double	a;
	a = 0.5 * (unit_direction.y + 1.0);
	return (blend_colors((t_color){1.0, 1.0, 1.0}, (t_color){0.5, 0.7, 1.0}, a));
	*/

	if (!world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		// printf("No hit, return background color(r:%f,g:%f,b:%f)\n",world->background.r,world->background.g,world->background.b);
		return (world->ambient);
	}
	color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec, rec.u, rec.v, rec.p);
	if (rec.mat.type == LAMBERTIAN && !lambertian_scatter(ray, &rec, &attenuation, &scattered, &pdf_value))
			return (color_from_emission);
	else if (rec.mat.type == METAL && !metal_scatter(ray, &rec, &attenuation, &scattered, &pdf_value))
		return (color_from_emission);
	else if (rec.mat.type == DIELECTRIC && !dielectric_scatter(ray, &rec, &attenuation, &scattered, &pdf_value))
		return (color_from_emission);
	else if (rec.mat.type == DIFFUSE_LIGHT)
		return (color_from_emission);
	
	if (rec.mat.type ==METAL || rec.mat.type == DIELECTRIC)
	{
		return (color_multiply_vector(attenuation, ray_color(&scattered, depth - 1, world, lights)));
	}
/* //handcoded light sampling
	t_vec3	on_light = new_vec3(random_double_range(213,343),554,random_double_range(227,332));
	t_vec3	to_light = vec3_subtract(on_light, rec.p);
	double distance_squared = vec3_length_squared(to_light);
	to_light = unit_vector(to_light);
	if (vec3_dot(to_light, rec.normal) < 0)
		return (color_from_emission);
	
	double	light_area=(343-213)*(332-227);
	double	light_cosine = fabs(to_light.y);
	if (light_cosine < 0.000001)
		return (color_from_emission);
		
	// pdf_value = 1/(2 * M_PI);
	pdf_value = distance_squared/(light_cosine * light_area);
	scattered = rt_ray(rec.p, to_light);*/
	
	t_hitable_pdf	light_pdf;
	light_pdf = hitable_pdf_new(&lights, rec.p);
	scattered = rt_ray(rec.p, light_pdf.base.generate((t_pdf *)&light_pdf));
	pdf_value = light_pdf.base.value((t_pdf *)&light_pdf, scattered.direction);

	double	scatter_pdf = rec.mat.scattering_pdf(ray, &rec, &scattered);
	// printf("scatter_pdf: %f\n", scatter_pdf);
	// double pdf_value = scatter_pdf;
	t_color	sample_color;
	sample_color = ray_color(&scattered, depth - 1, world, lights);
	color_from_scatter = color_multiply_vector(color_multiply_number(attenuation, scatter_pdf), sample_color);
	color_from_scatter = color_multiply_number(color_from_scatter, 1.0 / pdf_value);

color_from_scatter = color_multiply_number(color_from_scatter, 4);

	return (color_add(color_from_emission, color_from_scatter));
}



void	camera_initialize(t_camera *camera)
{
	int	image_height;

	image_height = camera->image_width / camera->aspect_ratio;
	if (image_height < 1)
		image_height = 1;
	camera->image_height = image_height;
	camera->sqrt_spp = (int)sqrt(camera->samples_per_pixel);
	camera->pixel_samples_scale = 1.0 / (camera->sqrt_spp * camera->sqrt_spp);
	camera->recip_sqrt_spp = 1.0 / camera->sqrt_spp;
	
	camera->center = camera->lookfrom;

	// double	focal_length = vec3_length(vec3_subtract(camera->lookfrom, camera->lookat));
	double	focal_length = 10.0;
	// double	viewport_height = 2.0;
	double	theta = degrees_to_radians(camera->vfov);
	double	h = tan(theta / 2);
	double	viewport_height = 2.0 * h * focal_length;
	double	viewport_width = viewport_height * ((double)camera->image_width/image_height);

	camera->w = unit_vector(vec3_subtract(camera->lookfrom, camera->lookat));
	camera->u = unit_vector(vec3_cross(camera->vup, camera->w));
	camera->v = vec3_cross(camera->w, camera->u);

	t_vec3	viewport_u = vec3_multiply(camera->u, viewport_width);
	t_vec3	viewport_v = vec3_multiply(camera->v, -viewport_height);

	t_vec3	pixel_delta_u = vec3_multiply(viewport_u, 1.0 / (double)camera->image_width);
	t_vec3	pixel_delta_v = vec3_multiply(viewport_v, 1.0 / (double)camera->image_height);

	t_vec3	viewport_upper_left = vec3_subtract(camera->center,vec3_multiply(camera->w, focal_length));
	viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_multiply(viewport_u, 0.5));
	viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_multiply(viewport_v, 0.5));

	t_vec3	pixel00_loc = vec3_add(viewport_upper_left, vec3_multiply(vec3_add(pixel_delta_u,pixel_delta_v), 0.5));

	camera->pixel00_loc = pixel00_loc;
	camera->pixel_delta_u = pixel_delta_u;
	camera->pixel_delta_v = pixel_delta_v;
}

t_vec3	sample_square()
{
	double	u;
	double	v;

	u = random_double() - 0.5;
	v = random_double() - 0.5;
	return (new_vec3(u, v, 0.0));
}

t_vec3	sample_square_stratified(int s_i, int s_j, t_camera *camera)
{
	double	u;
	double	v;

	u = (random_double() + s_i) * camera->recip_sqrt_spp - 0.5;
	v = (random_double() + s_j) * camera->recip_sqrt_spp - 0.5;
	return (new_vec3(u, v, 0.0));
}

t_ray	get_ray(int pixel_x, int pixel_y, int s_i, int s_j, t_camera *camera)
{
	t_vec3	offset;
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	offset = sample_square_stratified(s_i, s_j, camera);
	pixel_sample = vec3_add(camera->pixel00_loc,
					vec3_add(vec3_multiply(camera->pixel_delta_u, pixel_x + offset.x),
							 vec3_multiply(camera->pixel_delta_v, pixel_y + offset.y)));
	ray_direction = vec3_subtract(pixel_sample, camera->center);
	return (rt_ray(camera->center, ray_direction));
}

void	camera_render(t_camera *camera, t_world *wld)
{
	t_data img;

	img.img = mlx_new_image(wld->mlx, camera->image_width, camera->image_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);

	for (int j = 0; j < camera->image_height; j++)
	{
		// printf("Row %d\n", j);
        for (int i = 0; i < camera->image_width; i++)
		{
			t_color pixel_color = {0,0,0};
			for (int s_j = 0; s_j < camera->sqrt_spp; s_j++)
			{
				for (int s_i = 0; s_i < camera->sqrt_spp; s_i++)
				{
					t_ray	r;
					r = get_ray(i, j, s_i, s_j, camera);
					pixel_color = color_add(pixel_color, ray_color_new(&r, camera->max_depth, wld));
					// printf("Pixel color: R=%f, G=%f, B=%f\n", pixel_color.r,pixel_color.g,pixel_color.b);
					// pixel_color = color_add(pixel_color, ray_color(&r, camera->max_depth, wld, wld->lights));
				}
			}
			write_color(&img, i, j, color_multiply_number(pixel_color, camera->pixel_samples_scale));
        }
    }
	mlx_put_image_to_window(wld->mlx, wld->win, img.img, 0, 0);
	printf("Image painted to window\n");
}

// t_camera	*new_camera(t_vec3 *origin, t_vec3 *direction, double fovy, t_film *film)
// {
// 	t_camera	*camera;
// 	camera = malloc(sizeof(t_camera));
// 	if (!camera)
// 		return (NULL);
// 	camera->origin = origin;
// 	camera->direction = direction;
// 	vec3_normalize(camera->direction);
// 	camera->fovy = fovy;
// 	camera->film = film;
// 	return (camera);
// }