# World Terrain Visualizer

An interactive 3D wireframe terrain renderer running entirely in the browser. Built with C, SDL2, and compiled to WebAssembly via Emscripten.

Originally a [42 school](https://42.fr) FdF (Fil de Fer) project — a wireframe map renderer — extended with SDL2 hardware-accelerated rendering and ported to the web.

## How It Works

```
C source code (SDL2 renderer)
        │
        ▼
  Emscripten (emcc)
        │
        ▼
  WebAssembly (.wasm) + JS glue
        │
        ▼
  Runs in any modern browser
```

The renderer reads `.fdf` height map files and draws them as colored wireframe meshes using isometric projection. Line drawing is done via `SDL_RenderGeometry` with batched vertex/index buffers for performance. Color gradients are interpolated per-line based on height values.

## Controls

| Key | Action |
|-----|--------|
| `W` `A` `S` `D` | Pan the model |
| `Q` / `E` | Adjust height scale |
| `↑` / `↓` | Zoom in / out |
| `←` / `→` | Rotate |
| `P` | Toggle projection mode |

## Maps

| Map | Description |
|-----|-------------|
| 42 | The classic "42" text in 3D |
| Japan | Elevation data of Japan |
| Africa | Elevation data of Africa |
| Julia Fractal | Julia set rendered as terrain |

## Building

### Prerequisites

- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)

### Web build

```sh
./build_web.sh
```

This compiles the C source to WebAssembly and produces four files:

- `fdf.html` — the page (from `working_template.html` shell)
- `fdf.js` — Emscripten JS glue code
- `fdf.wasm` — compiled WebAssembly binary
- `fdf.data` — preloaded map files

### Native build (for development)

Requires SDL2 installed locally.

```sh
make
./fdf maps/42.fdf
```

### Serving

Any static file server works. For local testing:

```sh
python3 -m http.server 8000
```

For production on a VPS, point nginx or caddy at the directory containing the four build output files.

Example nginx config:

```nginx
server {
    listen 80;
    server_name your-domain.com;
    root /path/to/build/output;
    index fdf.html;

    location / {
        try_files $uri $uri/ =404;
    }

    # Required MIME types for WASM
    types {
        application/wasm wasm;
    }

    # Cache static assets
    location ~* \.(wasm|data|js)$ {
        expires 7d;
        add_header Cache-Control "public, immutable";
    }
}
```

## Project Structure

```
├── fdf.h                  # Main header — structs, SDL2 types, prototypes
├── emscripten_main.c      # Web entry point (emscripten_set_main_loop_arg)
├── fdf.c                  # Native desktop entry point (SDL event loop)
├── color.c                # Hex color parsing, RGB conversion
├── generate_map.c         # .fdf file parser, point generation
├── gradient.c             # Per-line color interpolation
├── hooks.c                # Keyboard input handling
├── line_sdl.c             # Line drawing via SDL_RenderGeometry
├── loop.c                 # Frame update, delta time, input processing
├── matrix.c               # Isometric projection, scale, shift, rotation
├── render_sdl.c           # SDL hardware-accelerated render path
├── Makefile               # Native build
├── Makefile.web           # Emscripten/WASM build
├── build_web.sh           # Web build script
├── working_template.html  # HTML shell template for Emscripten
├── libft/                 # 42's standard C library
└── maps/                  # Height map data files (.fdf)
```

## License

42 school project by [lbuscaro](https://github.com/leoc01).
