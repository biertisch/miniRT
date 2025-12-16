#include "minirt.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	get_color_from_img(t_data *data, int x, int y)
{
	char	*src;

	src = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)src);
}

void free_all_the_world(t_world *wld)
{
	if (wld)
	{
		if (wld->mlx && wld->win)
			mlx_destroy_window(wld->mlx, wld->win);
		if (wld->mlx)
		{
			mlx_destroy_display(wld->mlx);
			free(wld->mlx);
		}
	}
}

int	handle_destroy(void *param)
{
	t_world *wld;

	wld = (t_world *)param;
	printf("MAIN❌\n");
	free_all_the_world(wld);
	exit(0);
	return (0);
}

int	mouse_hook(int keycode, void *param)
{
	t_world *wld;

	wld = (t_world *)param;
	(void)wld;
	printf("Hello from key_hook![%d]\n", keycode);
	return (0);
}


int	has_object_was_clicked(t_world *world, t_ray *ray, t_interval ray_t, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;

	hit_anything = 0;
	closest_so_far = ray_t.max;
	int i = 0;
	while (i < world->num_objects)
	{
		current = world->objects[i];
		ray_t.max = closest_so_far;
		
		if (current->hit && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			world->current_obj = current;
		}
		i++;
	}
	return (hit_anything);
}

t_ray	get_ray_with_screenxy(int pixel_x, int pixel_y, t_camera *camera)
{
	t_vec3	pixel_sample;
	t_vec3	ray_direction;

	pixel_sample = vec3_add(camera->pixel00_loc,
					vec3_add(vec3_multiply(camera->pixel_delta_u, pixel_x),
							 vec3_multiply(camera->pixel_delta_v, pixel_y)));
	ray_direction = vec3_subtract(pixel_sample, camera->lookfrom);
	return (rt_ray(camera->lookfrom, ray_direction));
}

int mouse_press(int button, int x, int y, void *param)
{
	t_world	*wld;
	t_ray	ray;
	t_hit_record	rec;

	wld = (t_world *)param;

	ray = get_ray_with_screenxy(x, y, &wld->camera);
	if(has_object_was_clicked(wld, &ray, (t_interval){0.001, INFINITY}, &rec))
	{
		if(wld->current_obj){
			printf("hit ojb type:%d\n", wld->current_obj->type);
		}else{
			printf("hit obj is NULL\n");
		}
	}
    // printf("Mouse button %d pressed at (%d, %d)\n", button, x, y);
    return (0);
}

void	reg_hook(t_world *wld)
{
	mlx_hook(wld->win, KeyRelease, 1L << 1, handle_pressed, wld);
	mlx_hook(wld->win, DestroyNotify, 0, handle_destroy, wld);
	mlx_mouse_hook(wld->win, mouse_hook, wld);
	mlx_hook(wld->win, 4, 1L << 2, mouse_press, wld);
	// mlx_loop_hook(wld->mlx, loop_hook, wld);
}

void	docoloralignment(t_world *wld)
{
	wld->ambient = get_normalize_color(wld->ambient);
	wld->spot_light.light_color = get_normalize_color(wld->spot_light.light_color);
}

int main(int argc, char **argv)
{
	t_world		wld;
	t_quad		lights;
	t_object	light_obj;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: miniRT xxx.rt\n", STDERR_FILENO);
		return (1);
	}
	ft_memset(&wld, 0, sizeof(t_world));
	if (9)
		use_test_scene(&wld, 9);
	else
	{
		if (!parser(&wld, argv[1]))
			return (1);
		docoloralignment(&wld);
	}
	camera_initialize(&wld.camera);
	wld.mlx = mlx_init();
	wld.win = mlx_new_window(wld.mlx, wld.camera.image_width, wld.camera.image_height, "MiniRT");
	reg_hook(&wld);
	camera_render(&(wld.camera), &wld);
	// setup_controls(&wld);
	mlx_loop(wld.mlx);
	return (0);
}
