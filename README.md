# Building and running

The bundled libraries target 32-bit Windows, so use a 32-bit MinGW compiler. Separate build directories make switching between Debug and Release explicit.

For a Debug build (`-Og -g`, with `SKIPINTRO` enabled):

```sh
cmake -S . -B build/debug \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --parallel
wine 'bin/Win32_Debug/GolfXTRM debug.exe'
```

For a Release build (`-Ofast`, LTO, profiling flags, stripping, `INTRO`, and `NDEBUG`):

```sh
cmake -S . -B build/release \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/release --parallel
wine bin/Win32_Release/GolfXTRM/GolfXTRM.exe
```

On Windows, run each `.exe` directly instead of using `wine`. `CMAKE_BUILD_TYPE` is the only project-specific user-facing build switch. All builds target SSE2-capable x86 hardware. `GLEW_STATIC` is always enabled so the bundled static GLEW library is used; `SKIPINTRO` disables the intro in Debug, `INTRO` enables the intro in Release, and `NDEBUG` disables standard assertions in Release.
