/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:09:31 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 16:12:01 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	output_camera_info(t_camera *c)
{
	if (DEBUG == 0)
		return ;
	printf("Camera Info:\n");
	printf("  Lookfrom: (%f, %f, %f)\n", c->lookfrom.x,
		c->lookfrom.y, c->lookfrom.z);
	printf("  direction:   (%f, %f, %f)\n", c->forword.x,
		c->forword.y, c->forword.z);
	printf("  Vup:      (%f, %f, %f)\n", c->vup.x,
		c->vup.y, c->vup.z);
	printf("  U:        (%f, %f, %f)\n", c->u.x, c->u.y, c->u.z);
	printf("  V:        (%f, %f, %f)\n", c->v.x, c->v.y, c->v.z);
	printf("  W:        (%f, %f, %f)\n", c->w.x, c->w.y, c->w.z);
	printf("  FOV:      %f\n", c->vfov);
	printf("  Aspect Ratio: %f\n", (double)c->img_w / (double)c->img_h);
	printf("  Image Width:  %d\n", c->img_w);
	printf("  Image Height: %d\n", c->img_h);
	printf("  Max Depth:        %d\n", c->max_depth);
	printf("  Pixel00 Location:    (%f, %f, %f)\n", c->pix00_loc.x,
		c->pix00_loc.y, c->pix00_loc.z);
	printf("  Pixel Delta U:       (%f, %f, %f)\n", c->pix_delta_u.x,
		c->pix_delta_u.y, c->pix_delta_u.z);
	printf("  Pixel Delta V:       (%f, %f, %f)\n", c->pix_delta_v.x,
		c->pix_delta_v.y, c->pix_delta_v.z);
}
