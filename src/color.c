#include "minirt.h"

double	linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
		return (sqrt(linear_component));
	return (0.0);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	write_color(t_data *img, int x, int y, t_color color)
{
	int			r;
	int			g;
	int			b;
	t_interval	intensity;

	color.r = linear_to_gamma(color.r);
	color.g = linear_to_gamma(color.g);
	color.b = linear_to_gamma(color.b);
	intensity = (t_interval){0.0, 0.999};
	r = (int)(256 * interval_clamp(&intensity, color.r));
	g = (int)(256 * interval_clamp(&intensity, color.g));
	b = (int)(256 * interval_clamp(&intensity, color.b));
	my_mlx_pixel_put(img, x, y, (r << 16) | (g << 8) | b);
}

t_color	get_color(double r, double g, double b)
{
	t_color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	return (color);
}

t_color	get_normalize_color(t_color color)
{
	t_color	norm_color;

	norm_color.r = (color.r / 255.0);
	norm_color.g = (color.g / 255.0);
	norm_color.b = (color.b / 255.0);
	return (norm_color);
}

t_color	color_add(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r + b.r;
	result.g = a.g + b.g;
	result.b = a.b + b.b;
	return (result);
}
t_color	color_multiply_number(t_color color, double scalar)
{
	t_color	result;

	result.r = color.r * scalar;
	result.g = color.g * scalar;
	result.b = color.b * scalar;
	return (result);
}
t_color	color_multiply_vector(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r * b.r;
	result.g = a.g * b.g;
	result.b = a.b * b.b;
	return (result);
}

t_color color_clamp(t_color v, double min, double max)
{
	if (v.r < min)
		v.r = min;
	if (v.r > max)
		v.r = max;
	if (v.g < min)
		v.g = min;
	if (v.g > max)
		v.g = max;
	if (v.b < min)
		v.b = min;
	if (v.b > max)
		v.b = max;
	return v;
}
