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
	calculate_delta(fdf);
	fdf->params.zoom += fdf->params.zoom_dir * 8 * fdf->params.delta;
	fdf->params.shx += fdf->params.x_dir * sqrt(fdf->params.zoom) * 40 * fdf->params.delta;
	fdf->params.shy += fdf->params.y_dir * sqrt(fdf->params.zoom) * 40 * fdf->params.delta;
	fdf->params.z_angle += fdf->params.angle_dir * 0.5 * fdf->params.delta;
	to_iso(&fdf->map, (-M_PI / 2) + fdf->params.z_angle);
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

int	close_fdf(t_fdf *fdf, int code)
{
	if (fdf->map.point)
		free(fdf->map.point);
	if (fdf->mlx_win)
		mlx_destroy_window(fdf->mlx, fdf->mlx_win);
	if (fdf->mlx)
	{
		mlx_destroy_display(fdf->mlx);
		free(fdf->mlx);
	}
	exit(code);
}

int	key_press(int keysym, t_fdf *fdf)
{
	if (keysym == UP)
		fdf->params.zoom_dir = 1;
	if (keysym == DOWN)
		fdf->params.zoom_dir = -1;
	if (keysym == D_KEY)
		fdf->params.x_dir = 1;
	if (keysym == A_KEY)
		fdf->params.x_dir = -1;
	if (keysym == S_KEY)
		fdf->params.y_dir = 1;
	if (keysym == W_KEY)
		fdf->params.y_dir = -1;
	if (keysym == LEFT)
		fdf->params.angle_dir = 1;
	if (keysym == RIGHT)
		fdf->params.angle_dir = -1;
	printf("%d\n", keysym);
	return (0);
}

int	key_release(int keysym, t_fdf *fdf)
{
	if (keysym == UP)
		fdf->params.zoom_dir = 0;
	if (keysym == DOWN)
		fdf->params.zoom_dir = 0;
	if (keysym == ESC)
		close_fdf(fdf, 0);
	if (keysym == D_KEY)
		fdf->params.x_dir = 0;
	if (keysym == A_KEY)
		fdf->params.x_dir = 0;
	if (keysym == S_KEY)
		fdf->params.y_dir = 0;
	if (keysym == W_KEY)
		fdf->params.y_dir = 0;
	if (keysym == LEFT)
		fdf->params.angle_dir = 0;
	if (keysym == RIGHT)
		fdf->params.angle_dir = 0;
	return (0);
}

void	start(t_fdf *fdf, char *file)
{
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, 1);
	fdf->mlx_win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, 1);
	fdf->map = create_map(file);
	init_values(fdf);
	mlx_hook(fdf->mlx_win, 17, (1L<<17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L<<0), key_press, fdf);
	mlx_hook(fdf->mlx_win, 03, (1L<<1), key_release, fdf);
	mlx_loop_hook(fdf->mlx, loop, fdf);
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;
	
	if (argc != 2)
		return 1;
	start(&fdf, argv[1]);
	mlx_loop(fdf.mlx);
	return (0);
}
