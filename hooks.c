#include "fdf.h"

int	key_press(int keysym, t_fdf *fdf)
{
	if (keysym == UP)
		fdf->params.zoom_dir += 1;
	if (keysym == DOWN)
		fdf->params.zoom_dir -= 1;
	if (keysym == D_KEY)
		fdf->params.x_dir += 1;
	if (keysym == A_KEY)
		fdf->params.x_dir -= 1;
	if (keysym == S_KEY)
		fdf->params.y_dir += 1;
	if (keysym == W_KEY)
		fdf->params.y_dir -= 1;
	if (keysym == RIGHT)
		fdf->params.angle_dir += 1;
	if (keysym == LEFT)
		fdf->params.angle_dir -= 1;
	return (0);
}

int	key_release(int keysym, t_fdf *fdf)
{
	if (keysym == UP)
		fdf->params.zoom_dir -= 1;
	if (keysym == DOWN)
		fdf->params.zoom_dir += 1;
	if (keysym == ESC)
		close_fdf(fdf, 0);
	if (keysym == D_KEY)
		fdf->params.x_dir -= 1;
	if (keysym == A_KEY)
		fdf->params.x_dir += 1;
	if (keysym == S_KEY)
		fdf->params.y_dir -= 1;
	if (keysym == W_KEY)
		fdf->params.y_dir += 1;
	if (keysym == RIGHT)
		fdf->params.angle_dir -= 1;
	if (keysym == LEFT)
		fdf->params.angle_dir += 1;
	return (0);
}

int	loop(t_fdf *fdf)
{
	update(fdf);
	render(fdf);
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
