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

static void	initialize_fdf(t_fdf *fdf);
static void	set_parameters(t_fdf *fdf);

int	main(int argc, char **argv)
{
	t_fdf	fdf;

	if (argc != 2)
	{
		ft_putstr_fd("Invalid number of arguments: Use 1\n", 2);
		return (1);
	}
	initialize_fdf(&fdf);
	load_file_data(&fdf, argv[1]);
	set_parameters(&fdf);
	render(&fdf);
	mlx_loop(fdf.mlx);
}

static void	initialize_fdf(t_fdf *fdf)
{
	fdf->mlx = NULL;
	fdf->mlx_win = NULL;
	fdf->map.point = NULL;
	fdf->file_content = NULL;
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, "Fail to init MLX");
	fdf->mlx_win = mlx_new_window(fdf->mlx, W, H, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, "Fail to create window");
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	fdf->map.size_x = 0;
	fdf->map.size_y = 0;
}

static void	set_parameters(t_fdf *fdf)
{
	float		dx_rel;
	float		dy_rel;
	t_map		*map;
	t_params	*params;

	map = &fdf->map;
	params = &fdf->params;
	to_iso(map, (-M_PI / 2));
	set_limits(map);
	dx_rel = W / fabs(map->lim.x_max - map->lim.x_min);
	dy_rel = H / fabs(map->lim.y_max - map->lim.y_min);
	params->zoom = (W - P * 2.0f) / fabs(map->lim.x_max - map->lim.x_min);
	if (dx_rel > dy_rel)
		params->zoom = (H - P * 2.0f) / fabs(map->lim.y_max - map->lim.y_min);
	scale(map, params->zoom);
	set_limits(map);
	shift(map, *params);
}
