/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_tbn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 13:49:32 by bliu              #+#    #+#             */
/*   Updated: 2025/12/27 11:03:44 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_tbn	plane_tbn(t_vec3 normal)
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

t_tbn	cylinder_tbn(t_vec3 p, t_vec3 c, t_vec3 axis)
{
	t_tbn	tbn;
	t_vec3	cp;
	double	h;
	t_vec3	q;

	cp = vec3_sub(p, c);
	h = vec3_dot(cp, axis);
	q = vec3_add(c, vec3_mul_n(axis, h));
	tbn.cn = vec3_norm(vec3_sub(p, q));
	tbn.cb = axis;
	tbn.ct = vec3_norm(vec3_cross(tbn.cb, tbn.cn));
	tbn.cn = vec3_norm(vec3_cross(tbn.ct, tbn.cb));
	return (tbn);
}

t_tbn	cone_tbn(t_vec3 P, t_cone *cone)
{
	t_tbn	tbn;
	t_vec3	w;
	double	t;
	t_vec3	cq;
	double	k;

	w = vec3_sub(P, cone->apex);
	t = vec3_dot(w, cone->axis);
	cq = vec3_add(cone->apex, vec3_mul_n(cone->axis, t));
	k = cone->radius / cone->height;
	tbn.cn = vec3_norm(vec3_sub(vec3_sub(P, cq),
				vec3_mul_n(cone->axis, k * k * t)));
	tbn.ct = vec3_norm(vec3_cross(cone->axis, tbn.cn));
	tbn.cb = vec3_cross(tbn.cn, tbn.ct);
	return (tbn);
}

t_tbn	sphere_tbn(t_vec3 normal)
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
