#include "fdf.h"

void	render(t_fdf *fdf)
{
	t_data	*dt;

	dt = &fdf->data;
	dt->img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	dt->addr = mlx_get_data_addr(dt->img, &dt->bpp, &dt->ln_len, &dt->endian);
	draw(&fdf->map, dt);
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, dt->img, 0, 0);
	mlx_destroy_image(fdf->mlx, dt->img);
}

void	draw(t_map *map, t_data *data)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		if (map->point[i].ax)
			d_line(data, map->point[i - 1], map->point[i]);
		if (map->point[i].ay)
			d_line(data, map->point[i], map->point[i - map->size_x]);
		i++;
	}
}
