/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	render(t_fdf *fdf)
{
	t_data	*dt;

	dt = &fdf->data;
	dt->img = mlx_new_image(fdf->mlx, W, H);
	dt->addr = mlx_get_data_addr(dt->img, &dt->bpp, &dt->ln_len, &dt->endian);
	draw(&fdf->map, dt);
	mlx_put_image_to_window(fdf->mlx, fdf->mlx_win, dt->img, 0, 0);
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

void	draw(t_map *map, t_data *data)
{
	int	i;

	i = 0;
	while (i < map->area)
	{
		if (map->point[i].ax)
			d_line(data, map->point[i - 1], map->point[i]);
		if (map->point[i].ay)
			d_line(data, map->point[i], map->point[i - map->size_x]);
		i++;
	}
}
