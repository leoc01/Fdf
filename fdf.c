/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"	

static t_fdf	*initialize_fdf(void);
static void		set_parameters(t_fdf *fdf);

int	main(int argc, char **argv)
{
	t_fdf	*fdf;
	SDL_Event e;
	int running = 1;
	
	if (argc != 2)
	{
		ft_putstr_fd("Invalid number of arguments: Use 1\n", 2);
		return (1);
	}
	fdf = initialize_fdf();
	load_file_data(fdf, argv[1]);
	set_parameters(fdf);
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				running = 0;
			else if (e.type == SDL_KEYDOWN) {
				// Ignore repeated key presses when a key is held down
				if (e.key.repeat == 0) {
					if (e.key.keysym.sym == SDLK_ESCAPE)
						running = 0;
					else
						key_press(e.key.keysym.sym, fdf);
				}
			}
			else if (e.type == SDL_KEYUP)
				key_release(e.key.keysym.sym, fdf);
		}
		loop(fdf);
	}
	close_fdf(fdf, NULL, NULL);
	return (0);
}

static t_fdf	*initialize_fdf(void)
{
	t_fdf	*fdf;

	fdf = ft_calloc(1, sizeof (t_fdf));
	if (!fdf)
		close_fdf(fdf, NULL, "Fail to init FDF");
	fdf->map.point = NULL;
	fdf->map.projection = 2;
	fdf->file_content = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		close_fdf(fdf, NULL, "Fail to init SDL");
	fdf->window = SDL_CreateWindow(
    	"Fdf",
    	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    	W, H,
    	SDL_WINDOW_SHOWN
	);
	if (!fdf->window)
    	close_fdf(fdf, NULL, "Fail to create SDL window");
	fdf->win_w = W;
	fdf->win_h = H;
	fdf->renderer = SDL_CreateRenderer(
		fdf->window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!fdf->renderer)
		close_fdf(fdf, NULL, "Fail to create SDL renderer");
	
	fdf->texture = SDL_CreateTexture(
		fdf->renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		fdf->win_w, fdf->win_h
	);
	if (!fdf->texture)
		close_fdf(fdf, NULL, "Fail to create SDL texture");

	// Init geometry batching buffers
	fdf->map.vbuf = NULL;
	fdf->map.ibuf = NULL;
	fdf->map.vcap = 0;
	fdf->map.icap = 0;
	fdf->map.vcount = 0;
	fdf->map.icount = 0;
	return (fdf);
}

static void	set_parameters(t_fdf *fdf)
{
	float		dx_rel;
	float		dy_rel;
	t_map		*map;
	t_params	*par;

	map = &fdf->map;
	par = &fdf->params;
	par->fps = 0;
	par->angle_dir = 0;
	map->z_fac = 1;
	par->z_fac_dir = 0;
	to_iso(map, 0);
	set_limits(map);
	dx_rel = fdf->win_w / fabs(map->lim.x_max - map->lim.x_min);
	dy_rel = fdf->win_h / fabs(map->lim.y_max - map->lim.y_min);
	par->zoom = (fdf->win_w - P * 2.0f) / fabs(map->lim.x_max - map->lim.x_min);
	if (dx_rel > dy_rel)
		par->zoom = (fdf->win_h - P * 2.0f) / fabs(map->lim.y_max - map->lim.y_min);
	par->zoom_min = par->zoom / 2;
	scale(map, par->zoom);
	set_limits(map);
	par->cx = (map->lim.x_min + (map->lim.x_max - map->lim.x_min) / 2) - fdf->win_w / 2;
	par->cy = (map->lim.y_min + (map->lim.y_max - map->lim.y_min) / 2) - fdf->win_h / 2;
	shift(map, *par, fdf->win_w, fdf->win_h);
}
