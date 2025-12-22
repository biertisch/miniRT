/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:26:32 by bliu              #+#    #+#             */
/*   Updated: 2025/12/22 22:44:45 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


double	noise_bump(double u, double v)
{
	// Placeholder for a noise function; in practice, use Perlin or Simplex noise
	// return (rand() / (double)RAND_MAX) * 2.0 - 1.0; 
	return fmod(sin(u*12.9898 + v*78.233)*43758.5453, 1.0);
}

double	turbulence(double u, double v, int depth)
{
	double sum = 0.0;
	double freq = 1.0;
	double amp = 1.0;
	double max = 0.0;

	for (int i = 0; i < depth; i++)
	{
		sum += fabs(noise_bump(u * freq, v * freq)) * amp;
		max += amp;
		freq *= 2.0;
		amp *= 0.5;
	}
	return sum / max; // Normalize to [0,1]
}

double	checker_bump(double u, double v)
{
	double scale = 10.0;
	int check_u = (int)(u * scale);
	int check_v = (int)(v * scale);
	if ((check_u + check_v) % 2 == 0)
		return 0.1;
	else
		return -0.1;
}

double sine_bump(double u, double v)
{
    double freq = 20.0;
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

    return tbn;
}

t_tbn get_tbn_cone(t_vec3 P, t_vec3 axis)
{
    t_tbn tbn;

    tbn.T = vec3_norm((t_vec3){-P.z, 0, P.x});
    tbn.B = vec3_norm(
        vec3_sub(axis,
            vec3_mul_n(tbn.N, vec3_dot(axis, tbn.N)))
    );

    return tbn;
}

t_tbn get_tbn_sphere(t_vec3 N)
{
    t_tbn tbn;
    double phi = atan2(N.z, N.x);
    double theta = acos(N.y);

    tbn.N = N;
    tbn.T = (t_vec3){-sin(phi), 0, cos(phi)};
    tbn.B = (t_vec3){
        cos(theta) * cos(phi),
        -sin(theta),
        cos(theta) * sin(phi)
    };

    tbn.T = vec3_norm(tbn.T);
    tbn.B = vec3_norm(tbn.B);
    return tbn;
}


t_vec3 apply_bump(t_tbn tbn, double u, double v, double (*height)(double, double))
{
    double eps = 0.001;
    double scale = 0.1;

    double du = height(u + eps, v) - height(u - eps, v);
    double dv = height(u, v + eps) - height(u, v - eps);

    t_vec3 bumped = vec3_add(
        tbn.N,
        vec3_add(
            vec3_mul_n(tbn.T, du * scale),
            vec3_mul_n(tbn.B, dv * scale)
        )
    );
    return vec3_norm(bumped);
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
