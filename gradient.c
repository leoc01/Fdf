/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_step	def_step(t_point *i, t_point *f, float size)
{
	t_step	step;

	if (i->color.r == f->color.r)
		step.r = 0;
	else
		step.r = (f->color.r - i->color.r) / size;
	if (i->color.g == f->color.g)
		step.g = 0;
	else
		step.g = (f->color.g - i->color.g) / size;
	if (i->color.b == f->color.b)
		step.b = 0;
	else
		step.b = (f->color.b - i->color.b) / size;
	return (step);
}

int	step_color(t_step *step, t_color color, int current)
{
	color.r = (int)(color.r + (current * step->r)) & 0xFF;
	color.g = (int)(color.g + (current * step->g)) & 0xFF;
	color.b = (int)(color.b + (current * step->b)) & 0xFF;
	return (to_rgb(&color));
}
