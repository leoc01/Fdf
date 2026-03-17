/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:00 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/08/26 12:00:00 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Local capacity helpers for dynamic batching buffers
static int lsdl_ensure_capacity_int(int **buf, int *cap, int needed)
{
    if (needed <= *cap)
        return 1;
    int newcap = (*cap == 0) ? 2048 : *cap * 2;
    while (newcap < needed)
        newcap *= 2;
    int *nbuf = (int *)realloc(*buf, sizeof(int) * newcap);
    if (!nbuf)
        return 0;
    *buf = nbuf;
    *cap = newcap;
    return 1;
}

static int lsdl_ensure_capacity_vertex(SDL_Vertex **buf, int *cap, int needed)
{
    if (needed <= *cap)
        return 1;
    int newcap = (*cap == 0) ? 2048 : *cap * 2;
    while (newcap < needed)
        newcap *= 2;
    SDL_Vertex *nbuf = (SDL_Vertex *)realloc(*buf, sizeof(SDL_Vertex) * newcap);
    if (!nbuf)
        return 0;
    *buf = nbuf;
    *cap = newcap;
    return 1;
}

/**
 * Simple line drawing using SDL renderer
 * 
 * This uses SDL's built-in line drawing functions which are
 * hardware accelerated and much faster than pixel-by-pixel rendering
 */
static inline SDL_Color color_from_point(t_color c)
{
    SDL_Color sc;
    sc.r = (Uint8)c.r;
    sc.g = (Uint8)c.g;
    sc.b = (Uint8)c.b;
    sc.a = 255;
    return sc;
}

void d_line_sdl(t_fdf *fdf, t_point i, t_point f)
{
    // Quick reject: fully off-screen
    if ((i.px < 0 && f.px < 0)
        || (i.px > fdf->win_w && f.px > fdf->win_w)
        || (i.py < 0 && f.py < 0)
        || (i.py > fdf->win_h && f.py > fdf->win_h))
        return;

    // If it's a single point, draw a tiny degenerate segment
    if ((int)i.px == (int)f.px && (int)i.py == (int)f.py)
        f.px = i.px + 0.5f;

    // Build a thin quad (two triangles) for the line to enable vertex colors (gradient)
    const float thickness = 1.0f; // adjust if needed
    float dx = f.px - i.px;
    float dy = f.py - i.py;
    float len = sqrtf(dx*dx + dy*dy);
    if (len <= 0.0001f)
        return;
    float nx = -dy / len; // normal vector (perpendicular)
    float ny = dx / len;
    float ox = nx * (thickness * 0.5f);
    float oy = ny * (thickness * 0.5f);

    // Four vertices: i- , i+ , f- , f+
    int vbase = fdf->map.vcount;
    if (!lsdl_ensure_capacity_vertex(&fdf->map.vbuf, &fdf->map.vcap, vbase + 4))
        return;
    if (!lsdl_ensure_capacity_int(&fdf->map.ibuf, &fdf->map.icap, fdf->map.icount + 6))
        return;

    SDL_Color ci = color_from_point(i.color);
    SDL_Color cf = color_from_point(f.color);

    // i_minus
    fdf->map.vbuf[vbase + 0].position.x = i.px - ox;
    fdf->map.vbuf[vbase + 0].position.y = i.py - oy;
    fdf->map.vbuf[vbase + 0].color = ci;
    fdf->map.vbuf[vbase + 0].tex_coord.x = 0.0f;
    fdf->map.vbuf[vbase + 0].tex_coord.y = 0.0f;

    // i_plus
    fdf->map.vbuf[vbase + 1].position.x = i.px + ox;
    fdf->map.vbuf[vbase + 1].position.y = i.py + oy;
    fdf->map.vbuf[vbase + 1].color = ci;
    fdf->map.vbuf[vbase + 1].tex_coord.x = 0.0f;
    fdf->map.vbuf[vbase + 1].tex_coord.y = 1.0f;

    // f_minus
    fdf->map.vbuf[vbase + 2].position.x = f.px - ox;
    fdf->map.vbuf[vbase + 2].position.y = f.py - oy;
    fdf->map.vbuf[vbase + 2].color = cf;
    fdf->map.vbuf[vbase + 2].tex_coord.x = 1.0f;
    fdf->map.vbuf[vbase + 2].tex_coord.y = 0.0f;

    // f_plus
    fdf->map.vbuf[vbase + 3].position.x = f.px + ox;
    fdf->map.vbuf[vbase + 3].position.y = f.py + oy;
    fdf->map.vbuf[vbase + 3].color = cf;
    fdf->map.vbuf[vbase + 3].tex_coord.x = 1.0f;
    fdf->map.vbuf[vbase + 3].tex_coord.y = 1.0f;

    // two triangles: (0,1,2) and (2,1,3)
    fdf->map.ibuf[fdf->map.icount + 0] = vbase + 0;
    fdf->map.ibuf[fdf->map.icount + 1] = vbase + 1;
    fdf->map.ibuf[fdf->map.icount + 2] = vbase + 2;
    fdf->map.ibuf[fdf->map.icount + 3] = vbase + 2;
    fdf->map.ibuf[fdf->map.icount + 4] = vbase + 1;
    fdf->map.ibuf[fdf->map.icount + 5] = vbase + 3;
    fdf->map.vcount += 4;
    fdf->map.icount += 6;
}

// End of file - only using SDL renderer functions
