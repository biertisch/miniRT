/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:11:32 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:05 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//incorporate into main header?
#ifndef CONE_H
# define CONE_H

# include <math.h>
# include "libft.h"
# include "vec3.h"
# include "color.h"

typedef struct s_cone
{
	t_vec3	apex;
	t_vec3	axis;
	double	radius;
	double	height;
	t_color	color;
}	t_cone;

t_cone			new_cone(t_vec3 apex, t_vec3 axis, double radius, double height, t_color color);

#endif