/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:24:39 by bliu              #+#    #+#             */
/*   Updated: 2025/12/20 19:56:56 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	new_vec3(double x, double y, double z)
{
	t_vec3	vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	return (vec);
}

t_vec3	vec3_mul_n(t_vec3 a, double scalar)
{
	t_vec3	result;

	result.x = a.x * scalar;
	result.y = a.y * scalar;
	result.z = a.z * scalar;
	return (result);
}

t_vec3	vec3_div(t_vec3 a, double scalar)
{
	t_vec3	result;

	if (scalar != 0)
	{
		result.x = a.x / scalar;
		result.y = a.y / scalar;
		result.z = a.z / scalar;
	}
	else
		printf("Warning: Division by zero in vec3_divide\n");
	return (result);
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

double	vec3_length_squared(t_vec3 vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

double	vec3_length(t_vec3 vec)
{
	return (sqrt(vec3_length_squared(vec)));
}

t_vec3	vec3_norm(t_vec3 vec)
{
	double	length;
	t_vec3	result;

	length = vec3_length(vec);
	if (length != 0)
		result = vec3_div(vec, length);
	else
		printf("Warning: Zero length vector passed to vec3_normalize\n");
	return (result);
}

double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_vec3	unit_vector(t_vec3 vec)
{
	t_vec3	result;
	double	length;

	length = vec3_length(vec);
	if (length != 0)
	{
		result.x = vec.x / length;
		result.y = vec.y / length;
		result.z = vec.z / length;
	}
	else
		printf("Warning: Zero length vector passed to unit_vector\n");
	return (result);
}

t_vec3	random_vec3_range(double min, double max)
{
	t_vec3	result;

	result.x = random_double_range(min, max);
	result.y = random_double_range(min, max);
	result.z = random_double_range(min, max);
	return (result);
}

t_vec3	random_unit_vec3(void)
{
	t_vec3	p;
	double	length_squared;

	while (42)
	{
		p = random_vec3_range(-1.0, 1.0);
		length_squared = vec3_length_squared(p);
		if (length_squared > 1e-160 && length_squared <= 1)
			return (vec3_mul_n(p, 1.0 / sqrt(length_squared)));
	}
}

t_vec3	vec3_reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mul_n(n, 2.0 * vec3_dot(v, n))));
}

t_vec3	vec3_refract(t_vec3 uv, t_vec3 n, double etai_over_etat)
{
	double	cos_theta;
	t_vec3	r_out_perp;
	t_vec3	r_out_parallel;

	cos_theta = fmin(vec3_dot(vec3_mul_n(uv, -1.0), n), 1.0);
	r_out_perp = vec3_mul_n(vec3_add(uv, vec3_mul_n(n, cos_theta)),
			etai_over_etat);
	r_out_parallel = vec3_mul_n(n, -sqrt(fabs(1.0
					- vec3_length_squared(r_out_perp))));
	return (vec3_add(r_out_perp, r_out_parallel));
}

t_vec3	random_cosine_direction(void)
{
	double	r2;
	double	phi;
	double	x;
	double	y;
	double	z;

	r2 = random_double();
	phi = 2 * M_PI * random_double();
	x = cos(phi) * sqrt(r2);
	y = sin(phi) * sqrt(r2);
	z = sqrt(1 - r2);
	return (new_vec3(x, y, z));
}

/*

t_vec3	random_vec3(void)
{
	t_vec3	result;

	result.x = random_double();
	result.y = random_double();
	result.z = random_double();
	return (result);
}
int	vec3_near_zero(t_vec3 vec)
{
	double	s;

	s = 1e-8;
	return (fabs(vec.x) < s && fabs(vec.y) < s && fabs(vec.z) < s);
}

t_vec3	vec3_clamp(t_vec3 v, double min, double max)
{
	if (v.x < min)
		v.x = min;
	if (v.x > max)
		v.x = max;
	if (v.y < min)
		v.y = min;
	if (v.y > max)
		v.y = max;
	if (v.z < min)
		v.z = min;
	if (v.z > max)
		v.z = max;
	return (v);
}
*/