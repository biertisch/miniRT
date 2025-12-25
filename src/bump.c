/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:26:32 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 16:50:42 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	double	scale;
	int		check_u;
	int		check_v;

	scale = 10.0;
	check_u = (int)(u * scale);
	check_v = (int)(v * scale);
	if ((check_u + check_v) % 2 == 0)
		return (0.1);
	else
		return (-0.1);
}

double	sine_bump(double u, double v)
{
	double	freq;

	freq = 20.0;
	return (sin(u * freq * 2 * M_PI) * sin(v * freq * 2 * M_PI));
}

t_vec3	apply_bump_f(t_tbn tbn, double u, double v,
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
	bumped = vec3_add(tbn.cn, vec3_add(
				vec3_mul_n(tbn.ct, du * scale),
				vec3_mul_n(tbn.cb, dv * scale)));
	return (vec3_norm(bumped));
}

/*

t_bump_tex	load_texture(void *mlx, char *path)
{
	t_bump_tex	tex;

	tex.img = mlx_xpm_file_to_image(mlx, path, &tex.width, &tex.height);
	tex.addr = mlx_get_data_addr(tex.img, &tex.bpp, &tex.line_len, &tex.endian);
	return (tex);
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
*/