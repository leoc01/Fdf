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
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	fd = open(file, O_RDONLY);
	if (fd < 3 || !create_map(&fdf->map, fd))
		close_fdf(fdf, 1);
	fd = close(fd);
	fd = open(file, O_RDONLY);
	if (fd < 3 || !get_points(&fdf->map, fd))
		close_fdf(fdf, 1);
	fd = close(fd);
	init_params(fdf);
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

t_color	set_color(char *point)
{
	char	**p_color;
	char	*trim_color;
	t_color	color;

	color = hex_to_color("0xFFFFFF");
	p_color = ft_split(point, ',');
	free(p_color[0]);
	if (p_color[1])
	{
		trim_color = ft_strtrim(p_color[1], "\n");
		color = hex_to_color(trim_color);
		free(trim_color);
		free(p_color[1]);
	}
	free(p_color);
	return (color);
}

int	get_points(t_map *map, int fd)
{
	char	*row_line;
	char	**row;
	int		i;
	int		j;

	i = 0;
	while (i + 1 < map->size_y)
	{
		row_line = get_next_line(fd);
		row = ft_split(row_line, ' ');
		free(row_line);
		j = 0;
		while (j < map->size_x)
		{
			map->point[j + (i * map->size_x)].ax = j;
			map->point[j + (i * map->size_x)].ay = i;
			map->point[j + (i * map->size_x)].az = ft_atoi(row[j]);
			map->point[j + (i * map->size_x)].color = set_color(row[j]);
			free(row[j]);
			j++;
		}
		free(row);
		i++;
	}
	return (1);
}

void	init_params(t_fdf *fdf)
{
	to_iso(&fdf->map, (-M_PI / 2));
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
