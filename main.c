#include "fdf.h"

void	draw(t_map *map, t_data *data)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		if (map->point[i].ax)
			d_line(data, map->point[i-1], map->point[i], 0x00FFFFFF);
		if (map->point[i].ay)
			d_line(data, map->point[i], map->point[i-map->size_x], 0x00FFFFFF);
		i++;
	}
}

void	update(t_fdf *fdf)
{
	to_iso(&fdf->map, (-M_PI / 2) + fdf->params.angle);
	scale(&fdf->map, fdf->params.zoom);
	set_limits(&fdf->map);
	shift(&fdf->map, fdf->params);
}

void	render(t_fdf *fdf)
{
	fdf->data.img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	fdf->data.addr = mlx_get_data_addr(fdf->data.img, &fdf->data.bits_per_pixel, &fdf->data.line_length, &fdf->data.endian);
	draw(&fdf->map, &fdf->data); 
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, fdf->data.img, 0, 0);
	mlx_destroy_image(fdf->mlx, fdf->data.img);
}
int	loop(t_fdf *fdf)
{
	update(fdf);
	render(fdf);
	return (1);
}

int	key_hook(int keysym, t_fdf *fdf)
{
	if (keysym == 65362)
		fdf->params.zoom *= 1.1;
	if (keysym == 65364)
		fdf->params.zoom /= 1.1;
	if (keysym == 100)
		fdf->params.shx -= 10;
	if (keysym == 97)
		fdf->params.shx += 10;
	if (keysym == 115)
		fdf->params.shy -= 10;
	if (keysym == 119)
		fdf->params.shy += 10;
	if (keysym == 65361)
		fdf->params.angle -= M_PI / 16;
	if (keysym == 65363)
		fdf->params.angle += M_PI / 16;
	printf("%d\n", keysym);
	return (0);
}

void	start(t_fdf *fdf, char *file)
{
	fdf->mlx = mlx_init();
	fdf->mlx_win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "Fdf");
	fdf->map = create_map(file);
	init_values(fdf);
	mlx_key_hook(fdf->mlx_win, key_hook, fdf);
	mlx_loop_hook(fdf->mlx, loop, fdf);
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;
	
	if (argc != 2)
		return 1;
	start(&fdf, argv[1]);
	mlx_loop(fdf.mlx);
	return(0);
}
