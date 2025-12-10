#include "minirt.h"

t_diffuse_light	new_diffuse_light(t_texture *tex)
{
	t_diffuse_light	diffuse_light;

	diffuse_light.tex = tex;
	return (diffuse_light);
}

t_diffuse_light	new_diffuse_light_color(t_color color)
{
	t_diffuse_light	diffuse_light;
	t_solid_color_tex	*tex;

	tex = malloc(sizeof(t_solid_color_tex));
	*tex = solid_color_texture(color);
	diffuse_light.tex = (t_texture *)tex;
	return (diffuse_light);
}

t_color	diffuse_light_emitted(t_material *self, t_ray r_in, t_hit_record *rec, double u, double v, t_vec3 p)
{
	if (!rec->front_face)
		return (get_color(0.0, 0.0, 0.0));
	return (self->data.diffuse_light.tex->value(self->data.diffuse_light.tex, u, v, p));
}