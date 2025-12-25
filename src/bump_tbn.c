/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_tbn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 13:49:32 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 14:50:26 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_tbn	get_tbn_plane(t_vec3 normal)
{
	t_tbn	tbn;
	t_vec3	up;

	tbn.cn = vec3_norm(normal);
	if (fabs(tbn.cn.y) < 0.999)
		up = (t_vec3){0, 1, 0};
	else
		up = (t_vec3){1, 0, 0};
	tbn.ct = vec3_norm(vec3_cross(up, tbn.cn));
	tbn.cb = vec3_cross(tbn.cn, tbn.ct);
	return (tbn);
}

t_tbn	get_tbn_cylinder(t_vec3 P, t_vec3 axis)
{
	t_tbn	tbn;

	tbn.cn = vec3_norm((t_vec3){P.x, 0, P.z});
	tbn.ct = vec3_norm((t_vec3){-P.z, 0, P.x});
	tbn.cb = axis;
	return (tbn);
}

t_tbn	get_tbn_cone(t_vec3 P, t_cone *cone)
{
	// t_tbn	tbn;

	// tbn.ct = vec3_norm((t_vec3){-P.z, 0, P.x});
	// tbn.cb = vec3_norm(vec3_sub(axis,
	// 			vec3_mul_n(tbn.cn, vec3_dot(axis, tbn.cn))));
	// return (tbn);
	t_tbn	tbn;
	t_vec3	w;
	double	t;
	t_vec3	Q;
	double	k;

	w = vec3_sub(P, cone->apex);
	t = vec3_dot(w, cone->axis);

	Q = vec3_add(cone->apex, vec3_mul_n(cone->axis, t));
	k = cone->radius / cone->height;
	tbn.cn = vec3_norm(vec3_sub(vec3_sub(P, Q),
				vec3_mul_n(cone->axis, k * k * t)));
	tbn.ct = vec3_norm(vec3_cross(cone->axis, tbn.cn));
	tbn.cb = vec3_cross(tbn.cn, tbn.ct);
	return (tbn);
}

t_tbn	get_tbn_sphere(t_vec3 normal)
{
	t_tbn	tbn;

	// tbn.cn = N;
	// tbn.ct = vec3_norm((t_vec3){-tbn.cn.z, 0, tbn.cn.x});
	// tbn.cb = vec3_cross(tbn.cn, tbn.ct);
	t_vec3	up;

	tbn.cn = vec3_norm(normal);
	if (fabs(tbn.cn.y) < 0.999)
		up = (t_vec3){0, 1, 0};
	else
		up = (t_vec3){1, 0, 0};
	tbn.ct = vec3_norm(vec3_cross(up, tbn.cn));
	tbn.cb = vec3_cross(tbn.cn, tbn.ct);
	return (tbn);
}
