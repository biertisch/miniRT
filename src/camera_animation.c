/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_animation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:24:04 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 13:45:23 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_auto_c(t_cam_auto *o, t_camera *c)
{
	o->angle = 0;
	o->speed = 0.21;
	o->radius = sqrt(c->lookfrom.x * c->lookfrom.x + c->lookfrom.z * c->lookfrom.z);
	o->height = c->lookfrom.y;
	o->center = (t_vec3){0, 0, 0};
}

void	update_orbit_camera(t_camera *cam, t_cam_auto *o)
{
	cam->lookfrom.x = o->center.x + o->radius * cos(o->angle);
	cam->lookfrom.z = o->center.z + o->radius * sin(o->angle);
	cam->lookfrom.y = o->center.y + o->height;

	cam->forword = vec3_norm(vec3_sub(o->center, cam->lookfrom));
	o->angle += o->speed;
	// if (o->angle > 2 * M_PI)
	// 	o->angle -= 2 * M_PI;
}
