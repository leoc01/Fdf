/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <libft.h>

#define W 1920
#define H 1000
#define P 50

#define ESC 65307

#define Z_FAC 1

#define BUFFER 50000

typedef struct s_color
{
	int	rgb;
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_step
{
	float	r;
	float	g;
	float	b;
}	t_step;

typedef struct s_point
{
	float	ax;
	float	ay;
	float	az;
	float	px;
	float	py;
	t_color	color;
}	t_point;

typedef struct s_line
{
	t_point	i;
	t_point	f;
	float	dx;
	float	dy;
	int		s_dir;
	int		c_dir;
	t_color	init_color;
}	t_line;

typedef struct s_limits
{
	float	x_max;
	float	x_min;
	float	y_max;
	float	y_min;
}	t_limits;

typedef struct s_map
{
	int			size_x;
	int			size_y;
	int			area;
	t_point		*point;
	t_limits	lim;
}	t_map;

typedef struct s_params
{
	float		zoom;
	int			cx;
	int			cy;
}	t_params;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		ln_len;
	int		endian;
}	t_data;

typedef struct s_fdf
{
	void		*mlx;
	void		*mlx_win;
	char		*file_content;
	t_data		data;
	t_map		map;
	t_params	params;
}	t_fdf;

// main
void	start(t_fdf *fdf, char *file);

//generate_map
void	load_file_data(t_fdf *fdf, char *file);

// color
t_color	set_color(char *content);
int		to_rgb(t_color *color);
int		from_rgb(char rgb, int color);

// gradient
t_step	def_step(t_point *i, t_point *f, float size);
int		step_color(t_step *step, t_color color, int current);

// matrix
void	set_limits(t_map *map);
void	to_iso(t_map *map, float anglez);
void	scale(t_map *map, float zoom);
void	shift(t_map *map, t_params params);

// line
void	d_line(t_data *data, t_point i, t_point f);

// render
void	render(t_fdf *fdf);
void	putpix(t_data *data, int x, int y, int color);

// hooks
int		key_press(int keysyn, t_fdf *fdf);
int		close_fdf(t_fdf *fdf, char *msg, char *err);
