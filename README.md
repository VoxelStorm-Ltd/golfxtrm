# GolfXTRM

First person golf game with no HUDs, no aiming aids, no course map, no teleport between shots, and nothing to help you find the ball.

“Golf games seem to be about watching power meters and adjusting targeting sights. I prefer being outdoors, exploring procedural landscapes, and playing with physics simulations. So here’s my take on the genre.” -SlowRiot

This game was written in one week for a game jam.
Features
- Procedurally generated terrain with procedural woodland.
- Realistic passage of time, with day-night and summer-winter cycle.
- Trees grow realistically, in real-time. Of course, that’s very slow - you’d have to leave the computer running for a very long time to see it.
- Time is sped up 120x, so 1 second = 2 minutes in-game. A day lasts about 12 minutes, but you’d have to leave the game running for about 3 days to see the winter snow. The ball is of course very hard to find in the snow.

The latest build can be downloaded from this repository's automated [https://github.com/VoxelStorm-Ltd/golfxtrm/releases](releases).

The original edition is available at https://golfxtrm.com or https://voxelstorm.itch.io/golfxtrm

## Controls

- WASD to walk,
- Shift to run,
- Space to jump (why would you need to jump on a golf course? Perhaps to leap over a short shrub...)
- Hold left mouse button to move arms,
- Hold right mouse button to move arms and head,
- Hold both mouse buttons at the same time to move just your head.
- Press `B` to drop another ball (when you inevitably lose your first one).
-`Escape` to quit.
- For debugging, you can switch between filled and wireframe rendering with `O` and `P`.

## Commandline switches

The supported arguments are:

- `-fullscreen` uses the primary monitor in fullscreen mode. Its current resolution is used unless overridden with `-width` or `-height`.
- `-largewindow` uses a centered window sized slightly smaller than the primary monitor. This is the default mode.
- `-smallwindow` uses a centered 1000×750 window unless overridden with `-width` or `-height`.
- `-width <pixels>` and `-height <pixels>` override the corresponding window dimension.
- `-skipintro` skips the introductory sound and splash-screen delay. Debug builds already enable this behavior at compile time.
- `-novao` disables OpenGL vertex array objects for compatibility troubleshooting.
- `-timespeed <multiplier>` sets the world-clock rate. The default is `120`, meaning two in-game minutes per real second.  Try increasing this if you ever want to see the trees grow in your own lifetime.
- `-golfballspeed <multiplier>` scales physics updates for golf balls and other holdable items. The default is `1`.

If more than one window mode is supplied, the last of `-fullscreen`, `-largewindow`, and `-smallwindow` takes precedence.


## Building from source

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

On Windows, run each `.exe` directly instead of using `wine`. `CMAKE_BUILD_TYPE` is the only project-specific user-facing build switch. All builds target SSE2-capable x86 hardware. `GLEW_STATIC` is always enabled so the bundled static GLEW library is used; `SKIPINTRO` disables the intro in Debug, `INTRO` enables the intro in Release, and `NDEBUG` disables standard assertions in Release.
