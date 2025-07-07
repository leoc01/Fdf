#include "fdf.h"

void to_iso(t_map *map, float z_angle)
{
   int i;
    float cos_a = cos(z_angle);
    float sin_a = sin(z_angle);
    float rotated_x;
	float rotated_y;

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

void	shift(t_map *map, t_params params)
{
	int	i;

	params.cx = (map->limits.x_min + (map->limits.x_max - map->limits.x_min)/2) - WIDTH/2;
	params.cy = (map->limits.y_min + (map->limits.y_max - map->limits.y_min)/2) - HEIGHT/2;
	i = 0;
	while (i < map->area)
	{
		map->point[i].px = map->point[i].px - params.cx + params.shx * params.zoom;
		map->point[i].py = map->point[i].py - params.cy + params.shy * params.zoom;
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
	long long	now;

	now = get_time();
	fdf->params.delta = (now - fdf->params.last_frame_time) / 1000.0f;
	fdf->params.last_frame_time = now;
}

void	init_values(t_fdf *fdf)
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
