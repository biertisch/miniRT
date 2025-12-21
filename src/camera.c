/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 16:13:09 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 08:23:31 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	blend_colors(t_color c1, t_color c2, double t)
{
	t_color	blended;

	blended.r = (1 - t) * c1.r + t * c2.r;
	blended.g = (1 - t) * c1.g + t * c2.g;
	blended.b = (1 - t) * c1.b + t * c2.b;
	return (blended);
}

double	attenuation(double d)
{
	double	constant;
	double	linear;
	double	quadratic;

	constant = 1.0;
	linear = 0.1;
	quadratic = 0.01;
	return (1.0 / (constant + linear * d + quadratic * d * d));
}

int	in_shadow(t_vec3 point, t_world *world, t_vec3 light_pos)
{
	t_vec3			to_light;
	t_ray			shadow_r;
	t_hit_record	temp_rec;
	double			distance_to_light;

	to_light = vec3_sub(light_pos, point);
	distance_to_light = vec3_length(to_light);
	shadow_r.direction = vec3_norm(to_light);
	shadow_r.origin = vec3_add(point, vec3_mul_n(shadow_r.direction, 1e-4));
	if (world_hit(world, &shadow_r, new_interval(0.001,
				distance_to_light - 1e-4), &temp_rec))
		return (1);
	return (0);
}

void	value_clamp_min(double *value, double tob, double min)
{
	if (tob < min)
		*value = min;
	else
		*value = tob;
}

t_color	phone_of_light(t_phong *phong, t_hit_record *rec,
			t_world *world, t_s_light light)
{
	t_ray	r2l;
	t_vec3	reflect_dir;
	t_color	f_color;

	if (in_shadow(rec->p, world, light.position))
		return (color_clamp(color_mult_color(phong->o_color,
					color_multi_num(phong->ambient, 2.2)), 0.0, 1.0));
	r2l = rt_ray(rec->p, vec3_norm(vec3_sub(light.position, rec->p)));
	phong->atn = attenuation(vec3_length(vec3_sub(light.position, rec->p)));
	value_clamp_min(&phong->cos_nl, vec3_dot(rec->normal, r2l.direction), 0.0);
	phong->bright = light.brightness * phong->cos_nl;
	phong->diffuse = color_multi_num(light.light_color,
			phong->bright * phong->atn);
	reflect_dir = vec3_sub(vec3_mul_n(rec->normal, 2.0
				* vec3_dot(r2l.direction, rec->normal)), r2l.direction);
	value_clamp_min(&phong->cos_rv, vec3_dot(reflect_dir,
			vec3_norm(vec3_sub(rec->ray_in.origin, rec->p))), 0.0);
	phong->specular = color_multi_num(light.light_color, pow(phong->cos_rv,
				SPECULAR_FACTOR) * phong->cos_nl * light.brightness);
	f_color = color_add(color_multi_num(phong->ambient, 2.2), phong->diffuse);
	f_color = color_add(f_color, phong->specular);
	return (f_color);
}
t_color	ray_color_v3(t_ray *ray, int depth, t_world *world);

t_color	metal_reflection_color(t_phong *phong, t_hit_record *rec,
			int depth, t_world *world)
{
	t_color	reflected_color;
	t_color	final_color;
	t_vec3	c_i;
	t_vec3	c_r;
	t_ray	reflect_ray;

	reflected_color = get_color(0, 0, 0);
	final_color = color_add(phong->ambient, phong->specular);
	c_i = vec3_norm(rec->ray_in.direction);
	c_r = vec3_sub(c_i, vec3_mul_n(rec->normal, 2.0
				* vec3_dot(c_i, rec->normal)));
	reflect_ray.origin = vec3_add(rec->p, vec3_mul_n(rec->normal, 1e-4));
	reflect_ray.direction = vec3_norm(c_r);
	reflected_color = ray_color_v3(&reflect_ray, depth - 1, world);
	final_color = blend_colors(final_color, reflected_color, 0.8);
	// final_color = color_add(color_multi_num(final_color, 1.0 - 0.8),
	// 		color_multi_num(reflected_color, 0.8));
	return (final_color);
}

t_color	ray_color_v3(t_ray *ray, int depth, t_world *world)
{
	t_hit_record	rec;
	t_phong			phong;
	t_color			final_color;

	if (depth <= 0)
		return (get_color(0.0, 0.0, 0.0));
	phong.ambient = color_multi_num(world->ambient, world->ambient_ratio);
	if (world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		phong.o_color = rec.mat.emitted(&rec.mat, *ray, &rec);
		rec.orig_color = phong.o_color;
		rec.ray_in = *ray;
		final_color = phone_of_light(&phong, &rec, world, world->spot_light);
		if (rec.mat.type == METAL)
			final_color = metal_reflection_color(&phong, &rec, depth, world);
		return color_clamp(color_add(phong.o_color,
				color_mult_color(final_color, phong.o_color)), 0.0, 1.0);
	}
	else
		return (phong.ambient);
}

void	output_camara_info(t_camera *c)
{
	if (DEBUG == 0)
		return ;
	printf("Camera Info:\n");
	printf("  Lookfrom: (%f, %f, %f)\n", c->lookfrom.x,
		c->lookfrom.y, c->lookfrom.z);
	printf("  direction:   (%f, %f, %f)\n", c->forword.x,
		c->forword.y, c->forword.z);
	printf("  Vup:      (%f, %f, %f)\n", c->vup.x,
		c->vup.y, c->vup.z);
	printf("  U:        (%f, %f, %f)\n", c->u.x, c->u.y, c->u.z);
	printf("  V:        (%f, %f, %f)\n", c->v.x, c->v.y, c->v.z);
	printf("  W:        (%f, %f, %f)\n", c->w.x, c->w.y, c->w.z);
	printf("  FOV:      %f\n", c->vfov);
	printf("  Aspect Ratio: %f\n", (double)c->img_w / (double)c->img_h);
	printf("  Image Width:  %d\n", c->img_w);
	printf("  Image Height: %d\n", c->img_h);
	printf("  Max Depth:        %d\n", c->max_depth);
	printf("  Pixel00 Location:    (%f, %f, %f)\n", c->pix00_loc.x,
		c->pix00_loc.y, c->pix00_loc.z);
	printf("  Pixel Delta U:       (%f, %f, %f)\n", c->pix_delta_u.x,
		c->pix_delta_u.y, c->pix_delta_u.z);
	printf("  Pixel Delta V:       (%f, %f, %f)\n", c->pix_delta_v.x,
		c->pix_delta_v.y, c->pix_delta_v.z);
}

void	init_camera_viewport(t_camera *c)
{
	t_vec3	vport_upper_left;
	double	focal_length;
	double	viewport_h;
	t_vec3	viewport_v;
	t_vec3	viewport_u;

	focal_length = vec3_length(c->forword);
	viewport_h = 2.0 * (tan(degrees_to_radians(c->vfov) / 2)) * focal_length;
	viewport_u = vec3_mul_n(c->u, viewport_h * ((double)c->img_w / c->img_h));
	viewport_v = vec3_mul_n(c->v, -viewport_h);
	c->pix_delta_u = vec3_mul_n(viewport_u, 1.0 / (double)c->img_w);
	c->pix_delta_v = vec3_mul_n(viewport_v, 1.0 / (double)c->img_h);
	vport_upper_left = vec3_sub(c->lookfrom, vec3_mul_n(c->w, focal_length));
	vport_upper_left = vec3_sub(vport_upper_left, vec3_mul_n(viewport_u, 0.5));
	vport_upper_left = vec3_sub(vport_upper_left, vec3_mul_n(viewport_v, 0.5));
	c->pix00_loc = vec3_add(vport_upper_left,
			vec3_mul_n(vec3_add(c->pix_delta_u, c->pix_delta_v), 0.5));
}

t_vec3	choose_vup(t_vec3 forward)
{
	t_vec3	vup;

	vup = (t_vec3){0.0, 1.0, 0.0};
	if (fabs(vec3_dot(forward, vup)) >= 0.999)
		vup = (t_vec3){0.0, 0.0, 1.0};
	return (vup);
}

void	camera_light_initialize(t_world *wld)
{
	t_camera	*camera;

	camera = &wld->camera;
	camera->aspect_ratio = 16.0 / 9.0;
	camera->img_w = WIDTH;
	camera->max_depth = DEPTH;
	if (!camera->initialized)
	{
		camera->pitch = asin(camera->forword.y);
		camera->yaw = atan2(camera->forword.z, camera->forword.x);
		camera->vup = choose_vup(camera->forword);
	}
	camera->img_h = camera->img_w / camera->aspect_ratio;
	if (camera->img_h < 1)
		camera->img_h = 1;
	camera->w = vec3_mul_n(unit_vector(camera->forword), -1);
	camera->u = unit_vector(vec3_cross(camera->vup, camera->w));
	camera->v = vec3_cross(camera->w, camera->u);
	init_camera_viewport(camera);
	camera->initialized = 1;
	if (is_camera_on_surface(camera, wld))
	{
		printf("Caution: Camera is positioned on an object surface.\n");
	}
	output_camara_info(camera);
}

static	t_ray	get_ray(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_point;
	t_vec3	ray_direction;

	pixel_point = vec3_add(camera->pix00_loc,
			vec3_add(vec3_mul_n(camera->pix_delta_u, pixel_x),
				vec3_mul_n(camera->pix_delta_v, pixel_y)));
	ray_direction = vec3_norm(vec3_sub(pixel_point, camera->lookfrom));
	return (rt_ray(camera->lookfrom, ray_direction));
}

void	camera_render(t_camera *cam, t_world *wld)
{
	t_data	img;
	t_color	pix_c;
	t_ray	r;
	int		i;
	int		j;

	img.img = mlx_new_image(wld->mlx, cam->img_w, cam->img_h);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	j = 0;
	while (j < cam->img_h)
	{
		i = 0;
		while (i < cam->img_w)
		{
			pix_c = (t_color){0, 0, 0};
			r = get_ray(i, j, cam);
			pix_c = color_add(pix_c, ray_color_v3(&r, cam->max_depth, wld));
			write_color(&img, i, j, pix_c);
			i++;
		}
		j++;
	}
	mlx_put_image_to_window(wld->mlx, wld->win, img.img, 0, 0);
	mlx_destroy_image(wld->mlx, img.img);
}

/*
// Version 2: Phong Reflection Model with Shadows and reflection
t_color	ray_color_v2(t_ray *ray, int depth, t_world *world)
{
	t_hit_record	rec;
	// t_hit_record	temp_rec;
	t_ray			r_2light;
	t_phong			phong;
	// t_vec3			light_dir;
	t_vec3			view_dir;
	t_vec3			reflect_dir;
	double			cos_nl;
	double			cos_rv;
	double			distance;
	double			attenuation;

	if (depth <= 0)
		return (get_color(0.0, 0.0, 0.0));
	phong.ambient = color_multi_num(world->ambient, world->ambient_ratio);
	if (world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
	{
		phong.obj_color = rec.mat.emitted(&rec.mat, *ray, &rec);

		r_2light = rt_ray(rec.p, vec3_norm(
		vec3_sub(world->spot_light.position, rec.p)));
		distance = vec3_length(vec3_sub(world->spot_light.position, rec.p));
		
		// Check for shadows
		if (in_shadow(rec.p, world,  world->spot_light.position))
			return (color_clamp(color_mult_color(phong.obj_color,
						color_multi_num(phong.ambient, 2.2)), 0.0, 1.0));
		// Calculate attenuation
		attenuation = compute_attenuation(distance);
		// Diffuse component (Lambert)
		cos_nl = vec3_dot(rec.normal, r_2light.direction);
		if (cos_nl < 0)
			cos_nl = 0;
		phong.brightness = world->spot_light.brightness * cos_nl;
		phong.diffuse = color_multi_num(world->spot_light.light_color, 
			phong.brightness * attenuation);

		// Specular component (Phong)
		view_dir = vec3_norm(vec3_sub(ray->origin, rec.p));
		reflect_dir = vec3_sub(vec3_mul_n(rec.normal, 2.0 
			* vec3_dot(r_2light.direction, rec.normal)), r_2light.direction);
		cos_rv = vec3_dot(reflect_dir, view_dir);
		if (cos_rv < 0)
			cos_rv = 0;

		// Shininess factor - adjust this value (higher = sharper highlight)
		double shininess = 32.0;
		double spec_strength = pow(cos_rv, shininess);
		phong.specular = color_multi_num(world->spot_light.light_color,
			spec_strength * attenuation * world->spot_light.brightness);

		// Combine all components: ambient + diffuse + specular
		t_color final_color = color_add(
			color_multi_num(phong.ambient, 2.2), phong.diffuse);
		final_color = color_add(final_color, phong.specular);

		// ===== 镜面反射部分 =====
		t_color reflected_color = get_color(0,0,0);

		if (rec.mat.type == METAL)
		{
			final_color = color_add(phong.ambient, phong.specular);
			t_vec3 I = vec3_norm(ray->direction);
			t_vec3 N = rec.normal;
			t_vec3 R = vec3_sub(I, vec3_mul_n(N, 2.0 * vec3_dot(I, N)));

			t_ray reflect_ray;
			reflect_ray.origin = vec3_add(rec.p, vec3_mul_n(N, 1e-4));
			reflect_ray.direction = vec3_norm(R);

			reflected_color = ray_color_v2(&reflect_ray, depth - 1, world);
			// 混合
			final_color = color_add(
				color_multi_num(final_color, 1.0 - 0.8),
				color_multi_num(reflected_color, 0.8)
			);
		}
		return color_clamp(color_add(phong.obj_color,
			color_mult_color(final_color, phong.obj_color)),0.0, 1.0);
	}
	else
		return (phong.ambient);
}

// Version 1: Phong Reflection Model with Shadows and specular highlights
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
color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec);
r_2light = rt_ray(rec.p, vec3_subtract(world->spot_light.position, rec.p));
distance = vec3_length(vec3_subtract(world->spot_light.position, rec.p));

// Check for shadows
if (world_hit(world, &r_2light, new_interval(0.00001, distance-0.00001),
	&temp_rec) && temp_rec.t <= 1.0)
	return color_clamp(color_multiply_vector(color_from_emission,
		color_multiply_number(ambient, 2.2)), 0.0, 1.0);
// Calculate attenuation
attenuation = compute_attenuation(distance);
// Diffuse component (Lambert)
light_dir = vec3_normalize(r_2light.direction);
cos_nl = vec3_dot(rec.normal, light_dir);
if (cos_nl < 0)
	cos_nl = 0;
brightness = world->spot_light.brightness * cos_nl;
diffuse = color_multiply_number(world->spot_light.light_color,
	brightness * attenuation);

// Specular component (Phong)
view_dir = vec3_normalize(vec3_subtract(ray->origin, rec.p));
reflect_dir = vec3_subtract(vec3_multiply(rec.normal,
	2.0 * vec3_dot(light_dir, rec.normal)), light_dir);
cos_rv = vec3_dot(reflect_dir, view_dir);
if (cos_rv < 0)
	cos_rv = 0;

// Shininess factor - adjust this value (higher = sharper highlight)
double shininess = 32.0;
double spec_strength = pow(cos_rv, shininess);
specular = color_multiply_number(world->spot_light.light_color,
	spec_strength * attenuation * world->spot_light.brightness);

// Combine all components: ambient + diffuse + specular
t_color final_color = color_add(color_multiply_number(ambient, 2.2), diffuse);
final_color = color_add(final_color, specular);

// Multiply by surface color and add emis
// camera->lookat = vec3_add(camera->lookfrom, camera->lookat);sion
return color_clamp(color_add(color_from_emission,
	color_multiply_vector(final_color, color_from_emission)), 0.0, 1.0);
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
color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec);
// printf("Emitted color: R=%f, G=%f, B=%f\n", color_from_emission.r,
	color_from_emission.g, color_from_emission.b);
r_2light = rt_ray(rec.p, vec3_subtract(world->spot_light.position, rec.p));

if (world_hit(world, &r_2light, new_interval(0.001, 0.0000001 
+ vec3_length(vec3_subtract(world->spot_light.position, rec.p))), &temp_rec))
{
return color_clamp(color_multiply_vector(color_from_emission,
	color_multiply_number( ambient, 2.2)), 0.0, 1.0);
}
else
{
cos_nl = vec3_dot(rec.normal,vec3_normalize(r_2light.direction));
if (cos_nl < 0)
cos_nl = 0;
brightness = world->spot_light.brightness * cos_nl 
	/ vec3_length(r_2light.direction);

diffuse = color_multiply_number(world->spot_light.light_color, 
	brightness * compute_attenuation(vec3_length(r_2light.direction)));
diffuse = color_add(diffuse, color_multiply_number(ambient, 2.2));

// return color_multiply_vector(diffuse, color_from_emission);
return color_clamp(color_add(color_from_emission,
	color_multiply_vector(diffuse, color_from_emission)),0.0,1.0);
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
	
// if (world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
// {
// 	t_ray	scattered;
// 	t_color	attenuation;
// 	if (rec.mat.type == LAMBERTIAN)
// 	{
// 		if (lambertian_scatter(ray, &rec, &attenuation, &scattered))
// 		{
// 			return (color_multiply_vector(
	ray_color(&scattered, depth - 1, world), attenuation));
// 		}
// 	}
// 	else if (rec.mat.type == METAL)
// 	{
// 		if (metal_scatter(ray, &rec, &attenuation, &scattered))
// 		{
// 			return (color_multiply_vector(
	ray_color(&scattered, depth - 1, world), attenuation));
// 		}
// 	}
// 	else if (rec.mat.type == DIELECTRIC)
// 	{
// 		if (dielectric_scatter(ray, &rec, &attenuation, &scattered))
// 		{
// 			return (color_multiply_vector(
	ray_color(&scattered, depth - 1, world), attenuation));
// 		}
// 	}
// 	return ((t_color){0.0, 0.0, 0.0});
// 	// direction = vec3_add(rec.normal, random_unit_vec3());
// 	// r = rt_ray(rec.p, direction);
// 	// return (color_multiply(ray_color(&r, depth-1, world), 0.5));
// }

// unit_direction = vec3_normalize(ray->direction);
// double	a;
// a = 0.5 * (unit_direction.y + 1.0);
// return (blend_colors((t_color){1.0, 1.0, 1.0}, (t_color){0.5, 0.7, 1.0}, a));
	

// if (!world_hit(world->bvh_root, ray, new_interval(0.001, RT_INFINITY), &rec))
if (!world_hit(world, ray, new_interval(0.001, RT_INFINITY), &rec))
{
return (world->ambient);
}
color_from_emission = rec.mat.emitted(&rec.mat, *ray, &rec);
if (rec.mat.type == LAMBERTIAN && !lambertian_scatter(ray, &rec, 
	&attenuation, &scattered, &pdf_value))
	return (color_from_emission);
else if (rec.mat.type == METAL && !metal_scatter(ray, &rec, 
	&attenuation, &scattered, &pdf_value))
return (color_from_emission);
else if (rec.mat.type == DIELECTRIC && !dielectric_scatter(ray, 
	&rec, &attenuation, &scattered, &pdf_value))
return (color_from_emission);
else if (rec.mat.type == DIFFUSE_LIGHT)
return (color_from_emission);

if (rec.mat.type ==METAL || rec.mat.type == DIELECTRIC)
{
return (color_multiply_vector(attenuation, 
	ray_color(&scattered, depth - 1, world, lights)));
}

t_hitable_pdf	light_pdf;
light_pdf = hitable_pdf_new(&lights, rec.p);
scattered = rt_ray(rec.p, light_pdf.base.generate((t_pdf *)&light_pdf));
pdf_value = light_pdf.base.value((t_pdf *)&light_pdf, scattered.direction);

double	scatter_pdf = rec.mat.scattering_pdf(ray, &rec, &scattered);
// printf("scatter_pdf: %f\n", scatter_pdf);
// double pdf_value = scatter_pdf;
t_color	sample_color;
sample_color = ray_color(&scattered, depth - 1, world, lights);
color_from_scatter = color_multiply_vector(color_multiply_number(
	attenuation, scatter_pdf), sample_color);
color_from_scatter = color_multiply_number(color_from_scatter,1.0 / pdf_value);

color_from_scatter = color_multiply_number(color_from_scatter, 4);

	return (color_add(color_from_emission, color_from_scatter));
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
// pixel_color = color_add(pixel_color, ray_color(&r, 
	camera->max_depth, wld, wld->lights));
				}
			}
write_color(&img, i, j, color_multiply_number(pixel_color,
	camera->pixel_samples_scale));
        }
    }
	mlx_put_image_to_window(wld->mlx, wld->win, img.img, 0, 0);
	printf("Image painted to window\n");
}

t_vec3	sample_square()
{
	double	u;
	double	v;

	u = random_double() - 0.5;
	v = random_double() - 0.5;
	return (new_vec3(u, v, 0.0));
}

// t_vec3	sample_square_stratified(int s_i, int s_j, t_camera *camera)
// {
// 	double	u;
// 	double	v;
// 	u = (random_double() + s_i) * camera->recip_sqrt_spp - 0.5;
// 	v = (random_double() + s_j) * camera->recip_sqrt_spp - 0.5;
// 	return (new_vec3(u, v, 0.0));
// }
*/