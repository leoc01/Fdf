/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:47:04 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/25 18:47:06 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (keysym == E_KEY)
		fdf->params.z_fac_dir -= 1;
	if (keysym == Q_KEY)
		fdf->params.z_fac_dir += 1;
	if (keysym == P_KEY)
		change_projection(&fdf->map.projection);
	return (0);
}

int	key_release(int keysym, t_fdf *fdf)
{
	if (keysym == UP)
		fdf->params.zoom_dir -= 1;
	if (keysym == DOWN)
		fdf->params.zoom_dir += 1;
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
	if (keysym == E_KEY)
		fdf->params.z_fac_dir += 1;
	if (keysym == Q_KEY)
		fdf->params.z_fac_dir -= 1;
	return (0);
}

int	loop(t_fdf *fdf)
{
	update(fdf);
	render_sdl(fdf);
	return (0);
}

int	close_fdf(t_fdf *fdf, char *msg, char *err)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	if (err)
		perror(err);
	if (fdf->map.point)
		free(fdf->map.point);

	if (fdf->texture)
		SDL_DestroyTexture(fdf->texture);
	if (fdf->renderer)
		SDL_DestroyRenderer(fdf->renderer);
	if (fdf->window)
		SDL_DestroyWindow(fdf->window);

	// Free batching buffers
	if (fdf->map.vbuf)
		free(fdf->map.vbuf);
	if (fdf->map.ibuf)
		free(fdf->map.ibuf);

	SDL_Quit();

	if (fdf)
		free(fdf);
	if (msg)
		exit(1);
	exit(0);
}
