/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 13:07:03 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/29 21:45:47 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	print_object_info(t_object *object, int index)
{
	printf("\n%d. ", index);
	if (object->type == PLANE)
		printf("Plane:\nPoint\t\t%.2f, %.2f, %.2f\n"
			"Normal\t\t%.4f, %.4f, %.4f\n", object->geo.plane.point.x,
			object->geo.plane.point.y, object->geo.plane.point.z,
			object->geo.plane.normal.x, object->geo.plane.normal.y,
			object->geo.plane.normal.z);
	else if (object->type == SPHERE)
		printf("Sphere:\nCenter\t\t%.2f, %.2f, %.2f\nDiameter\t%.2f\n",
			object->geo.sphere.center.x, object->geo.sphere.center.y,
			object->geo.sphere.center.z, object->geo.sphere.radius * 2);
	else if (object->type == CYLINDER)
		printf("Cylinder:\nCenter\t\t%.2f, %.2f, %.2f\n"
			"Axis\t\t%.4f, %.4f, %.4f\nDiameter\t%.2f\nHeight\t\t%.2f\n",
			object->geo.cylinder.center.x, object->geo.cylinder.center.y,
			object->geo.cylinder.center.z, object->geo.cylinder.axis.x,
			object->geo.cylinder.axis.y, object->geo.cylinder.axis.z,
			object->geo.cylinder.radius * 2, object->geo.cylinder.height);
	else if (object->type == CONE)
		printf("Cone:\nApex\t\t%.2f, %.2f, %.2f\nAxis\t\t%.4f, %.4f, %.4f\n"
			"Diameter\t%.2f\nHeight\t\t%.2f\n", object->geo.cone.apex.x,
			object->geo.cone.apex.y, object->geo.cone.apex.z,
			object->geo.cone.axis.x, object->geo.cone.axis.y,
			object->geo.cone.axis.z, object->geo.cone.radius * 2,
			object->geo.cone.height);
}

static void	print_light_info(t_s_light *light, int index)
{
	printf("\n%d. Light:\nPosition\t%.2f, %.2f, %.2f\n"
		"Brightness\t%.2f\nColor\t\t%.2f, %.2f, %.2f\n",
		index, light->position.x, light->position.y, light->position.z,
		light->brightness, light->color.r, light->color.g, light->color.b);
}

void	print_info(t_world *wld)
{
	int	i;

	printf("\n----------------------------------------\n"
		"\t\tINFO\n"
		"----------------------------------------\n"
		"Camera:\nOrigin\t\t%.2f, %.2f, %.2f\n"
		"Direction\t%.4f, %.4f, %.4f\n",
		wld->camera.lookfrom.x, wld->camera.lookfrom.y,
		wld->camera.lookfrom.z, wld->camera.forword.x,
		wld->camera.forword.y, wld->camera.forword.z);
	i = 0;
	while (i < wld->num_lights)
	{
		print_light_info(wld->lights[i], i);
		i++;
	}
	i = 0;
	while (i < wld->num_objects)
	{
		print_object_info(wld->objects[i], i + wld->num_lights);
		i++;
	}
}

void	print_help(void)
{
	printf("\n----------------------------------------\n"
		"\t\tHELP\n"
		"----------------------------------------\n"
		"print scene info\tp\n"
		"move camera\t\tw / s / a / d\n"
		"rotate camera\t\t↑ / ↓ / ← / →\n"
		"rotate scene\t\tr\n"
		"help\t\t\th\n"
		"quit\t\t\tesc\n");
}
