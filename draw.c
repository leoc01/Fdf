#include "fdf.h"

void	putpix(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (!(x > 0 && y > 0 && x < WIDTH && y < HEIGHT))
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
}

void	swap(t_point *i, t_point *f)
{
	t_point aux;

	aux.px = i->px;
	aux.py = i->py;
	i->px = f->px;
	i->py = f->py;
	f->px = aux.px;
	f->py = aux.py;
}

void	d_line_low(t_data *data, t_line *line)
{
	int	d;
	t_step step;
	int color;
	int	current;

	step = def_step(&line->i, &line->f, line->dx);
	if (line->i.py > line->f.py)
		line->y_dir = -1;
	d = 2 * line->dy - line->dx;
	current = 0;
	while (line->i.px <= line->f.px)
	{
		color = get_color(&step, line->i.color, current);
		putpix(data, line->i.px, line->i.py, color);
		if (d >= 0)
		{
			line->i.py += line->y_dir;
			d += line->dy - line->dx;
		}
		else
			d += line->dy;
		line->i.px++;
		current++;
	}
}

void	d_line_high(t_data *data, t_line *line)
{
	int		d;
	t_step	step;
	int		color;
	int		current;

	step = def_step(&line->i, &line->f, line->dy);
	if (line->i.px > line->f.px)
		line->x_dir = -1;
	d = 2 * line->dx - line->dy;
	current = 0;
	while (line->i.py <= line->f.py)
	{
		color = get_color(&step, line->i.color, current);
		putpix(data, line->i.px, line->i.py, color);
		if (d >= 0)
		{
			line->i.px += line->x_dir;
			d += line->dx - line->dy;
		}
		else
			d += line->dx;
		line->i.py++;
		current++;
	}
}

void	d_line(t_data *data, t_point i, t_point f)
{
	t_line	line;
	
	line.i = i;
	line.f = f;
	line.dx = fabs(line.f.px - line.i.px);
	line.dy = fabs(line.f.py - line.i.py);
	line.x_dir = 1;
	line.y_dir = 1;
	line.init_color = line.i.color;
	if (line.dx >= line.dy)
	{
		if (line.i.px > line.f.px)
		{
			swap(&line.i, &line.f);
		}
		d_line_low(data, &line);
	}
	else
	{
		if (line.i.py > line.f.py)
		{
			swap(&line.i, &line.f);
		}
		d_line_high(data, &line);
	}
}
