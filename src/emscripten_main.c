#include "fdf.h"

// Function declarations
t_fdf *initialize_fdf(void);
void set_parameters(t_fdf *fdf);

static void check_resize(t_fdf *fdf)
{
    int cw, ch;

    cw = 0;
    ch = 0;
    emscripten_get_canvas_element_size("#canvas", &cw, &ch);
    if (cw > 0 && ch > 0 && (cw != fdf->win_w || ch != fdf->win_h))
    {
        fdf->win_w = cw;
        fdf->win_h = ch;
        SDL_SetWindowSize(fdf->window, cw, ch);
        if (fdf->texture)
            SDL_DestroyTexture(fdf->texture);
        fdf->texture = SDL_CreateTexture(
            fdf->renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            cw, ch
        );
    }
}

void main_loop(void *arg)
{
    t_context *context = (t_context *)arg;
    t_fdf *fdf = context->fdf;
    SDL_Event e;

    check_resize(fdf);

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            context->running = 0;
        else if (e.type == SDL_KEYDOWN) {
            // Ignore repeated key presses when a key is held down
            if (e.key.repeat == 0)
                key_press(e.key.keysym.sym, fdf);
        }
        else if (e.type == SDL_KEYUP)
            key_release(e.key.keysym.sym, fdf);
    }
    
    loop(fdf);

    // If not running anymore, quit
    if (!context->running) {
        emscripten_cancel_main_loop();
        close_fdf(fdf, NULL, NULL);
    }
}

int main(int argc, char **argv)
{
    t_fdf *fdf;
    t_context context;
    
    if (argc < 2) {
        // In the web version, default to 42.fdf if no arguments
        char *default_map = "maps/t1.fdf";
        fdf = initialize_fdf();
        printf("Using default map: %s\n", default_map);
        load_file_data(fdf, default_map);
    } else {
        fdf = initialize_fdf();
        printf("Loading map: %s\n", argv[1]); // Debug output
        
        // Ensure the path has the proper format
        char map_path[256];
        strcpy(map_path, argv[1]);
        
        // Check if file exists, if not try different combinations
        FILE *file = fopen(map_path, "r");
        if (!file) {
            printf("Could not open map: %s\n", map_path);
            
            // Default fallback
            strcpy(map_path, "maps/t1.fdf");
            printf("Falling back to default: %s\n", map_path);
        } else {
            fclose(file);
        }
        
        load_file_data(fdf, map_path);
    }
    
    set_parameters(fdf);
    
    context.fdf = fdf;
    context.running = 1;
    
    // Set up the main loop for Emscripten
    emscripten_set_main_loop_arg(main_loop, &context, 0, 1);
    
    return 0;
}

// We need to implement these functions here since we're not using the ones from fdf.c
t_fdf *initialize_fdf(void)
{
    t_fdf    *fdf;

    fdf = ft_calloc(1, sizeof(t_fdf));
    if (!fdf)
        close_fdf(fdf, NULL, "Fail to init FDF");
    fdf->map.point = NULL;
    fdf->map.projection = 2;
    fdf->file_content = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        close_fdf(fdf, NULL, "Fail to init SDL");
    fdf->window = SDL_CreateWindow(
        "Fdf",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        W, H,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!fdf->window)
        close_fdf(fdf, NULL, "Fail to create SDL window");
    fdf->win_w = W;
    fdf->win_h = H;
    fdf->renderer = SDL_CreateRenderer(
        fdf->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!fdf->renderer)
        close_fdf(fdf, NULL, "Fail to create SDL renderer");
    
    fdf->texture = SDL_CreateTexture(
        fdf->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        fdf->win_w, fdf->win_h
    );
    if (!fdf->texture)
        close_fdf(fdf, NULL, "Fail to create SDL texture");
    // Init geometry batching buffers
    fdf->map.vbuf = NULL;
    fdf->map.ibuf = NULL;
    fdf->map.vcap = 0;
    fdf->map.icap = 0;
    fdf->map.vcount = 0;
    fdf->map.icount = 0;
    return (fdf);
}

void set_parameters(t_fdf *fdf)
{
    float        dx_rel;
    float        dy_rel;
    t_map        *map;
    t_params    *par;

    map = &fdf->map;
    par = &fdf->params;
    par->fps = 0;
    par->angle_dir = 0;
    map->z_fac = 1;
    par->z_fac_dir = 0;
    to_iso(map, 0);
    set_limits(map);
    dx_rel = fdf->win_w / fabs(map->lim.x_max - map->lim.x_min);
    dy_rel = fdf->win_h / fabs(map->lim.y_max - map->lim.y_min);
    par->zoom = (fdf->win_w - P * 2.0f) / fabs(map->lim.x_max - map->lim.x_min);
    if (dx_rel > dy_rel)
        par->zoom = (fdf->win_h - P * 2.0f) / fabs(map->lim.y_max - map->lim.y_min);
    par->zoom_min = par->zoom / 2;
    scale(map, par->zoom);
    set_limits(map);
    par->cx = (map->lim.x_min + (map->lim.x_max - map->lim.x_min) / 2) - fdf->win_w / 2;
    par->cy = (map->lim.y_min + (map->lim.y_max - map->lim.y_min) / 2) - fdf->win_h / 2;
    shift(map, *par, fdf->win_w, fdf->win_h);
}
