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

t_color ray_color_v1(t_ray *ray, int depth, t_world *world)
{
    t_hit_record	rec;
    t_hit_record	temp_rec;
    t_ray			r_2light;
    t_color			ambient;
    t_color			diffuse;
    t_color			specular;
    t_color			color_from_emission;
    t_vec3			light_dir;
    t_vec3			view_dir;
    t_vec3			reflect_dir;
    double			cos_nl;
    double			cos_rv;
    double			brightness;
    double			distance;
    double			attenuation;

    if (depth <= 0)
        return (get_color(0.0, 0.0, 0.0));

    ambient = color_multiply_number(world->ambient, world->ambient_ratio);
    
    if (world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
    {
        color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec, rec.u, rec.v, rec.p);
        
		// t_vec3 shadow_origin = vec3_add(rec.p, vec3_multiply(rec.normal, 0.001));
        // r_2light = rt_ray(shadow_origin, vec3_subtract(world->spot_light.position, shadow_origin));
		
		r_2light = rt_ray(rec.p, vec3_subtract(world->spot_light.position, rec.p));
        distance = vec3_length(vec3_subtract(world->spot_light.position, rec.p));
    
        // Check for shadows
        if (world_hit(world, &r_2light, new_interval(0.00001, distance-0.00001), &temp_rec))
        {
			// printf("In shadow\n");
			// printf("Hit object type in shadow: %d, t: %f, distance: %f, front_face: %d, p: (%f, %f, %f)\n", temp_rec.hit_obj->type,temp_rec.t, distance,temp_rec.front_face,temp_rec.p.x,temp_rec.p.y,temp_rec.p.z);
            // In shadow - only ambient and emission
			if (temp_rec.t <= 1)
            	return color_clamp(color_multiply_vector(color_from_emission, color_multiply_number(ambient, 2.2)), 0.0, 1.0);
        }
        
        // Calculate attenuation
        attenuation = compute_attenuation(distance);
        
        // Diffuse component (Lambert)
        light_dir = vec3_normalize(r_2light.direction);
        cos_nl = vec3_dot(rec.normal, light_dir);
        if (cos_nl < 0)
            cos_nl = 0;
        brightness = world->spot_light.brightness * cos_nl;
        diffuse = color_multiply_number(world->spot_light.light_color, brightness * attenuation);
        
        // Specular component (Phong)
        view_dir = vec3_normalize(vec3_subtract(ray->origin, rec.p));
        reflect_dir = vec3_subtract(vec3_multiply(rec.normal, 2.0 * vec3_dot(light_dir, rec.normal)), light_dir);
        cos_rv = vec3_dot(reflect_dir, view_dir);
        if (cos_rv < 0)
            cos_rv = 0;
        
        // Shininess factor - adjust this value (higher = sharper highlight)
        double shininess = 32.0;
        double spec_strength = pow(cos_rv, shininess);
        specular = color_multiply_number(world->spot_light.light_color, spec_strength * attenuation * world->spot_light.brightness);
        
        // Combine all components: ambient + diffuse + specular
        t_color final_color = color_add(color_multiply_number(ambient, 2.2), diffuse);
        final_color = color_add(final_color, specular);
        
        // Multiply by surface color and add emission
        return color_clamp(color_add(color_from_emission, color_multiply_vector(final_color, color_from_emission)), 0.0, 1.0);
    }
    else
        return (ambient);
}

t_color ray_color_v0(t_ray *ray, int depth, t_world *world)
{
	t_hit_record	rec;
	t_hit_record	temp_rec;
	t_ray			scattered;
	t_ray			r_2light;
	t_color			ambient;
	t_color			color_from_emission;
	double			cos_nl;
	double			brightness;
	t_color			diffuse;

	if (depth <= 0)
		return (get_color(0.0, 0.0, 0.0));

	ambient = color_multiply_number(world->ambient, world->ambient_ratio);
	// if (world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
	if (world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec, rec.u, rec.v, rec.p);
		// printf("Emitted color: R=%f, G=%f, B=%f\n", color_from_emission.r, color_from_emission.g, color_from_emission.b);
		r_2light = rt_ray(rec.p, vec3_subtract(world->spot_light.position, rec.p));
		
		// if (world_hit(world->bvh_root, &r_2light, new_interval(0.001, 0.0000001 + vec3_length(vec3_subtract(world->spot_light.position, rec.p))), &temp_rec))
		if (world_hit(world, &r_2light, new_interval(0.001, 0.0000001 + vec3_length(vec3_subtract(world->spot_light.position, rec.p))), &temp_rec))
		{
			return color_clamp(color_multiply_vector(color_from_emission, color_multiply_number( ambient, 2.2)), 0.0, 1.0);
		}
		else
		{
			cos_nl = vec3_dot(rec.normal,vec3_normalize(r_2light.direction));
			if (cos_nl < 0)
				cos_nl = 0;
			brightness = world->spot_light.brightness * cos_nl / vec3_length(r_2light.direction);

// printf("Brightness: %f\n", brightness * compute_attenuation(vec3_length(r_2light.direction)));

			diffuse = color_multiply_number(world->spot_light.light_color, brightness * compute_attenuation(vec3_length(r_2light.direction)));
			diffuse = color_add(diffuse, color_multiply_number(ambient, 2.2));

			// return color_multiply_vector(diffuse, color_from_emission);
			return color_clamp(color_add(color_from_emission, color_multiply_vector(diffuse, color_from_emission)),0.0,1.0);
		}
	}
	else
		return (ambient);
}

void	output_camara_info(t_camera *camera)
{
	printf("Camera Info:\n");
	printf("  Lookfrom: (%f, %f, %f)\n", camera->lookfrom.x, camera->lookfrom.y, camera->lookfrom.z);
	printf("  Lookat:   (%f, %f, %f)\n", camera->lookat.x, camera->lookat.y, camera->lookat.z);
	printf("  Vup:      (%f, %f, %f)\n", camera->vup.x, camera->vup.y, camera->vup.z);
	printf("  U:        (%f, %f, %f)\n", camera->u.x, camera->u.y, camera->u.z);
	printf("  V:        (%f, %f, %f)\n", camera->v.x, camera->v.y, camera->v.z);
	printf("  W:        (%f, %f, %f)\n", camera->w.x, camera->w.y, camera->w.z);
	printf("  FOV:      %f\n", camera->vfov);
	printf("  Aspect Ratio: %f\n", (double)camera->image_width / (double)camera->image_height);
	printf("  Image Width:  %d\n", camera->image_width);
	printf("  Image Height: %d\n", camera->image_height);
	printf("  Samples per Pixel: %d\n", camera->samples_per_pixel);
	printf("  Max Depth:        %d\n", camera->max_depth);
	printf("  Pixel00 Location:    (%f, %f, %f)\n", camera->pixel00_loc.x, camera->pixel00_loc.y, camera->pixel00_loc.z);
	printf("  Pixel Delta U:       (%f, %f, %f)\n", camera->pixel_delta_u.x, camera->pixel_delta_u.y, camera->pixel_delta_u.z);
	printf("  Pixel Delta V:       (%f, %f, %f)\n", camera->pixel_delta_v.x, camera->pixel_delta_v.y, camera->pixel_delta_v.z);
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

	// if (!world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
	if (!world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
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

	camera->aspect_ratio = 16.0/9.0;
	camera->image_width = 800;
	camera->samples_per_pixel = 1;
	camera->max_depth = 5;
	image_height = camera->image_width / camera->aspect_ratio;
	if (image_height < 1)
		image_height = 1;
	camera->image_height = image_height;
	camera->sqrt_spp = (int)sqrt(camera->samples_per_pixel);
	camera->pixel_samples_scale = 1.0 / (camera->sqrt_spp * camera->sqrt_spp);
	camera->recip_sqrt_spp = 1.0 / camera->sqrt_spp;
	double	focal_length = vec3_length(vec3_subtract(camera->lookfrom, camera->lookat));
	// double	focal_length = 10.0;
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

	t_vec3	viewport_upper_left = vec3_subtract(camera->lookfrom,vec3_multiply(camera->w, focal_length));
	viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_multiply(viewport_u, 0.5));
	viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_multiply(viewport_v, 0.5));

	t_vec3	pixel00_loc = vec3_add(viewport_upper_left, vec3_multiply(vec3_add(pixel_delta_u,pixel_delta_v), 0.5));

	camera->pixel00_loc = pixel00_loc;
	camera->pixel_delta_u = pixel_delta_u;
	camera->pixel_delta_v = pixel_delta_v;

	output_camara_info(camera);
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

static	t_ray	get_ray(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	pixel_sample = vec3_add(camera->pixel00_loc,
					vec3_add(vec3_multiply(camera->pixel_delta_u, pixel_x),
							 vec3_multiply(camera->pixel_delta_v, pixel_y)));
	ray_direction = vec3_subtract(pixel_sample, camera->lookfrom);
	return (rt_ray(camera->lookfrom, ray_direction));
}

t_ray	get_ray_v0(int pixel_x, int pixel_y, int s_i, int s_j, t_camera *camera)
{
	t_vec3	offset;
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	offset = sample_square_stratified(s_i, s_j, camera);
	pixel_sample = vec3_add(camera->pixel00_loc,
					vec3_add(vec3_multiply(camera->pixel_delta_u, pixel_x + offset.x),
							 vec3_multiply(camera->pixel_delta_v, pixel_y + offset.y)));
	ray_direction = vec3_subtract(pixel_sample, camera->lookfrom);
	return (rt_ray(camera->lookfrom, ray_direction));
}

void	camera_render(t_camera *camera, t_world *wld)
{
	t_data	img;
	t_color	pixel_color;
	t_ray	r;
	int		i;
	int		j;

	img.img = mlx_new_image(wld->mlx, camera->image_width, camera->image_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	j = 0;
	while (j < camera->image_height)
	{
		i = 0;
        while (i < camera->image_width)
		{
			pixel_color = (t_color){0,0,0};
			r = get_ray(i, j, camera);
			pixel_color = color_add(pixel_color, ray_color_v1(&r, camera->max_depth, wld));
			write_color(&img, i, j, 
				color_multiply_number(pixel_color, camera->pixel_samples_scale));
			i++;
		}
		j++;
    }
	mlx_put_image_to_window(wld->mlx, wld->win, img.img, 0, 0);
	printf("Image painted to window\n");
}
/*
void	camera_render_v0(t_camera *camera, t_world *wld)
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
					pixel_color = color_add(pixel_color, ray_color_v1(&r, camera->max_depth, wld));
					// pixel_color = color_add(pixel_color, ray_color(&r, camera->max_depth, wld, wld->lights));
				}
			}
			write_color(&img, i, j, color_multiply_number(pixel_color, camera->pixel_samples_scale));
        }
    }
	mlx_put_image_to_window(wld->mlx, wld->win, img.img, 0, 0);
	printf("Image painted to window\n");
}
*/

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