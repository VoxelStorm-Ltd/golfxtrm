#!/bin/bash

if [ ! -f "build/client.html" ]; then
  echo "Project not yet built, building..."
  ./build.sh
fi

port=6937

echo "Launching with emrun..."
#emrun --port "$port" build/client.html --browser /home/slowriot/usr/firefox-nightly/firefox-bin --browser_args="--new-window"
emrun --port "$port" build/client.html --browser /usr/lib/chromium/chromium --browser_args='--new-window --js-flags="--experimental-wasm-simd"'
echo "Finished."
