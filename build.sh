#!/usr/bin/env bash
set -e

mkdir -p build_web
cd build_web

emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu || echo 4)

mkdir -p ../dist
cp flightsim.js flightsim.wasm ../dist/
cp ../web/index.html ../web/loader.js ../dist/

echo "Build complete. Output ready in ./dist/"
