/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_picture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:41:23 by bliu              #+#    #+#             */
/*   Updated: 2025/12/24 03:24:23 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


t_bump_tex	load_xpm(void *mlx, char *path)
{
	t_bump_tex	tex;

	tex = (t_bump_tex){0};
	tex.img = mlx_xpm_file_to_image(mlx, path, &tex.width, &tex.height);
	tex.addr = mlx_get_data_addr(tex.img, &tex.bpp, &tex.line_len, &tex.endian);
	return (tex);
}

t_color	texture_sample(t_bump_tex *tex, double u, double v)
{
	int				x;
	int				y;
	unsigned int	pixel;
	t_color			c;

	x = (int)(u * (tex->width - 1));
	y = (int)((v) * (tex->height - 1));
	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	pixel = *(unsigned int *)(tex->addr + y * tex->line_len
			+ x * (tex->bpp / 8));
	c.r = ((pixel >> 16) & 0xFF) / 255.0;
	c.g = ((pixel >> 8) & 0xFF) / 255.0;
	c.b = (pixel & 0xFF) / 255.0;
	return (c);
}

t_color	texture_map_value(t_texture *self, double u, double v, t_vec3 p)
{
	t_pic_tex	*texture;

	(void)p;
	texture = (t_pic_tex *)self;
	return (texture_sample(&texture->pic_tex, u, v));
}

t_pic_tex		picture_texture(t_bump_tex tex)
{
	t_pic_tex	texture;

	texture.base.value = &texture_map_value;
	texture.base.type = PICTURE;
	texture.pic_tex = tex;
	texture.albedo = get_color(1.0, 1.0, 1.0);
	return (texture);
}
