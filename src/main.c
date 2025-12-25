/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 12:02:48 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 16:39:46 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_world	*world(void)
{
	static t_world	wld;

	return (&wld);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Usage: miniRT xxx.rt\n", STDERR_FILENO);
		return (1);
	}
	ft_memset(world(), 0, sizeof(t_world));
	world()->mlx = mlx_init();
	load_default_textures(world());
	if (!parser(world(), argv[1]))
		return (1);
	camera_light_initialize(world());
	world()->win = mlx_new_window(world()->mlx, world()->camera.img_w,
		world()->camera.img_h, "MiniRT");
	reg_hook(world());
	camera_render(&(world()->camera), world());
	setup_controls(world());
	mlx_loop(world()->mlx);
	return (0);
}

/*
void	add_test_objects(t_world *wld)
{
	t_sphere		sphere;

	sphere.center = (t_vec3){0, 0, 0};
	sphere.radius = 2.0;
	wld->pic_c_tex = picture_texture(load_xpm(wld->mlx,
				"textures/earthmap4k.xpm"));
	wld->pic_c_tex.bump_tex = load_xpm(wld->mlx,
			"textures/earthmap4kspecular.xpm");
	sphere.mat = get_material_texture(LAMBERTIAN,
			(t_texture *)&wld->pic_c_tex, 0);
	add_object_to_world(wld, SPHERE, &sphere);
}
*/