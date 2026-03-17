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
make -f Makefile.web clean
make -f Makefile.web

if [ -f "fdf.html" ]; then
    echo ""
    echo "Build successful."
    echo "Files to serve: fdf.html, fdf.js, fdf.wasm, fdf.data"
    echo ""
    echo "To test locally:"
    echo "  python3 -m http.server 8000"
    echo ""
else
    echo "Build failed: fdf.html not generated."
    exit 1
fi
