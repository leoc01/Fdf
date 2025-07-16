#include "fdf.h"

void	putpix(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (!(x > 0 && y > 0 && x < WIDTH && y < HEIGHT))
		return ;
	dst = data->addr + (y * data->ln_len + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	swap(t_point *i, t_point *f)
{
	t_point	aux;

	aux.px = i->px;
	aux.py = i->py;
	i->px = f->px;
	i->py = f->py;
	f->px = aux.px;
	f->py = aux.py;
}
