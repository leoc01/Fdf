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
/*
static int	get_r(int color)
{
	return ((color >> 16) & 0xFF);
}

static int get_g(int color)
{
	return ((color >> 8) & 0xFF);
}

static int get_b(int color)
{
	return ((color) & 0xFF);
}
*/
static float max(float a, float b)
{
	if (a >= b)
		return (a);
	return (b);
}

int	get_color(t_point *i, t_point *f)
{
	static int	step;
	static int	current_p;
	float		size;
	int			color;

	if (!current_p)
	{
		size = max(fabs(f->px - i->px), fabs(f->py - i->py));
		step = ((f->color) - (i->color))/size;
		printf("step: %d\n", step);
	}
	color = (i->color + current_p * step);
	current_p++;
	if (i->px == f->px)
	{
		printf("color\n");
		step = 0;
		current_p = 0;
	}
	return (color);
}
