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

int	rgb_from(char rgb, int color)
{
	if (rgb == 'r')
		return ((color >> 16) & 0xFF);
	if (rgb == 'g')
		return ((color >> 8) & 0xFF);
	if (rgb == 'b')
		return ((color) & 0xFF);
	return (0xFF);
}

int	to_rgb(t_color *color)
{
	return ((color->r << 16) | (color->g << 8) | (color->b));
}

t_step	def_step(t_point *i, t_point *f, int size)
{
	t_step	step;

	step.r = (f->color.r - i->color.r) / size;
	step.g = (f->color.g - i->color.g) / size;
	step.b = (f->color.b - i->color.b) / size;
	return (step);
}

int	 get_color(t_step *step, t_color color, int current)
{	
	color.r += current * step->r;
	color.g += current * step->g;
	color.b += current * step->b;
	return (to_rgb(&color));
}
