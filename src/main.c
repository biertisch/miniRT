/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 12:02:48 by bliu              #+#    #+#             */
/*   Updated: 2025/12/24 03:00:08 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// int	get_color_from_img(t_data *data, int x, int y)
// {
// 	char	*src;
// 	src = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
// 	return (*(unsigned int *)src);
// }

int	handle_destroy(void *param)
{
	t_world	*wld;

	wld = (t_world *)param;
	printf("MAIN❌\n");
	free_all_the_world(wld);
	exit(0);
	return (0);
}

int	mouse_hook(int keycode, void *param)
{
	t_world	*wld;

	wld = (t_world *)param;
	(void)wld;
	printf("Hello from key_hook![%d]\n", keycode);
	return (0);
}

int	handle_pressed(int keycode, void *param)
{
	t_world	*wld;

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

void	reg_hook(t_world *wld)
{
	mlx_hook(wld->win, KeyRelease, 1L << 1, handle_pressed, wld);
	mlx_hook(wld->win, DestroyNotify, 0, handle_destroy, wld);
	mlx_mouse_hook(wld->win, mouse_hook, wld);
	mlx_hook(wld->win, 4, 1L << 2, mouse_press, wld);
	mlx_loop_hook(wld->mlx, loop, wld);
}

void	add_test_objects(t_world *wld)
{
	t_sphere		sphere;

	sphere.center = (t_vec3){0,0,0};
	sphere.radius = 2.0;
	wld->pic_c_tex = picture_texture(load_xpm(wld->mlx, "textures/bumpearth.xpm"));
	// wld->bump_tex = load_xpm(wld->mlx, "textures/earth_bump.xpm");
	sphere.mat = get_material_texture(LAMBERTIAN,
			(t_texture *)&wld->pic_c_tex, 0);
	add_object_to_world(wld, SPHERE, &sphere);
}

int	main(int argc, char **argv)
{
	t_world		wld;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: miniRT xxx.rt\n", STDERR_FILENO);
		return (1);
	}
	ft_memset(&wld, 0, sizeof(t_world));
	if (!parser(&wld, argv[1]))
		return (1);
	camera_light_initialize(&wld);
	wld.mlx = mlx_init();
	wld.win = mlx_new_window(wld.mlx, wld.camera.img_w,
		wld.camera.img_h, "MiniRT");
	add_test_objects(&wld);
	reg_hook(&wld);
	camera_render(&(wld.camera), &wld);
	setup_controls(&wld);
	mlx_loop(wld.mlx);
	return (0);
}
