/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_line(t_line *line, t_point *i, t_point *f);
static void	d_line_low(t_data *data, t_line *line);
static void	d_line_high(t_data *data, t_line *line);

void	d_line(t_data *data, t_point i, t_point f)
{
	t_line	line;

	init_line(&line, &i, &f);
	if (line.dx >= line.dy)
		d_line_low(data, &line);
	else
		d_line_high(data, &line);
}

static void	init_line(t_line *line, t_point *i, t_point *f)
{
	line->i = *i;
	line->f = *f;
	line->dx = fabs(f->px - i->px);
	line->dy = fabs(f->py - i->py);
	line->s_dir = 1;
	line->c_dir = 1;
	line->init_color = i->color;
	if (line->dx >= line->dy)
	{
		if (i->px > f->px)
		{
			swap(&line->i, &line->f);
			line->init_color = f->color;
			line->c_dir = -1;
		}
	}
	else
	{
		if (i->py > f->py)
		{
			swap(&line->i, &line->f);
			line->init_color = f->color;
			line->c_dir = -1;
		}
	}
}

static void	d_line_low(t_data *data, t_line *line)
{
	int		d;
	t_step	step;
	int		color;
	int		current;

	step = def_step(&line->i, &line->f, line->dx * line->c_dir);
	if (line->i.py > line->f.py)
		line->s_dir = -1;
	d = 2 * line->dy - line->dx;
	current = 0;
	while (line->i.px <= line->f.px)
	{
		color = step_color(&step, line->init_color, current);
		putpix(data, line->i.px, line->i.py, color);
		if (d >= 0)
		{
			line->i.py += line->s_dir;
			d += line->dy - line->dx;
		}
		else
			d += line->dy;
		line->i.px++;
		current++;
	}
}

static void	d_line_high(t_data *data, t_line *line)
{
	int		d;
	t_step	step;
	int		color;
	int		current;

	step = def_step(&line->i, &line->f, line->dy * line->c_dir);
	if (line->i.px > line->f.px)
		line->s_dir = -1;
	d = 2 * line->dx - line->dy;
	current = 0;
	while (line->i.py <= line->f.py)
	{
		color = step_color(&step, line->init_color, current);
		putpix(data, line->i.px, line->i.py, color);
		if (d >= 0)
		{
			line->i.px += line->s_dir;
			d += line->dx - line->dy;
		}
		else
			d += line->dx;
		line->i.py++;
		current++;
	}
}


