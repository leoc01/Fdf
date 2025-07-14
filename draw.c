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

void	d_line_low(t_data *data, t_point i, t_point f, float dx, float dy)
{
	int	x_dir;
	int		y_dir;
	int	d;
	t_step step;
	int color;
	int	current;

	x_dir = 1;
	if (i.px > f.px)
	{
		swap(&i, &f);
		dy = -dy;
		dx = -dx;
		x_dir = -1;
	}
	step = def_step(&i, &f, dx);
	y_dir = 1;
	if (dy < 0)
		y_dir = -1;
	dy *= y_dir;
	d = 2 * dy - dx;
	current = f.px - i.px;
	while (i.px <= f.px)
	{
		color = get_color(&step, i.color, current);
		putpix(data, i.px, i.py, color);
		if (d >= 0)
		{
			i.py += y_dir;
			d += dy - dx;
		}
		else
			d += dy;
		i.px++;
		current += x_dir;
	}
}

void	d_line_high(t_data *data, t_point i, t_point f, float dx, float dy)
{
	int		x_dir;
	int		y_dir;
	int		d;
	t_step step;
	int color;
	int	current;

	y_dir = 1;
	if (i.py > f.py)
	{
		swap(&i, &f);
		dy = -dy;
		dx = -dx;
		y_dir = -1;
	}
	step = def_step(&i, &f, dy);
	x_dir = 1;
	if (dx < 0)
		x_dir = -1;
	dx *= x_dir;
	d = 2 * dx - dy;
	current = f.py - i.py;
	while (i.py <= f.py)
	{
		color = get_color(&step, i.color, current);
		putpix(data, i.px, i.py, color);
		if (d >= 0)
		{
			i.px += x_dir;
			d += dx - dy;
		}
		else
			d += dx;
		i.py++;
		current += y_dir;
	}
}

void	d_line(t_data *data, t_point i, t_point f)
{
	float	dx;
	float	dy;

	dx = f.px - i.px;
	dy = f.py - i.py;
	if (fabs(dx) >= fabs(dy))
		d_line_low(data, i, f, dx, dy);
	else
		d_line_high(data, i, f, dx, dy);
}
