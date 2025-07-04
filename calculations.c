#include "fdf.h"

void	to_iso(t_map *map)
{
	int i;

	i = 0;
	while (i < map->area)
	{
		map->point[i].px = (map->point[i].ax + map->point[i].ay) * 2;
		map->point[i].py = (map->point[i].ay - map->point[i].ax) - map->point[i].az / Z_FAC;
		i++;
	}
}

void	set_limits(t_map *map)
{
	int	i;

	i = 1;
	map->limits.x_max = 0;
	map->limits.x_min = 0;
	map->limits.y_max = 0;
	map->limits.y_min = 0;
	while (i < map->area)
	{
		if (map->point[i].px > map->limits.x_max)
			map->limits.x_max = map->point[i].px;
		if (map->point[i].px < map->limits.x_min)
			map->limits.x_min = map->point[i].px;
		if (map->point[i].py > map->limits.y_max)
			map->limits.y_max = map->point[i].py;
		if (map->point[i].py < map->limits.y_min)
			map->limits.y_min = map->point[i].py;
		i++;
	}
}

void	scale(t_map *map, float zoom)
{
	int	i;
	float	sc;

	set_limits(map);
	sc = (WIDTH - PADDING * 2.0f) / fabs(map->limits.x_max - map->limits.x_min);
	if (WIDTH / fabs(map->limits.x_max - map->limits.x_min) > HEIGHT / fabs(map->limits.y_max - map->limits.y_min))
		sc = (HEIGHT - PADDING * 2.0f) / fabs(map->limits.y_max - map->limits.y_min);
	sc *= zoom;
	i = 0;
	while (i < map->area)
	{
		map->point[i].px *= sc;
		map->point[i].py *= sc;
		i++;
	}
}

void	shift(t_map *map)
{
	int	i;
	int	shx;
	int	shy;

	set_limits(map);
	shx = (map->limits.x_min + (map->limits.x_max - map->limits.x_min)/2) - WIDTH/2;
	shy = (map->limits.y_min + (map->limits.y_max - map->limits.y_min)/2) - HEIGHT/2;
	i = 0;
	while (i < map->area)
	{
		map->point[i].px = map->point[i].px - shx;
		map->point[i].py = map->point[i].py - shy;
		i++;
	}
}
