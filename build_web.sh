#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Source Emscripten SDK
if [ -f "$HOME/emsdk/emsdk_env.sh" ]; then
    source "$HOME/emsdk/emsdk_env.sh"
elif [ -n "$EMSDK" ]; then
    source "$EMSDK/emsdk_env.sh"
else
    echo "Error: Emscripten SDK not found. Install emsdk or set EMSDK env var."
    exit 1
fi

# Build
make clean
make WEB=1

if [ -f "build/fdf.html" ]; then
    echo ""
    echo "Build successful."
    echo "Files to serve: build/fdf.html, build/fdf.js, build/fdf.wasm, build/fdf.data"
    echo ""
    echo "To test locally:"
    echo "  python3 -m http.server 8000"
    echo ""
else
    echo "Build failed: fdf.html not generated."
    exit 1
fi
