#include <mlx.h>
#include <mlx_int.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "./libft/libft.h"

#define WIDTH 1300
#define HEIGHT 690
#define PADDING 50
#define	Z_FAC 2

typedef struct	s_point {
	int ax;
	int ay;
	int az;
	float	px;
	float	py;
	int	color;
}	t_point;

typedef struct	s_limits {
	float	x_max;
	float	x_min;
	float	y_max;
	float	y_min;
}	t_limits;

typedef	struct	s_map {
	int			x;
	int			y;
	int			area;
	t_point		*point;
	t_limits	limits;
}	t_map;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef	struct	s_fdf {
	void	*mlx;
	void	*mlx_win;
	t_data	data;
	t_map	map;
	float		zoom;
}	t_fdf;

void	putpix(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (!(x > 0 && y > 0 && x < WIDTH && y < HEIGHT))
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
}

void	swap(t_point *i, t_point *f)
{
	t_point aux;

	aux.px = i->px;
	aux.py = i->py;
	i->px = f->px;
	i->py = f->py;
	f->px = aux.px;
	f->py = aux.py;
}

void	d_line_low(t_data *data, t_point i, t_point f, int color)
{
	int dx;
	int dy;
	int	dir;
	int	d;

	if (i.px > f.px)
		swap(&i, &f);
	dir = 1;
	dx = f.px - i.px;
	dy = f.py - i.py;
	if (dy < 0)
		dir = -1;
	dy *= dir;
	d = 2 * dy - dx;
	while (i.px <= f.px)
	{
		putpix(data, i.px++, i.py, color);
		if (d >= 0)
		{
			i.py += dir;
			d += dy - dx;
		}
		else
			d += dy;
	}
}

void	d_line_high(t_data *data, t_point i, t_point f, int color)
{
	int dx;
	int dy;
	int	dir;
	int	d;

	if (i.py > f.py)
		swap(&i, &f);
	dir = 1;
	dx = f.px - i.px;
	dy = f.py - i.py;
	if (dx < 0)
		dir = -1;
	dx *= dir;
	d = 2 * dx - dy;
	while (i.py <= f.py)
	{
		putpix(data, i.px, i.py++, color);
		if (d >= 0)
		{
			i.px += dir;
			d += dx - dy;
		}
		else
			d += dx;
	}
}

void	d_line(t_data *data, t_point i, t_point f, int color)
{
	int	dx;
	int dy;

	dx = fabs(f.px - i.px);
	dy = fabs(f.py - i.py);
	if (dx > dy)
		d_line_low(data, i, f, color);
	else
		d_line_high(data, i, f, color);
}

void	get_points(t_point *point, t_map *map, char *file)
{
	char	*line;
	char 	**row;
	int		i;
	int		j;
	int		fd;
	
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	j = 0;
	while (line)
	{
		row = ft_split(line, ' ');
		free(line);
		i = 0;
		while (i < map->x)
		{
			point[i + (j * map->x)].ax = i;
			point[i + (j * map->x)].ay = j;
			point[i + (j * map->x)].az = ft_atoi(row[i]);
			i++;
		}
		j++;
		line = get_next_line(fd);
	}
	close(fd);
}

t_map	create_map(char *file)
{
	t_map	map;
	char	*line;
	char 	**row;
	int		fd;
	
	fd = open(file, O_RDONLY);
	map.x = 0;
	map.y = 0;
	line = get_next_line(fd);
	row = ft_split(line, ' ');
	while (row[map.x])
		map.x++;
	while (line)
	{
		line = get_next_line(fd);
		free(line);
		map.y++;
	}
	close(fd);
	map.area = map.x * map.y;
	map.point = ft_calloc(map.x * map.y, sizeof(t_point));
	get_points(map.point, &map, file);
	return (map);
}

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

void	draw(t_map *map, t_data *data)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		if (map->point[i].ax)
			d_line(data, map->point[i-1], map->point[i], 0x00FF00FF);
		if (map->point[i].ay)
			d_line(data, map->point[i], map->point[i-map->x], 0x00FF00FF);
		putpix(data, map->point[i].px, map->point[i].py, 0x00FFFFFF);
		i++;
	}
}

void	update(t_fdf *fdf)
{
	to_iso(&fdf->map);
	scale(&fdf->map, fdf->zoom);
	shift(&fdf->map);
}

void	render(t_fdf *fdf)
{
	fdf->data.img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	fdf->data.addr = mlx_get_data_addr(fdf->data.img, &fdf->data.bits_per_pixel, &fdf->data.line_length, &fdf->data.endian);
	draw(&fdf->map, &fdf->data); 
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, fdf->data.img, 0, 0);
	mlx_destroy_image(fdf->mlx, fdf->data.img);
}
int	loop(t_fdf *fdf)
{
	update(fdf);
	render(fdf);
	return (1);
}

int	key_hook(int keysym, t_fdf *fdf)
{
	
	if (keysym == 65362)
		fdf->zoom *= 1.1;
	if (keysym == 65364)
		fdf->zoom /= 1.1;
	printf("%d\n", keysym);
	return (0);
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;
	

	if (argc != 2)
		return 1;
	fdf.mlx = mlx_init();
	fdf.mlx_win = mlx_new_window(fdf.mlx, WIDTH, HEIGHT, "Fdf");
	fdf.map = create_map(argv[1]);
	fdf.zoom = 1;
	mlx_key_hook(fdf.mlx_win, key_hook, &fdf);
	mlx_loop_hook(fdf.mlx, loop, &fdf);
	mlx_loop(fdf.mlx);
	ft_putstr_fd("tchau",1);
}
