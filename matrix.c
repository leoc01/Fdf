/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_limits(t_map *map)
{
	int	i;

	i = 0;
	map->lim.x_max = 0;
	map->lim.x_min = 0;
	map->lim.y_max = 0;
	map->lim.y_min = 0;
	while (i < map->area)
	{
		if (map->point[i].px > map->lim.x_max)
			map->lim.x_max = map->point[i].px;
		if (map->point[i].px < map->lim.x_min)
			map->lim.x_min = map->point[i].px;
		if (map->point[i].py > map->lim.y_max)
			map->lim.y_max = map->point[i].py;
		if (map->point[i].py < map->lim.y_min)
			map->lim.y_min = map->point[i].py;
		i++;
	}
}

void	to_iso(t_map *map, float z_angle)
{
	int		i;
	float	cos_a;
	float	sin_a;
	float	rotated_x;
	float	rotated_y;

	cos_a = cos(z_angle);
	sin_a = sin(z_angle);
	i = 0;
	while (i < map->area)
	{
		rotated_x = map->point[i].ax * cos_a - map->point[i].ay * sin_a;
		rotated_y = map->point[i].ax * sin_a + map->point[i].ay * cos_a;
		map->point[i].px = (rotated_x - rotated_y) * 2;
		map->point[i].py = (rotated_x + rotated_y) - map->point[i].az / Z_FAC;
		i++;
	}
}

void	scale(t_map *map, float zoom)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		map->point[i].px *= zoom;
		map->point[i].py *= zoom;
		i++;
	}
}

void	shift(t_map *map, t_params params)
{
	int			i;
	t_limits	*lim;	

	lim = &map->lim;
	params.cx = (lim->x_min + (lim->x_max - lim->x_min) / 2) - W / 2;
	params.cy = (lim->y_min + (lim->y_max - lim->y_min) / 2) - H / 2;
	i = 0;
	while (i < map->area)
	{
		map->point[i].px = map->point[i].px - params.cx;
		map->point[i].py = map->point[i].py - params.cy;
		i++;
	}
}
