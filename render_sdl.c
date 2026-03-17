/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sdl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:00 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/08/26 12:00:00 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void check_n_draw_sdl(t_fdf *fdf, t_map *map, int i);

/**
 * Enhanced rendering method that draws directly to the renderer
 * This uses hardware acceleration for better performance
 */
void render_sdl(t_fdf *fdf)
{
    // Start with a clear renderer
    SDL_SetRenderDrawColor(fdf->renderer, 0, 0, 0, 255);
    SDL_RenderClear(fdf->renderer);

    // Begin geometry batch
    geometry_begin(fdf);

    // Draw the wireframe model (batched)
    draw_sdl(fdf);

    // Flush all geometry at once
    geometry_flush(fdf);

    // Present the rendered frame
    SDL_RenderPresent(fdf->renderer);
}

/**
 * Draw the wireframe model using SDL's renderer functions
 */
void draw_sdl(t_fdf *fdf)
{
    int i;
    t_map *map;
    
    map = &fdf->map;
    if (map->point[0].px >= map->point[map->size_y - 1].px)
    {
        i = map->area - 1;
        while (i > 0)
        {
            check_n_draw_sdl(fdf, map, i);
            i--;
        }
    }
    else
    {
        i = 0;
        while (i < map->area)
        {
            check_n_draw_sdl(fdf, map, i);
            i++;
        }
    }

    // Optional: origin point in green (batched as a 1px line)
    t_point o = map->point[0];
    o.color.r = 0; o.color.g = 255; o.color.b = 0;
    d_line_sdl(fdf, o, o);
}

/**
 * Check and draw lines using SDL renderer
 */
static void check_n_draw_sdl(t_fdf *fdf, t_map *map, int i)
{
    if (map->point[i].ax)
        d_line_sdl(fdf, map->point[i - 1], map->point[i]);
    if (map->point[i].ay)
        d_line_sdl(fdf, map->point[i], map->point[i - map->size_x]);
}

/**
 * Draw a single pixel to the data buffer
 * Implementation kept for compatibility with other code parts
 */
void putpix(t_data *data, int x, int y, int color, int win_w, int win_h)
{
    char *dst;

    if (!(x > 0 && y > 0 && x < win_w && y < win_h))
        return;
    dst = data->addr + (y * data->ln_len + x * (data->bpp / 8));
    *(unsigned int *)dst = color;
}


void geometry_begin(t_fdf *fdf)
{
    fdf->map.vcount = 0;
    fdf->map.icount = 0;
}

void geometry_flush(t_fdf *fdf)
{
    if (fdf->map.icount > 0)
        SDL_RenderGeometry(fdf->renderer, NULL, fdf->map.vbuf, fdf->map.vcount, fdf->map.ibuf, fdf->map.icount);
}
