/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	rgb_from(char rgb, int color)
{
	if (rgb = 'r')
		return ((color >> 16) & 0xFF);
	if (rgb = 'g')
		return ((color >> 8) & 0xFF);
	if (rgb = 'b')
		return ((color) & 0xFF);
}

static float max(float a, float b)
{
	if (a >= b)
		return (a);
	return (b);
}

t_step	def_step(t_point i, t_point f)
{
	t_step	step;

	step.r = (get_rgb('r', f.color) - get_rgb('r', i.color)) / size;
	step.g = (get_rgb('g', f.color) - get_rgb('g', i.color)) / size;
	step.b = (get_rgb('b', f.color) - get_rgb('b', i.color)) / size;
}

int	get_color(t_point *i, t_point *f, int current_p, int size)
{
	static int	step;
	int			color;

	if (0)
	{
		step = ((f->color) - (i->color))/size;
		printf("step: %d\n", step);
	}
	color = (i->color + current_p * step);
	current_p++;
	if (0)
	{
		printf("color\n");
		step = 0;
		current_p = 0;
	}
	return (0x00FFFFFF);
}
