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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <libft.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define W 1366
#define H 700
#define P 100

#define ESC SDLK_ESCAPE
#define NUM_PLUS SDLK_KP_PLUS
#define NUM_MINUS SDLK_KP_MINUS
#define DOWN SDLK_DOWN
#define UP SDLK_UP
#define LEFT SDLK_LEFT
#define RIGHT SDLK_RIGHT
#define W_KEY SDLK_w
#define A_KEY SDLK_a
#define S_KEY SDLK_s
#define D_KEY SDLK_d
#define E_KEY SDLK_e
#define Q_KEY SDLK_q
#define P_KEY SDLK_p

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
	int			projection;
	t_point		*point;
	t_limits	lim;

	// Geometry batching for fast SDL rendering with gradients
	SDL_Vertex		*vbuf;     // dynamic vertex buffer
	int				vcount;    // number of used vertices
	int				vcap;      // capacity of vertex buffer
	int				*ibuf;     // dynamic index buffer
	int				icount;    // number of used indices
	int				icap;      // capacity of index buffer
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
	char	*addr;     // raw pixel buffer (from SDL_LockTexture)
	int		bpp;      // bits per pixel (always 32 for ARGB8888)
	int		ln_len;   // pitch (bytes per row)
	int		endian;   // not really needed, SDL gives ARGB8888
}	t_data;

typedef struct s_fdf
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;     // NEW: our framebuffer
	char			*file_content;
	int				win_w;
	int				win_h;
	t_data			data;
	t_map			map;
	t_params		params;
}	t_fdf;

#ifdef __EMSCRIPTEN__
typedef struct s_context
{
	t_fdf	*fdf;
	int		running;
}	t_context;
#endif

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
void		shift(t_map *map, t_params params, int win_w, int win_h);
void		change_projection(int *projection);

// render
void		putpix(t_data *data, int x, int y, int color, int win_w, int win_h);

// enhanced rendering with SDL
void		render_sdl(t_fdf *fdf);
void		draw_sdl(t_fdf *fdf);
void		geometry_begin(t_fdf *fdf);
void		geometry_flush(t_fdf *fdf);

//line
void		d_line_sdl(t_fdf *fdf, t_point i, t_point f);

// loop
void		update(t_fdf *fdf);
int			loop(t_fdf *fdf);
long long	get_time(void);
void		calculate_delta(t_fdf *fdf);

// hooks
int			key_press(int keysym, t_fdf *fdf);
int			key_release(int keysym, t_fdf *fdf);
int			close_fdf(t_fdf *fdf, char *msg, char *err);

// main
#ifndef __EMSCRIPTEN__
// Only declare these if not compiling with emscripten
t_fdf	*initialize_fdf(void);
void	set_parameters(t_fdf *fdf);
#endif
