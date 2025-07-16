#include "fdf.h"

int	key_press(int keysym, t_fdf *fdf)
{
	if (keysym == ESC)
		close_fdf(fdf, 0);
	return (0);
}

int	close_fdf(t_fdf *fdf, int code)
{
	if (fdf->map.point)
		free(fdf->map.point);
	if (fdf->mlx_win)
		mlx_destroy_window(fdf->mlx, fdf->mlx_win);
	if (fdf->mlx)
	{
		mlx_destroy_display(fdf->mlx);
		free(fdf->mlx);
	}
	exit (code);
}
