#include "fdf.h"

void	start(t_fdf *fdf, char *file)
{
	int	fd;

	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, 1);
	fdf->mlx_win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, 1);
	fd = open(file, O_RDONLY);
	if (fd < 3)
		close_fdf(fdf, 1);
	if (!create_map(&fdf->map, fd))
		close_fdf(fdf, 1);
	fd = close(fd);
	fd = open(file, O_RDONLY);
	if (fd < 3)
		close_fdf(fdf, 1);
	if (!get_points(&fdf->map, fd))
		close_fdf(fdf, 1);
	fd = close(fd);
	init_params(fdf);
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	mlx_hook(fdf->mlx_win, 03, (1L << 1), key_release, fdf);
	mlx_loop_hook(fdf->mlx, loop, fdf);
}

int	get_points(t_map *map, int fd)
{
	char	*row_line;
	char	**row;
	int		i;
	int		j;
	char	**color;

	row_line = get_next_line(fd);
	j = 0;
	while (row_line)
	{
		row = ft_split(row_line, ' ');
		free(row_line);
		i = 0;
		while (i < map->size_x)
		{
			map->point[i + (j * map->size_x)].ax = i;
			map->point[i + (j * map->size_x)].ay = j;
			map->point[i + (j * map->size_x)].az = ft_atoi(row[i]);
			map->point[i + (j * map->size_x)].color = hex_to_color("0xFFFFFF");
			color = ft_split(row[i], ',');
			free(color[0]);
			if (color[1])
			{
				map->point[i + (j * map->size_x)].color = hex_to_color(color[1]);
				free(color[1]);
			}
			free(color);
			free(row[i]);
			i++;
		}
		free(row);
		j++;
		row_line = get_next_line(fd);
	}
	return (1);
}

int	create_map(t_map *map, int fd)
{
	char	*row;
	char	**column;

	map->point = NULL;
	map->size_x = 0;
	map->size_y = 0;
	row = get_next_line(fd);
	column = ft_split(row, ' ');
	while (column[map->size_x])
		free(column[map->size_x++]);
	free(column);
	map->size_y++;
	while (row)
	{
		free(row);
		row = get_next_line(fd);
		map->size_y++;
	}
	map->area = map->size_x * map->size_y;
	map->point = ft_calloc(map->size_x * map->size_y, sizeof(t_point));
	return (1);
}

void	init_params(t_fdf *fdf)
{
	fdf->params.last_frame_time = get_time();
	fdf->params.delta = 0;
	fdf->params.zoom_dir = 0;
	fdf->params.x_dir = 0;
	fdf->params.y_dir = 0;
	fdf->params.z_angle = 0;
	fdf->params.angle_dir = 0;
	to_iso(&fdf->map, (-M_PI / 2) * fdf->params.z_angle);
	set_limits(&fdf->map);
	fdf->params.zoom = (WIDTH - PADDING * 2.0f) / fabs(fdf->map.limits.x_max - fdf->map.limits.x_min);
	if (WIDTH / fabs(fdf->map.limits.x_max - fdf->map.limits.x_min) > HEIGHT / fabs(fdf->map.limits.y_max - fdf->map.limits.y_min))
		fdf->params.zoom = (HEIGHT - PADDING * 2.0f) / fabs(fdf->map.limits.y_max - fdf->map.limits.y_min);
	fdf->params.shx = 0;
	fdf->params.shy = 0;
	scale(&fdf->map, fdf->params.zoom);
	set_limits(&fdf->map);
	shift(&fdf->map, fdf->params);
}
