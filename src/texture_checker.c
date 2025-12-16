#include "minirt.h"

t_checker_texture	checker_texture(double scale, t_texture *even, t_texture *odd)
{
	t_checker_texture	texture;

	texture.inv_scale = scale;
	texture.even = even;
	texture.odd = odd;
	texture.base.value = &checker_texture_value;
	return (texture);
}

t_checker_texture	*checker_texture_colors(double scale, t_color even_color, t_color odd_color)
{
	t_checker_texture	*texture;
	t_solid_color_tex	*even_tex;
	t_solid_color_tex	*odd_tex;

	texture = malloc(sizeof(t_checker_texture));
	even_tex = malloc(sizeof(t_solid_color_tex));
	odd_tex = malloc(sizeof(t_solid_color_tex));
	*even_tex = solid_color_texture(even_color);
	*odd_tex = solid_color_texture(odd_color);
	*texture = checker_texture(scale, (t_texture *)even_tex, (t_texture *)odd_tex);
	return (texture);
}

t_color	checker_texture_value(t_texture *texture, double u, double v, t_vec3 p)
{
	t_checker_texture *ct = (t_checker_texture *)texture;
    double s = sin(p.x * ct->inv_scale * M_PI)
             * sin(p.z * ct->inv_scale * M_PI);
    if (s < 0)
        return texture_value(ct->even, u, v, p);
    else
        return texture_value(ct->odd, u, v, p);
}
t_color	checker_texture_value_old(t_texture *texture, double u, double v, t_vec3 p)
{
	int					xInt;
	int					yInt;
	int					zInt;
	int					isEven;
	t_checker_texture	*checker_tex;

	checker_tex = (t_checker_texture *)texture;
	xInt = (int)(floor(p.x * checker_tex->inv_scale));
	yInt = (int)(floor(p.y * checker_tex->inv_scale));
	zInt = (int)(floor(p.z * checker_tex->inv_scale));
	isEven = (xInt + yInt + zInt) % 2 == 0;
	if (isEven)
		return (texture_value(checker_tex->even, u, v, p));
	else
		return (texture_value(checker_tex->odd, u, v, p));
}