/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 13:59:54 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 15:29:34 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	camera_action(void (*func)(t_camera *, t_direction, float),
	t_direction beheaver, t_world *wld, float val)
{
	func(&wld->camera, beheaver, val);
	wld->camera_auto.inited = 0;
	camera_light_initialize(wld);
	camera_render(&wld->camera, wld);
}

static	void	camera_move(t_camera *cam, t_direction direction, float dist)
{
	t_vec3	forward;
	t_vec3	right;

	if (direction == FORWARD_BACKWARD)
	{
		forward = cam->forword;
		cam->lookfrom = vec3_add(cam->lookfrom, vec3_mul_n(forward, dist));
	}
	else if (direction == LEFT_RIGHT)
	{
		forward = cam->forword;
		right = unit_vector(vec3_cross(forward, cam->vup));
		cam->lookfrom = vec3_add(cam->lookfrom, vec3_mul_n(right, dist));
	}
}

static void	camera_update_basis(t_camera *cam)
{
	t_vec3	world_up;

	world_up = new_vec3(0, 1, 0);
	if (fabs(vec3_dot(cam->forword, world_up)) > 0.999)
		world_up = new_vec3(0, 0, 1);
	cam->u = unit_vector(vec3_cross(world_up, cam->forword));
	cam->vup = vec3_cross(cam->forword, cam->u);
}

static	void	camera_rotate_pitch(t_camera *cam, t_direction dir, float angle)
{
	double	step;

	step = ((cam->vfov / 60.0 * STEP_ANGLE * M_PI / 180.0)) * angle;
	if (dir == UP_DOWN)
	{
		cam->pitch += step;
		if (cam->pitch > MAX_PITCH_ANGLE)
			cam->pitch = MAX_PITCH_ANGLE;
		if (cam->pitch < -MAX_PITCH_ANGLE)
			cam->pitch = -MAX_PITCH_ANGLE;
	}
	else if (dir == LEFT_RIGHT)
		cam->yaw += step;
	cam->forword.x = cos(cam->pitch) * cos(cam->yaw);
	cam->forword.y = sin(cam->pitch);
	cam->forword.z = cos(cam->pitch) * sin(cam->yaw);
	cam->forword = unit_vector(cam->forword);
	camera_update_basis(cam);
}

void	do_action(int keycode, t_world *wld)
{
	if (keycode == 65361)
		camera_action(camera_rotate_pitch, LEFT_RIGHT, wld, -1);
	else if (keycode == 65362)
		camera_action(camera_rotate_pitch, UP_DOWN, wld, 1);
	else if (keycode == 65363)
		camera_action(camera_rotate_pitch, LEFT_RIGHT, wld, 1);
	else if (keycode == 65364)
		camera_action(camera_rotate_pitch, UP_DOWN, wld, -1);
	else if (keycode == 'w')
		camera_action(camera_move, FORWARD_BACKWARD, wld, +STEP_MOVE);
	else if (keycode == 's')
		camera_action(camera_move, FORWARD_BACKWARD, wld, -STEP_MOVE);
	else if (keycode == 'a')
		camera_action(camera_move, LEFT_RIGHT, wld, -STEP_MOVE);
	else if (keycode == 'd')
		camera_action(camera_move, LEFT_RIGHT, wld, +STEP_MOVE);
	else
		extend_action(keycode, wld);
}

/*

double	get_rota_step(double fov)
{
	return (fov / 60.0 * STEP_ANGLE * M_PI / 180.0);
}
void	camera_move_forward_back(t_camera *cam, t_direction dir, float dist)
{
	t_vec3	forward;

	forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	cam->lookfrom = vec3_add(cam->lookfrom, vec3_multiply(forward, dist));
	cam->lookat = vec3_add(cam->lookat, vec3_multiply(forward, dist));
}

void	camera_move_right_left(t_camera *cam, float dist)
{
	t_vec3	forward;
	t_vec3	right;

	forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	right = unit_vector(vec3_cross(forward, cam->vup));
	cam->lookfrom = vec3_add(cam->lookfrom, vec3_multiply(right, dist));
	cam->lookat = vec3_add(cam->lookat, vec3_multiply(right, dist));
}
void	camera_rotate_left_right(t_camera *cam, t_direction dir, float angle)
{
	t_vec3	forward;
	t_vec3	right;
	double	cos_angle;
	double	sin_angle;
	t_vec3	new_forward;

	forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	right = unit_vector(vec3_cross(forward, cam->vup));
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	new_forward = vec3_add(vec3_multiply(forward, cos_angle),
			vec3_multiply(right, -sin_angle));
	cam->lookat = vec3_add(cam->lookfrom, new_forward);
}

void	camera_pitch_down(t_camera *cam, t_direction direction, float angle)
{
	t_vec3	forward;
	t_vec3	right;
	double	cos_angle;
	double	sin_angle;
	t_vec3	new_forward;

	forward = unit_vector(vec3_subtract(cam->lookat, cam->lookfrom));
	right = unit_vector(vec3_cross(forward, cam->vup));
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	new_forward = vec3_add(vec3_multiply(forward, cos_angle),
			vec3_multiply(cam->vup, sin_angle));
	cam->lookat = vec3_add(cam->lookfrom, new_forward);
	cam->vup = vec3_cross(right, new_forward);
}
*/