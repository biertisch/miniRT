/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 12:25:38 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/28 12:28:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controls.h"

t_mat3	mat3_identity(void)
{
	return ((t_mat3){{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		}});
}

t_mat3	mat3_mult(t_mat3 a, t_mat3 b)
{
	t_mat3	res;
	int		i;
	int		j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			res.m[i][j] = a.m[i][0] * b.m[0][j]
				+ a.m[i][1] * b.m[1][j]
				+ a.m[i][2] * b.m[2][j];
			j++;
		}
		i++;
	}
	return (res);
}

t_vec3	mat3_mult_vec3(t_mat3 m, t_vec3 v)
{
	return ((t_vec3){
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
	});
}
