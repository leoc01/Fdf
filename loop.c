#include "fdf.h"

void	update(t_fdf *fdf)
{
	calculate_delta(fdf);
	if (fdf->map.limits.x_max - fdf->map.limits.x_min > WIDTH / 2 || fdf->params.zoom_dir > 0)
		fdf->params.zoom += fdf->params.zoom_dir * fdf->params.zoom * fdf->params.delta;
	fdf->params.shx += fdf->params.x_dir * fdf->params.delta * 400 / fdf->params.zoom;
	fdf->params.shy += fdf->params.y_dir * fdf->params.delta * 400 / fdf->params.zoom;
	fdf->params.z_angle += fdf->params.angle_dir * fdf->params.delta * 0.8;
	to_iso(&fdf->map, fdf->params.z_angle);
	scale(&fdf->map, fdf->params.zoom);
	set_limits(&fdf->map);
	shift(&fdf->map, fdf->params);
}

void	render(t_fdf *fdf)
{
	char	*fps;
	fdf->data.img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	fdf->data.addr = mlx_get_data_addr(fdf->data.img, &fdf->data.bpp, &fdf->data.line_length, &fdf->data.endian);
	draw(&fdf->map, &fdf->data);
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, fdf->data.img, 0, 0);
	fps = ft_itoa((int)fdf->params.fps);
	mlx_string_put(fdf->mlx, fdf->mlx_win, 10, 10, 0xFFFFFF, fps);
	free(fps);
	mlx_destroy_image(fdf->mlx, fdf->data.img);
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

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	calculate_delta(t_fdf *fdf)
{
	long long		now;
	static float	accumulator;
	static int		fps;
	
	now = get_time();
	fdf->params.delta = (now - fdf->params.last_frame_time) / 1000.0f;
	fdf->params.last_frame_time = now;
	accumulator += fdf->params.delta;
	fps++;
	if (accumulator >= 1.0f)
	{
		fdf->params.fps = fps;
		fps = 0;
		accumulator = 0;	
	}
}
