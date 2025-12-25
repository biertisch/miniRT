/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 11:27:54 by bliu              #+#    #+#             */
/*   Updated: 2025/12/25 11:34:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


int	handle_destroy(void *param)
{
	t_world	*wld;

	wld = (t_world *)param;
	printf("MAIN❌\n");
	free_all_the_world(wld);
	exit(0);
	return (0);
}

static int	mouse_hook(int keycode, void *param)
{
	t_world	*wld;

	wld = (t_world *)param;
	(void)wld;
	printf("Hello from key_hook![%d]\n", keycode);
	return (0);
}

static int	handle_pressed(int keycode, void *param)
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
