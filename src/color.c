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

static t_color	hex_to_color(char *n);

t_color	set_color(char *content)
{
	char	str_color[9];
	int		i;
	t_color	color;

	if (*content && (content[0] != '0' || ft_toupper(content[1]) != 'X'))
		return (hex_to_color("0XFFFFFF"));
	i = 0;
	while (i < 9)
	{
		if (*content && content[0] != ' ' && content[0] != '\n')
		{
			str_color[i] = content[0];
			content++;
		}
		else
			str_color[i] = '\0';
		i++;
	}
	color = hex_to_color(str_color);
	return (color);
}

static t_color	hex_to_color(char *n)
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
	color.r = from_rgb('r', color.rgb);
	color.g = from_rgb('g', color.rgb);
	color.b = from_rgb('b', color.rgb);
	return (color);
}

int	to_rgb(t_color *color)
{
	return ((color->r << 16) | (color->g << 8) | (color->b));
}

int	from_rgb(char rgb, int color)
{
	if (rgb == 'r')
		return ((color >> 16) & 0xFF);
	if (rgb == 'g')
		return ((color >> 8) & 0xFF);
	if (rgb == 'b')
		return ((color) & 0xFF);
	return (0xFF);
}
