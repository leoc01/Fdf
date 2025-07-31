/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:45:35 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/25 18:45:37 by lbuscaro         ###   ########.fr       */
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
		if (map->point[i].bx > map->lim.x_max)
			map->lim.x_max = map->point[i].bx;
		if (map->point[i].bx < map->lim.x_min)
			map->lim.x_min = map->point[i].bx;
		if (map->point[i].by > map->lim.y_max)
			map->lim.y_max = map->point[i].by;
		if (map->point[i].by < map->lim.y_min)
			map->lim.y_min = map->point[i].by;
		i++;
	}
}

void	to_iso(t_map *map, float z_angle)
{
	int		i;
	float	cos_a;
	float	sin_a;
	float	rot_x;
	float	rot_y;

	cos_a = cos(z_angle);
	sin_a = sin(z_angle);
	i = 0;
	while (i < map->area)
	{
		rot_x = map->point[i].ax * cos_a - map->point[i].ay * sin_a;
		rot_y = map->point[i].ax * sin_a + map->point[i].ay * cos_a;
		map->point[i].bx = (rot_x - rot_y) * map->projection;
		map->point[i].by = (rot_x + rot_y);
		map->point[i].px = (rot_x - rot_y) * map->projection;
		map->point[i].py = (rot_x + rot_y);
		if (map->projection == 2)
			map->point[i].py -= map->point[i].az / map->z_fac;
		i++;
	}
}

void	scale(t_map *map, float zoom)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		map->point[i].bx *= zoom;
		map->point[i].by *= zoom;
		map->point[i].px *= zoom;
		map->point[i].py *= zoom;
		i++;
	}
}

void	shift(t_map *map, t_params p)
{
	int	i;

	p.cx = (map->lim.x_min + (map->lim.x_max - map->lim.x_min) / 2) - W / 2;
	p.cy = (map->lim.y_min + (map->lim.y_max - map->lim.y_min) / 2) - H / 2;
	i = 0;
	while (i < map->area)
	{
		map->point[i].bx = map->point[i].bx - p.cx + p.shx * p.zoom;
		map->point[i].by = map->point[i].by - p.cy + p.shy * p.zoom;
		map->point[i].px = map->point[i].px - p.cx + p.shx * p.zoom;
		map->point[i].py = map->point[i].py - p.cy + p.shy * p.zoom;
		i++;
	}
}

void	change_projection(int *projection)
{
	if (*projection == 1)
		*projection = 2;
	else
		*projection = 1;
}
