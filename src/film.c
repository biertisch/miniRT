#include "minirt.h"

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	get_t(int trgb)
{
	return ((trgb >> 24) & 0xFF);
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

int	get_b(int trgb)
{
	return (trgb & 0xFF);
}

void	film_save(t_film *film)
{
	FILE	*fp;
	int		x;
	int		y;

	fp = fopen("output.ppm", "w");
	if (!fp)
	{
		perror("Failed to open file for writing");
		return ;
	}
	fprintf(fp, "P3\n%d %d\n255\n", film->width, film->height);
	for (y = 0; y < film->height; y++)
	{
		for (x = 0; x < film->width; x++)
		{
			int color = film->data[x][y];
			int r = get_r(color);
			int g = get_g(color);
			int b = get_b(color);
			fprintf(fp, "%d %d %d ", r, g, b);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}
