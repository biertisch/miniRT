#include "minirt.h"

t_ray rt_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray new_ray;

	new_ray.origin = origin;
	new_ray.direction = direction;
	return (new_ray);
}

t_vec3	ray_at(t_ray *ray, double t)
{
	t_vec3 point;

	point = vec3_add(ray->origin, vec3_mul_n(ray->direction, t));
	return (point);
}



// double	hit_sphere(t_vec3 center, double radius, t_ray *ray)
// {
// 	t_vec3	oc;
// 	double	a;
// 	double	h;
// 	double	c;
// 	double	discriminant;

// 	oc = vec3_subtract(center, ray->origin);
// 	a = vec3_length_squared(ray->direction);
// 	h = vec3_dot(oc, ray->direction);
// 	c = vec3_length_squared(oc) - radius * radius;
// 	discriminant = h * h - a * c;
// 	if (discriminant < 0)
// 		return (-1.0);
// 	else
// 		return (h - sqrt(discriminant)) / a;
// }


/*
void ray_tracing(void *mlx, void *window, t_scene *scene)
{
	int	mlx_x=0;
	int	mlx_y=0;
	double x_angle;
	double y_angle;
	int color;
	double y_ray;
	double x_ray;
	t_vec3 *ray;
	t_vplane *vplane;

	vplane = get_view_plane(scene->width, scene->hight, scene->camera->fovy);
	y_angle = (scene->hight / 2.0);
	while(y_angle >= -(scene->hight / 2.0))
	{
		y_ray = y_angle * vplane->y_pixel;
		x_angle = -(scene->width / 2.0);
		mlx_x = 0;
		while(x_angle<=(scene->width / 2.0))
		{
			x_ray = x_angle * vplane->x_pixel;
			ray = new_vec3(x_ray, y_ray, -1.0);	
			vec3_normalize(ray);
			// Simple shading based on ray direction
			if (sphere_intersect(scene->camera, ray, scene->sphere))
				color = 0xFF0000; // Red if intersects
			else
				color = 0x000000; // Black otherwise
			mlx_pixel_put(mlx, window, mlx_x, mlx_y, color);
			free(ray);
			x_angle += 1.0;
			mlx_x++;
			// printf("mlx_x: %d, mlx_y: %d\n", mlx_x, mlx_y);
		}
		y_angle -= 1.0;
		mlx_y++;
	}

}
*/