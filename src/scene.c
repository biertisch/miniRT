#include "minirt.h"

t_scene	*new_scene(t_camera *camera, t_sphere *spheres, int num_spheres, int width, int height)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->camera = camera;
	scene->sphere = spheres;
	scene->num_spheres = num_spheres;
	scene->width = width;
	scene->hight = height;
	return (scene);
}