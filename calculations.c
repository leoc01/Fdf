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

	i = 0;
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
	
	i = 0;
	while (i < map->area)
	{
		map->point[i].px *= zoom;
		map->point[i].py *= zoom;
		i++;
	}
}

void	shift(t_map *map, int shx, int shy)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		map->point[i].px = map->point[i].px - shx;
		map->point[i].py = map->point[i].py - shy;
		i++;
	}
}

void	init_values(t_fdf *fdf)
{
	to_iso(&fdf->map);
	set_limits(&fdf->map);
	fdf->zoom = (WIDTH - PADDING * 2.0f) / fabs(fdf->map.limits.x_max - fdf->map.limits.x_min);
	if (WIDTH / fabs(fdf->map.limits.x_max - fdf->map.limits.x_min) > HEIGHT / fabs(fdf->map.limits.y_max - fdf->map.limits.y_min))
		fdf->zoom = (HEIGHT - PADDING * 2.0f) / fabs(fdf->map.limits.y_max - fdf->map.limits.y_min);
	scale(&fdf->map, fdf->zoom);
	set_limits(&fdf->map);
	fdf->shx = (fdf->map.limits.x_min + (fdf->map.limits.x_max - fdf->map.limits.x_min)/2) - WIDTH/2;
	fdf->shy = (fdf->map.limits.y_min + (fdf->map.limits.y_max - fdf->map.limits.y_min)/2) - HEIGHT/2;
	shift(&fdf->map, fdf->shx, fdf->shy);
}