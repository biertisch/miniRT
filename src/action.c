#include "minirt.h"

void camera_move_forward(t_camera *cam, float dist)
{
    t_vec3 forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
    cam->lookfrom = vec3_add(cam->lookfrom, vec3_multiply(forward, dist));
    cam->lookat = vec3_add(cam->lookat, vec3_multiply(forward, dist));
}

void camera_move_right(t_camera *cam, float dist)
{
    t_vec3 forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
    t_vec3 right = unit_vector(vec3_cross(forward, cam->vup));
    cam->lookfrom = vec3_add(cam->lookfrom, vec3_multiply(right, dist));
    cam->lookat = vec3_add(cam->lookat, vec3_multiply(right, dist));
}

void	camera_rotate_left(t_camera *cam, float angle)
{
	t_vec3 forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	t_vec3 right = unit_vector(vec3_cross(forward, cam->vup));
	
	// Rotate forward vector around vup axis
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);
	t_vec3 new_forward = vec3_add(
		vec3_multiply(forward, cos_angle),
		vec3_multiply(right, -sin_angle)
	);
	cam->lookat = vec3_add(cam->lookfrom, new_forward);
}

void camera_pitch_down(t_camera *cam, float angle)
{
	t_vec3 forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	t_vec3 right = unit_vector(vec3_cross(forward, cam->vup));
	
	// Rotate forward vector around right axis
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);
	t_vec3 new_forward = vec3_add(
		vec3_multiply(forward, cos_angle),
		vec3_multiply(cam->vup, sin_angle)
	);
	cam->lookat = vec3_add(cam->lookfrom, new_forward);
	cam->vup = vec3_cross(right, new_forward);
}

void	camera_action(void (*func)(t_camera *, float), t_world *wld, float val)
{
	func(&wld->camera, val);
	camera_initialize(&wld->camera);
	camera_render(&wld->camera, wld);
}

void	resize_obj(t_world *wld, float scale)
{
	if(wld->current_obj)
	{
		if(wld->current_obj->type == SPHERE){
			wld->current_obj->geo.sphere.radius *= scale;
			camera_render(&wld->camera, wld);
		}else if (wld->current_obj->type == CYLINDER){
			wld->current_obj->geo.cylinder.radius *= scale;
			wld->current_obj->geo.cylinder.height *= scale;
			camera_render(&wld->camera, wld);
		}
	}
	else
		printf("Use mouse to select object first.\n");
}

static void	do_action(int keycode, t_world *wld)
{
	if (keycode == 65361)
	{
		camera_action(camera_rotate_left, wld, +0.1f);
		printf("⬅️\n");
	}
	else if (keycode == 65362)
	{
		camera_action(camera_pitch_down, wld, +0.1f);
		printf("⬆️\n");
	}
	else if (keycode == 65363)
	{
		camera_action(camera_rotate_left, wld, -0.1f);
		printf("➡️\n");
	}
	else if (keycode == 65364)
	{
		camera_action(camera_pitch_down, wld, -0.1f);
		printf("⬇️\n");
	}
	else if (keycode == 'w')
		camera_action(camera_move_forward, wld, +3.3f);
	else if (keycode == 's')
		camera_action(camera_move_forward, wld, -3.3f);
	else if (keycode == 'a')
		camera_action(camera_move_right, wld, -3.3f);
	else if (keycode == 'd')
		camera_action(camera_move_right, wld, +3.3f);
	else if (keycode == 'p')
	{
		printf("📷 Print Info:\n");
		printf("  Camera Position: (%.2f, %.2f, %.2f)\n", wld->camera.lookfrom.x, wld->camera.lookfrom.y, wld->camera.lookfrom.z);
		printf("  Camera Target:   (%.2f, %.2f, %.2f)\n", wld->camera.lookat.x, wld->camera.lookat.y, wld->camera.lookat.z);	
	}else if (keycode == '=')
		resize_obj(wld, 1.1f);
	else if (keycode == '-')
		resize_obj(wld, 0.9f);
}

int	handle_pressed(int keycode, void *param)
{
	t_world *wld;

	wld = (t_world *)param;
	if (DEBUG)
		printf("Key pressed: %d\n", keycode);
	if (keycode == 65307)
	{
		printf("ESC❌\n");
		free_all_the_world(wld);
		exit(0);
	}
	else
		do_action(keycode, wld);
	return (0);
}
