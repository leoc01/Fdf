#include <mlx.h>
#include <mlx_int.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "./libft/libft.h"
#include <stdio.h>

#define WIDTH 1300
#define HEIGHT 690
#define PADDING 50
#define	Z_FAC 2

typedef struct	s_point {
	int 	ax;
	int 	ay;
	int 	az;
	float	px;
	float	py;
	int		color;
}	t_point;

typedef struct	s_limits {
	float	x_max;
	float	x_min;
	float	y_max;
	float	y_min;
}	t_limits;

typedef	struct	s_map {
	int			size_x;
	int			size_y;
	int			area;
	t_point		*point;
	t_limits	limits;
}	t_map;

typedef struct	s_params {
	float	zoom;
	int		cx;
	int		cy;
	int		shx;
	int		shy;
	float	angle;
}	t_params;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef	struct	s_fdf {
	void		*mlx;
	void		*mlx_win;
	t_data		data;
	t_map		map;
	t_params	params;
}	t_fdf;

void	putpix(t_data *data, int x, int y, int color);
void	swap(t_point *i, t_point *f);
void	d_line_low(t_data *data, t_point i, t_point f, int color);
void	d_line_high(t_data *data, t_point i, t_point f, int color);
void	d_line(t_data *data, t_point i, t_point f, int color);
void	get_points(t_point *point, t_map *map, char *file);
t_map	create_map(char *file);
void	to_iso(t_map *map, float angle);
void	set_limits(t_map *map);
void	scale(t_map *map, float zoom);
void	shift(t_map *map, t_params params);
void	draw(t_map *map, t_data *data);
void	update(t_fdf *fdf);
void	render(t_fdf *fdf);
int		loop(t_fdf *fdf);
int		key_hook(int keysym, t_fdf *fdf);
void	start(t_fdf *fdf, char *file);
void	init_values(t_fdf *fdf);