/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:26:32 by bliu              #+#    #+#             */
/*   Updated: 2025/12/24 02:50:30 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_bump_tex load_texture(void *mlx, char *path)
{
	t_bump_tex	tex;

	tex.img = mlx_xpm_file_to_image(mlx, path, &tex.width, &tex.height);
	tex.addr = mlx_get_data_addr(tex.img, &tex.bpp, &tex.line_len, &tex.endian);
	return (tex);
}

double	bump_map_height(t_bump_tex *tex, double u, double v)
{
	int		x;
	int		y;
	unsigned int	pixel;

	// x = (int)(u * tex->width) % tex->width;
	// y = (int)(v * tex->height) % tex->height;
	// if (x < 0)
	// 	x += tex->width;
	// if (y < 0)
	// 	y += tex->height;
	x = (int)(u * (tex->width  - 1));
	y = (int)((v) * (tex->height - 1));
	pixel = *(unsigned int *)(tex->addr
			+ y * tex->line_len
			+ x * (tex->bpp / 8));
	return ((pixel & 0xFF) / 255.0);
}

t_vec3 bump_tangent_normal(t_bump_tex *tex, double u, double v, double strength)
{
	double	du;
	double	dv;
	double	height;
	double	height_u;
	double	height_v;
	t_vec3	bump_normal;

	height = bump_map_height((t_bump_tex *)tex, u, v);
	height_u = bump_map_height((t_bump_tex *)tex, u + 0.001, v);
	height_v = bump_map_height((t_bump_tex *)tex, u, v + 0.001);
	du = (height_u - height) * strength;
	dv = (height_v - height) * strength;
	bump_normal = vec3_norm((t_vec3){-du, -dv, 1.0});
	return (bump_normal);
}

t_vec3	apply_bump_map(t_tbn tbn, t_vec3 Nt)
{
	t_vec3	bumped;

	bumped = vec3_add(vec3_mul_n(tbn.T, Nt.x),
			vec3_add(vec3_mul_n(tbn.B, Nt.y), vec3_mul_n(tbn.N, Nt.z)));
	return (vec3_norm(bumped));
}


double	noise_bump(double u, double v)
{
	return (fmod(sin(u * 12.9898 + v * 78.233) * 43758.5453, 1.0));
}

double	turbulence(double u, double v)
{
	double	sum;
	double	freq;
	double	amp;
	double	max;
	int		i;

	sum = 0.0;
	freq = 1.0;
	amp = 1.0;
	max = 0.0;
	i = 0;
	while (i < 5)
	{
		sum += fabs(noise_bump(u * freq, v * freq)) * amp;
		max += amp;
		freq *= 2.0;
		amp *= 0.5;
		i++;
	}
	return (sum / max);
}

double	checker_bump(double u, double v)
{
	double scale;
	int check_u;
	int check_v;

	scale = 10.0;
	check_u = (int)(u * scale);
	check_v = (int)(v * scale);
	if ((check_u + check_v) % 2 == 0)
		return (0.1);
	else
		return (-0.1);
}

double sine_bump(double u, double v)
{
	double freq;

	freq = 20.0;
	return sin(u * freq * 2 * M_PI)
		* sin(v * freq * 2 * M_PI);
}


t_tbn get_tbn_plane(void)
{
	t_tbn tbn;

	tbn.T = (t_vec3){1, 0, 0};
	tbn.B = (t_vec3){0, 0, 1};
	tbn.N = (t_vec3){0, 1, 0};
	return tbn;
}

t_tbn get_tbn_cylinder(t_vec3 P, t_vec3 axis)
{
	t_tbn tbn;

	tbn.N = vec3_norm((t_vec3){P.x, 0, P.z});
	tbn.T = vec3_norm((t_vec3){-P.z, 0, P.x});
	tbn.B = axis;
	return (tbn);
}

t_tbn get_tbn_cone(t_vec3 P, t_vec3 axis)
{
	t_tbn tbn;

	tbn.T = vec3_norm((t_vec3){-P.z, 0, P.x});
	tbn.B = vec3_norm(
		vec3_sub(axis,
			vec3_mul_n(tbn.N, vec3_dot(axis, tbn.N)))
	);
	return (tbn);
}

t_tbn get_tbn_sphere(t_vec3 N)
{
    t_tbn tbn;

	tbn.N = N;
	tbn.T = vec3_norm((t_vec3){ -tbn.N.z, 0, tbn.N.x });
	tbn.B = vec3_cross(tbn.N, tbn.T);
	return (tbn);
}


t_vec3 apply_bump(t_tbn tbn, double u, double v,
	double (*height)(double, double))
{
	double	eps;
	double	scale;
	double	du;
	double	dv;
	t_vec3	bumped;

	eps = 0.001;
	scale = 0.1;
	du = height(u + eps, v) - height(u - eps, v);
	dv = height(u, v + eps) - height(u, v - eps);
	bumped = vec3_add(tbn.N, vec3_add(
			vec3_mul_n(tbn.T, du * scale),
			vec3_mul_n(tbn.B, dv * scale)));
	return (vec3_norm(bumped));
}


void apply_sphere_bump(t_hit_record *rec, t_sphere *sp)
{
    double u = rec->u;
    double v = rec->v;

    double du = 0.001;
    double dv = 0.001;

    double h  = sine_bump(u, v);
    double hu = sine_bump(u + du, v);
    double hv = sine_bump(u, v + dv);
	
	// double h  = turbulence(u, v, 5);
	// double hu = turbulence(u + du, v, 5);
	// double hv = turbulence(u, v + dv, 5);

	// double h  = checker_bump(u, v);
	// double hu = checker_bump(u + du, v);
	// double hv = checker_bump(u, v + dv);

	// double h  = noise_bump(u, v);
	// double hu = noise_bump(u + du, v);
	// double hv = noise_bump(u, v + dv);

    double dhdu = (hu - h) * BUMP_SCALE;
    double dhdv = (hv - h) * BUMP_SCALE;

	(void)sp; // suppress unused parameter warning

	t_vec3 N = rec->normal;
	t_vec3 T = vec3_norm((t_vec3){ -N.z, 0, N.x });
	t_vec3 B = vec3_cross(N, T);

    t_vec3 bumped = vec3_add(
        N,
        vec3_add(
            vec3_mul_n(T, dhdu),
            vec3_mul_n(B, dhdv)
        )
    );
    rec->normal = vec3_norm(bumped);
}
