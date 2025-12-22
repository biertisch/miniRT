/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 17:22:50 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 21:58:24 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	default_scattering_pdf(t_ray *ray_in, t_hit_record *rec, t_ray *scater)
{
	(void)ray_in;
	(void)rec;
	(void)scater;
	return (0);
}

t_color	default_emitted(t_material *self, t_ray r_in, t_hit_record *rec)
{
	(void)self;
	(void)r_in;
	if (self->type == LAMBERTIAN)
		return (color_multi_num(self->data.lamb.tex->value(
					self->data.lamb.tex, rec->u, rec->v, rec->p), 0.3));
	else if (self->type == METAL)
		return (color_multi_num(self->data.metal.tex->value(
					self->data.metal.tex, rec->u, rec->v, rec->p), 0.3));
	else if (self->type == DIELECTRIC)
		return (get_color(0.0, 0.0, 0.0));
	else
		return (get_color(0.0, 0.0, 0.0));
}

double	lambertian_scattering_pdf(t_ray *ray_in, t_hit_record *rec,
	t_ray *scattered)
{
	double	cosine;

	(void)ray_in;
	cosine = fmax(vec3_dot(rec->normal,
				unit_vector(scattered->direction)), 0.0);
	(void)cosine;
	return (1 / (2 * M_PI));
}

t_material	get_material(t_mat_type type, t_color albedo, double fuz_ridx)
{
	t_material			mat;
	t_solid_color_tex	*tex;

	mat.type = type;
	tex = malloc(sizeof(t_solid_color_tex));
	*tex = solid_color_texture(norm_color(albedo));
	mat.emitted = &default_emitted;
	if (type == LAMBERTIAN)
		mat.data.lamb.tex = (t_texture *)tex;
	else if (type == METAL)
	{
		if (fuz_ridx < 1)
			mat.data.metal.fuzz = fuz_ridx;
		else
			mat.data.metal.fuzz = 1;
		mat.data.metal.tex = (t_texture *)tex;
	}
	else if (type == DIELECTRIC)
		mat.data.dielect.ref_idx = fuz_ridx;
	else if (type == DIFFUSE_LIGHT)
	{
		mat.data.diffuse_light.tex = (t_texture *)tex;
		mat.emitted = &diffuse_light_emitted;
	}
	return (mat);
}

t_material	get_material_texture(t_mat_type type, t_texture *texture,
	double fuz_ridx)
{
	t_material	mat;

	mat.type = type;
	mat.emitted = &default_emitted;
	if (type == LAMBERTIAN)
		mat.data.lamb.tex = texture;
	else if (type == METAL)
	{
		if (fuz_ridx < 1)
			mat.data.metal.fuzz = fuz_ridx;
		else
			mat.data.metal.fuzz = 1;
		mat.data.metal.tex = texture;
	}
	else if (type == DIELECTRIC)
	{
		mat.data.dielect.ref_idx = fuz_ridx;
		mat.data.dielect.tex = texture;
	}
	else if (type == DIFFUSE_LIGHT)
	{
		mat.data.diffuse_light.tex = texture;
		mat.emitted = &diffuse_light_emitted;
	}
	return (mat);
}

/*
int	lambertian_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation,
	t_ray *scattered, double *pdf)
{
	t_vec3	scatter_direction;

	(void)ray_in;
	t_onb	uvw = onb_new(rec->normal);
	scatter_direction = onb_transform(uvw, random_cosine_direction());
	*scattered = rt_ray(rec->p, unit_vector(scatter_direction));
	*attenuation = rec->mat.data.lamb.tex->value(rec->mat.data.lamb.tex,
		rec->u, rec->v, rec->p);
	*pdf = vec3_dot(onb_w(uvw), scattered->direction) / M_PI;
	return (1);
}

int	metal_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation,
	t_ray *scattered, double *pdf)
{
	t_vec3	reflected;

	(void)pdf;
	reflected = vec3_reflect(ray_in->direction, rec->normal);
	reflected = vec3_add(unit_vector(reflected), vec3_mul_n(random_unit_vec3(),
				rec->mat.data.metal.fuzz));
	*scattered = rt_ray(rec->p, reflected);
	*attenuation = rec->mat.data.metal.tex->value(rec->mat.data.metal.tex,
			rec->u, rec->v, rec->p);
	return (vec3_dot(scattered->direction, rec->normal) > 0);
}

int	dielectric_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation,
	t_ray *scattered, double *pdf)
{
	double	ri;
	t_vec3	unit_direction;
	double	cos_theta;
	double	sin_theta;
	int		cannot_refract;
	t_vec3	direction;

	(void)pdf;
	*attenuation = get_color(1.0, 1.0, 1.0);
	if (rec->front_face)
		ri = 1.0 / rec->mat.data.dielect.ref_idx;
	else
		ri = rec->mat.data.dielect.ref_idx;
	unit_direction = unit_vector(ray_in->direction);
	cos_theta = fmin(vec3_dot(vec3_mul_n(unit_direction, -1),
				rec->normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	cannot_refract = ri * sin_theta > 1.0;
	if (cannot_refract || reflectance(cos_theta, ri) > random_double())
		direction = vec3_reflect(unit_direction, rec->normal);
	else
		direction = vec3_refract(unit_direction, rec->normal, ri);
	*scattered = rt_ray(rec->p, direction);
	return (1);
}

double	reflectance(double cosine, double ref_idx)
{
	double	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}
*/