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

	if (argc != 2)
	{
		ft_putstr_fd("Invalid number of arguments: Use 1\n", 2);
		return (1);
	}
	fdf = initialize_fdf();
	load_file_data(fdf, argv[1]);
	set_parameters(fdf);
	render(fdf);
	mlx_loop_hook(fdf->mlx, loop, fdf);
	mlx_loop(fdf->mlx);
}

static t_fdf	*initialize_fdf(void)
{
	t_fdf	*fdf;

	fdf = ft_calloc(1, sizeof (t_fdf));
	if (!fdf)
		close_fdf(fdf, NULL, "Fail to init FDF");
	fdf->mlx = NULL;
	fdf->mlx_win = NULL;
	fdf->map.point = NULL;
	fdf->file_content = NULL;
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		close_fdf(fdf, NULL, "Fail to init MLX");
	fdf->mlx_win = mlx_new_window(fdf->mlx, W, H, "Fdf");
	if (!fdf->mlx_win)
		close_fdf(fdf, NULL, "Fail to create window");
	mlx_hook(fdf->mlx_win, 17, (1L << 17), close_fdf, fdf);
	mlx_hook(fdf->mlx_win, 02, (1L << 0), key_press, fdf);
	mlx_hook(fdf->mlx_win, 03, (1L << 1), key_release, fdf);
	return (fdf);
}

static void	set_parameters(t_fdf *fdf)
{
	float		dx_rel;
	float		dy_rel;
	t_map		*map;
	t_params	*params;

	map = &fdf->map;
	params = &fdf->params;
	params->angle_dir = 0;
	to_iso(map, 0);
	set_limits(map);
	dx_rel = W / fabs(map->lim.x_max - map->lim.x_min);
	dy_rel = H / fabs(map->lim.y_max - map->lim.y_min);
	params->zoom = (W - P * 2.0f) / fabs(map->lim.x_max - map->lim.x_min);
	if (dx_rel > dy_rel)
		params->zoom = (H - P * 2.0f) / fabs(map->lim.y_max - map->lim.y_min);
	scale(map, params->zoom);
	set_limits(map);
	params->cx = (map->lim.x_min + (map->lim.x_max - map->lim.x_min) / 2) - W / 2;
	params->cy = (map->lim.y_min + (map->lim.y_max - map->lim.y_min) / 2) - H / 2;
	shift(map, *params);
}
