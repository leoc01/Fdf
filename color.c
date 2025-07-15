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

t_color	hex_to_color(char *n)
{
	char	b[17];
	int		len;
	t_color	color;
	int		i;

	ft_strlcpy(b, "0123456789ABCDEF", 17);
	i = 2;
	len = ft_strlen(&n[i]) - 1;
	color.rgb = 0;
	if (n[0] != '0' || ft_toupper(n[1]) != 'X')
		color.rgb = 0X00FFFFFF;
	while (n[i])
	{
		color.rgb += (ft_strchr(b, ft_toupper(n[i])) - &b[0]) * pow(16, len);
		len--;
		i++;
	}
	if (len == 2)
		color.rgb = (color.rgb >> 16) & 0XFF;
	if (len == 4)
		color.rgb = (color.rgb >> 8) & 0XFFFF;
	color.r = rgb_from('r', color.rgb);
	color.g = rgb_from('g', color.rgb);
	color.b = rgb_from('b', color.rgb);
	return (color);
}

int	to_rgb(t_color *color)
{
	return ((color->r << 16) | (color->g << 8) | (color->b));
}

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

int	get_color(t_step *step, t_color color, int current)
{
	color.r = (int)(color.r + (current * step->r)) & 0xFF;
	color.g = (int)(color.g + (current * step->g)) & 0xFF;
	color.b = (int)(color.b + (current * step->b)) & 0xFF;
	return (to_rgb(&color));
}
