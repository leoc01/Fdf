/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/28 12:07:54 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	check_n_draw(t_map *map, int i, t_data *data);

void	render(t_fdf *fdf)
{
	t_data	*dt;
	char	*fps;

	dt = &fdf->data;
	dt->img = mlx_new_image(fdf->mlx, W, H);
	dt->addr = mlx_get_data_addr(dt->img, &dt->bpp, &dt->ln_len, &dt->endian);
	draw(fdf, dt);
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, dt->img, 0, 0);
	fps = ft_itoa(fdf->params.fps);
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 20, 0xFFFFFF, "FPS: ");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 50, 20, 0xFFFFFF, fps);
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 40, 0xFFFFFF, "Move:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 55, 0xFFFFFF, "W A S D");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 75, 0xFFFFFF, "Rotate:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 90, 0xFFFFFF, "LEFT RIGHT");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 110, 0xFFFFFF, "Zoom:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 125, 0xFFFFFF, "UP DOWN");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 145, 0xFFFFFF, "Height:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 160, 0xFFFFFF, "Q E");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 180, 0xFFFFFF, "Projection:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 195, 0xFFFFFF, "P");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 215, 0xFFFFFF, "Close:");
	mlx_string_put(fdf->mlx, fdf->mlx_win, 20, 230, 0xFFFFFF, "ESC");
	free(fps);
	mlx_destroy_image(fdf->mlx, dt->img);
}

void	putpix(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (!(x > 0 && y > 0 && x < W && y < H))
		return ;
	dst = data->addr + (y * data->ln_len + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw(t_fdf *fdf, t_data *data)
{
	int		i;
	t_map	*map;

	map = &fdf->map;
	if (map->point[0].px >= map->point[map->size_y - 1].px)
	{
		i = map->area;
		while (i > 0)
		{
			check_n_draw(map, i, data);
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < map->area)
		{
			check_n_draw(map, i, data);
			i++;
		}
	}
	putpix(data, map->point[0].px, map->point[0].py, 0x00FF00);
}

static void	check_n_draw(t_map *map, int i, t_data *data)
{
	if (map->point[i].ax)
		d_line(data, map->point[i - 1], map->point[i]);
	if (map->point[i].ay)
		d_line(data, map->point[i], map->point[i - map->size_x]);
}
