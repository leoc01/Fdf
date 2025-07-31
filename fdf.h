/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:45:48 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/25 18:46:05 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <libft.h>
#include <sys/time.h>

#define W 1920
#define H 1050
#define P 100

#define ESC 65307
#define NUM_PLUS 65451
#define NUM_MINUS 65453
#define DOWN 65364
#define UP 65362
#define DOWN 65364
#define LEFT 65361
#define RIGHT 65363
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100
#define E_KEY 101
#define Q_KEY 113

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
	float	bx;
	float	by;
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
	float		z_fac;
	int			area;
	t_point		*point;
	t_limits	lim;
}	t_map;

typedef struct s_params
{
	float		zoom;
	float		zoom_min;
	int			cx;
	int			cy;
	float		delta;
	long long	last_frame_time;
	int			zoom_dir;
	float		shx;
	float		shy;
	int			x_dir;
	int			y_dir;
	float		z_angle;
	int			z_fac_dir;
	int			angle_dir;
	int			fps;
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
void		start(t_fdf *fdf, char *file);

//generate_map
void		load_file_data(t_fdf *fdf, char *file);

// color
int			to_rgb(t_color *color);
int			from_rgb(char rgb, int color);
int			step_color(t_step *step, t_color color, int current);
t_step		def_step(t_point *i, t_point *f, float size);
t_color		set_color(char *content);

// matrix
void		set_limits(t_map *map);
void		to_iso(t_map *map, float anglez);
void		scale(t_map *map, float zoom);
void		shift(t_map *map, t_params params);

// render
void		putpix(t_data *data, int x, int y, int color);
void		render(t_fdf *fdf);
void		draw(t_fdf *fdf, t_data *data);
//line
void		d_line(t_data *data, t_point i, t_point f);

// loop
void		update(t_fdf *fdf);
int			loop(t_fdf *fdf);
long long	get_time(void);
void		calculate_delta(t_fdf *fdf);

// hooks
int			key_press(int keysym, t_fdf *fdf);
int			key_release(int keysym, t_fdf *fdf);
int			close_fdf(t_fdf *fdf, char *msg, char *err);
