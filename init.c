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
	char	*content;

	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, 1);
	fdf->mlx_win = mlx_new_window(fdf->mlx, W, H, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, 1);
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	content = store_content(file);
	if (!create_map(&fdf->map, content))
	{
		fdf->map.point = NULL;
		close_fdf(fdf, 1);
	}
	if (!get_points(&fdf->map, content))
		close_fdf(fdf, 1);
	free(content);
	init_params(fdf);
}

int	create_map(t_map *map, char *content)
{
	int	x;

	map->size_x = 0;
	map->size_y = 0;
	x = 0;
	while (content[0])
	{
		while (content[0] == ' ')
			content++;
		while (content[0] != ' ' && content[0] != '\n')
			content++;
		map->size_x++;
		if (content[0] == '\n')
		{
			if (x == 0)
				x = map->size_x;
			map->size_x = 0;
			map->size_y++;
		}
		content++;
	}
	map->size_x = x;
	map->area = map->size_x * map->size_y;
	map->point = ft_calloc(map->area, sizeof(t_point));
	return (1);
}

t_color	set_color(char *content)
{
	char	str_color[9];
	int		i;
	t_color	color;

	if (content[0] != '0' || ft_toupper(content[1]) != 'X')
		return (hex_to_color("0XFFFFFF"));
	i = 0;
	while (i < 9)
	{
		if (content[0] != ' ' && content[0] != '\n')
		{
			str_color[i] = content[0];
			content++;
		}
		else
			str_color[i] = '\0';
		i++;
	}
	color = hex_to_color(str_color);
	return (color);
}

int	save_point(t_map *map, char *content, int i, int j)
{
	int	c;

	c = 0;
	while (content[c] == ' ' || content[c] == '\n')
		c++;
	map->point[j + (i * map->size_x)].ax = j;
	map->point[j + (i * map->size_x)].ay = i;
	map->point[j + (i * map->size_x)].az = ft_atoi(&content[c]);
	while (content[c] >= '0' && content[c] <= '9')
		c++;
	if (content[c] == ',')
		c++;
	map->point[j + (i * map->size_x)].color = set_color(&content[c]);
	while (content[c] != ' ' && content[c] != '\n')
		c++;
	return (c);
}

int	get_points(t_map *map, char *content)
{
	int		i;
	int		j;
	int		c;

	c = 0;
	i = 0;
	while (i < map->size_y)
	{
		j = 0;
		while (j < map->size_x)
		{
			c = save_point(map, content, i, j);
			content += c;
			j++;
		}
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
