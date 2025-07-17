/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	start(t_fdf *fdf, char *file)
{
	int	fd;

	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, 1);
	fdf->mlx_win = mlx_new_window(fdf->mlx, W, H, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, 1);
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	fd = open(file, O_RDONLY);
	if (fd < 3 || !create_map(&fdf->map, fd))
	{
		fdf->map.point = NULL;
		close_fdf(fdf, 1);
	}
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
	char	*line;
	char	**row;
	int		i;
	int		j;

	i = 0;
	while (i + 1 < map->size_y)
	{
		line = get_next_line(fd);
		row = ft_split(line, ' ');
		free(line);
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
	float		dx_rel;
	float		dy_rel;
	t_map		*map;
	t_params	*params;

	map = &fdf->map;
	params = &fdf->params;
	to_iso(map, (-M_PI / 2));
	set_limits(map);
	dx_rel = W / fabs(map->lim.x_max - map->lim.x_min);
	dy_rel = H / fabs(map->lim.y_max - map->lim.y_min);
	params->zoom = (W - P * 2.0f) / fabs(map->lim.x_max - map->lim.x_min);
	if (dx_rel > dy_rel)
		params->zoom = (H - P * 2.0f) / fabs(map->lim.y_max - map->lim.y_min);
	scale(map, params->zoom);
	set_limits(map);
	shift(map, *params);
}
