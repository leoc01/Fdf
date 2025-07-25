/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:54:07 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/25 18:54:08 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	update(t_fdf *fdf)
{
	t_params	*p;
	float	dx;
	float	dy;

	dx = fdf->map.lim.x_max - fdf->map.lim.x_min;
	dy = fdf->map.lim.y_max - fdf->map.lim.y_min;
	p = &fdf->params;
	calculate_delta(fdf);
	if (p->zoom >= p->z || p->zoom_dir > 0)
		p->zoom += p->zoom_dir * p->zoom * p->delta;
	p->cx += p->x_dir * p->delta * 400 / p->zoom;
	p->cy += p->y_dir * p->delta * 400 / p->zoom;
	p->z_angle += p->angle_dir * p->delta * 0.8;
	to_iso(&fdf->map, p->z_angle);
	scale(&fdf->map, p->zoom);
	set_limits(&fdf->map);
	shift(&fdf->map, *p);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	calculate_delta(t_fdf *fdf)
{
	long long		now;
	static float	accumulator;
	static int		fps;

	now = get_time();
	fdf->params.delta = (now - fdf->params.last_frame_time) / 1000.0f;
	fdf->params.last_frame_time = now;
	accumulator += fdf->params.delta;
	fps++;
	if (accumulator >= 1.0f)
	{
		fdf->params.fps = fps;
		fps = 0;
		accumulator = 0;
	}
}
