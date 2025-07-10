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

void	d_line_low(t_data *data, t_point i, t_point f)
{
	int dx;
	int dy;
	int	dir;
	int	d;
	t_step step;
	int color;
	int	current;

	if (i.px > f.px)
		swap(&i, &f);
	dir = 1;
	dx = f.px - i.px;
	dy = f.py - i.py;
	if (dy < 0)
		dir = -1;
	dy *= dir;
	d = 2 * dy - dx;
	current = 0;
	step = def_step(&i, &f, f.px - i.px);
	while (i.px <= f.px)
	{
		color = get_color(&step, i.color, current);
		putpix(data, i.px, i.py, color);
		if (d >= 0)
		{
			i.py += dir;
			d += dy - dx;
		}
		else
			d += dy;
		i.px++;
		current++;
	}
}

void	d_line_high(t_data *data, t_point i, t_point f)
{
	int		dx;
	int		dy;
	int		dir;
	int		d;

	if (i.py > f.py)
		swap(&i, &f);
	dir = 1;
	dx = f.px - i.px;
	dy = f.py - i.py;
	if (dx < 0)
		dir = -1;
	dx *= dir;
	d = 2 * dx - dy;
	while (i.py <= f.py)
	{
		putpix(data, i.px, i.py, i.color.rgb);
		if (d >= 0)
		{
			i.px += dir;
			d += dx - dy;
		}
		else
			d += dx;
		i.py++;
	}
}

void	d_line(t_data *data, t_point i, t_point f)
{
	int	dx;
	int dy;

	dx = fabs(f.px - i.px);
	dy = fabs(f.py - i.py);
	if (dx >= dy)
		d_line_low(data, i, f);
	else
		d_line_high(data, i, f);
}
