#include "minirt.h"

int	lambertian_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)
{
	t_vec3	scatter_direction;

	(void)ray_in;
	t_onb	uvw = onb_new(rec->normal);
	// scatter_direction = vec3_add(rec->normal, random_unit_vec3());
	scatter_direction = onb_transform(uvw, random_cosine_direction());
	// if (vec3_near_zero(scatter_direction))
	// 	scatter_direction = rec->normal;
	*scattered = rt_ray(rec->p, unit_vector(scatter_direction));
	// printf("in lambertian_scatter %p\n",rec->mat.data.lamb.tex->value);
	// *attenuation = rec->mat.albedo;
	*attenuation = rec->mat.data.lamb.tex->value(rec->mat.data.lamb.tex, rec->u, rec->v, rec->p);
	*pdf = vec3_dot(onb_w(uvw), scattered->direction) / M_PI;
	return (1);
}

double	default_scattering_pdf(t_ray *ray_in, t_hit_record *rec, t_ray *scattered)
{
	(void)ray_in;
	(void)rec;
	(void)scattered;

	return (0);
}

t_color	default_emitted(t_material *self,t_ray r_in,t_hit_record *rec, double u, double v, t_vec3 p)
{
	(void)self;
	(void)u;
	(void)v;
	(void)p;
	(void)r_in;
	(void)rec;
	// return (get_color(0.0, 0.0, 0.0));
	
	switch(self->type)
	{
		case LAMBERTIAN:
			return color_multiply_number(self->data.lamb.tex->value(self->data.lamb.tex, u, v, p),0.3);
		case METAL:
			return color_multiply_number(self->data.metal.tex->value(self->data.metal.tex, u, v, p),0.3);
		case DIELECTRIC:
			return (get_color(0.0, 0.0, 0.0));
		default:
			return (get_color(0.0, 0.0, 0.0));
	}
}

double	lambertian_scattering_pdf(t_ray *ray_in, t_hit_record *rec, t_ray *scattered)
{
	double cosine;

	(void)ray_in;
	cosine = fmax(vec3_dot(rec->normal, unit_vector(scattered->direction)), 0.0);
	(void)cosine;
	// return (cosine / M_PI);
	return (1 / (2 * M_PI));
}

int	metal_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)
{
	t_vec3	reflected;

	reflected = vec3_reflect(ray_in->direction, rec->normal);
	reflected = vec3_add(unit_vector(reflected), vec3_multiply(random_unit_vec3(), rec->mat.data.metal.fuzz));
	*scattered = rt_ray(rec->p, reflected);
	*attenuation = rec->mat.data.metal.tex->value(rec->mat.data.metal.tex, rec->u, rec->v, rec->p);
	return (vec3_dot(scattered->direction, rec->normal) > 0);
}

double	reflectance(double cosine, double ref_idx)
{
	// Use Schlick's approximation for reflectance.
	double	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

int	dielectric_scatter(t_ray *ray_in, t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)
{
	double	ri;

	*attenuation = get_color(1.0, 1.0, 1.0);
	if (rec->front_face)
		ri = 1.0 / rec->mat.data.dielect.ref_idx;
	else
		ri = rec->mat.data.dielect.ref_idx;
	t_vec3	unit_direction = unit_vector(ray_in->direction);
	double	cos_theta = fmin(vec3_dot(vec3_multiply(unit_direction, -1), rec->normal), 1.0);
	double	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	int		cannot_refract = ri * sin_theta > 1.0;
	
	t_vec3	direction;
	if (cannot_refract || reflectance(cos_theta, ri) > random_double())
		direction = vec3_reflect(unit_direction, rec->normal);
	else
		direction = vec3_refract(unit_direction, rec->normal, ri);
	*scattered = rt_ray(rec->p, direction);
	return (1);
}

t_material get_material(t_mat_type type, t_color albedo, double fuz_ridx)
{
	t_material mat;
	t_solid_color_tex	*tex;

	mat.type = type;
	tex = malloc(sizeof(t_solid_color_tex));
	if (type == LAMBERTIAN)
	{
		*tex = solid_color_texture(albedo);
		mat.data.lamb.tex = (t_texture *)tex;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &lambertian_scattering_pdf;
	}
	else if (type == METAL)
	{
		if (fuz_ridx < 1)
			mat.data.metal.fuzz = fuz_ridx;
		else
			mat.data.metal.fuzz = 1;
		*tex = solid_color_texture(albedo);
		mat.data.metal.tex = (t_texture *)tex;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	else if (type == DIELECTRIC)
	{
		mat.data.dielect.ref_idx = fuz_ridx;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	else if( type == DIFFUSE_LIGHT)
	{
		*tex = solid_color_texture(albedo);
		mat.data.diffuse_light.tex = (t_texture *)tex;
		mat.emitted = &diffuse_light_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	else
	{
		*tex = solid_color_texture(albedo);
		mat.data.lamb.tex = (t_texture *)tex;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	return (mat);
}

t_material	get_material_texture(t_mat_type type, t_texture *texture, double fuz_ridx)
{
	t_material	mat;

	mat.type = type;
	if (type == LAMBERTIAN)
	{
		mat.data.lamb.tex = texture;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &lambertian_scattering_pdf;
	}
	else if (type == METAL)
	{
		if (fuz_ridx < 1)
			mat.data.metal.fuzz = fuz_ridx;
		else
			mat.data.metal.fuzz = 1;
		mat.data.metal.tex = texture;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	else if (type == DIELECTRIC)
	{
		mat.data.dielect.ref_idx = fuz_ridx;
		mat.data.dielect.tex = texture;
		mat.emitted = &default_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	else if (type == DIFFUSE_LIGHT)
	{
		mat.data.diffuse_light.tex = texture;
		mat.emitted = &diffuse_light_emitted;
		mat.scattering_pdf = &default_scattering_pdf;
	}
	return (mat);
}

