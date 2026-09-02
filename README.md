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

Runtime arguments can be appended to either executable command, for example:

```sh
wine bin/Win32_Release/GolfXTRM/GolfXTRM.exe -fullscreen
wine bin/Win32_Release/GolfXTRM/GolfXTRM.exe -smallwindow -width 1280 -height 720 -skipintro
```

The supported arguments are:

- `-fullscreen` uses the primary monitor in fullscreen mode. Its current resolution is used unless overridden with `-width` or `-height`.
- `-largewindow` uses a centered window sized slightly smaller than the primary monitor. This is the default mode.
- `-smallwindow` uses a centered 1000×750 window unless overridden with `-width` or `-height`.
- `-width <pixels>` and `-height <pixels>` override the corresponding window dimension.
- `-skipintro` skips the introductory sound and splash-screen delay. Debug builds already enable this behavior at compile time.
- `-novao` disables OpenGL vertex array objects for compatibility troubleshooting.
- `-timespeed <multiplier>` sets the world-clock rate. The default is `120`, meaning two in-game minutes per real second.
- `-golfballspeed <multiplier>` scales physics updates for golf balls and other holdable items. The default is `1`.

If more than one window mode is supplied, the last of `-fullscreen`, `-largewindow`, and `-smallwindow` takes precedence.

To play, use `W`, `A`, `S`, and `D` to move relative to the direction the golfer is facing. Hold Left Shift to run and press Space to jump. Walk within reach of the ball, then hold the left mouse button and sweep the mouse to swing the golfer's arms and club through it.

Mouse movement has four modes depending on which buttons are held:

| Mouse buttons | Input mode | Effect |
| --- | --- | --- |
| None | Head and body | Looks around. The golfer's body turns to follow the view, and the arms return to their resting position. |
| Left only | Arms | Swings or aims the arms while the head and body remain facing the same direction. |
| Right only | Head and arms | Swings or aims the arms while the head follows their direction; the body remains still. |
| Left and right | Head | Looks around while the body remains still and the arms retain their position. |

Press `B` to drop another ball, `O` for filled rendering, `P` for wireframe rendering, and Escape to quit.

On Windows, run each `.exe` directly instead of using `wine`. `CMAKE_BUILD_TYPE` is the only project-specific user-facing build switch. All builds target SSE2-capable x86 hardware. `GLEW_STATIC` is always enabled so the bundled static GLEW library is used; `SKIPINTRO` disables the intro in Debug, `INTRO` enables the intro in Release, and `NDEBUG` disables standard assertions in Release.
