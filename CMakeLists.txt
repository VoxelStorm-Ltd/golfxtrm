cmake_minimum_required(VERSION 3.10)

project(client)

include_directories(BEFORE SYSTEM include)
include_directories(BEFORE .)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BOOST_ROOT "/usr/src/boost_1_76_0")

include_directories(AFTER SYSTEM ${BOOST_ROOT})

add_compile_definitions(
  GLFW_INCLUDE_ES3
  NO_BLOB_LOADER
  LOGSTORM_SINGLE_THREADED
  # enable when support improves (https://emscripten.org/docs/porting/exceptions.html):
  #NO_DISABLE_EXCEPTION_CATCHING
)

add_executable(client
  # project-specific:
  ashtree.cpp
  firtree.cpp
  golfball.cpp
  golfclub.cpp
  golfcourse.cpp
  golfer.cpp
  golfer_update.cpp
  holdable.cpp
  landscape_feature.cpp
  main.cpp
  oaktree.cpp
  objloader.cpp
  particle.cpp
  perlin.cpp
  terrain.cpp
  universe.cpp
  vmath.cpp
  worldcomponents_render.cpp
  worldcomponents_update.cpp
  world.cpp
  # shared libraries:
  vectorstorm/vectorstorm.cpp
)

target_compile_options(client PRIVATE
  # TODO: extra options for release build: https://emscripten.org/docs/tools_reference/emcc.html
  #--closure 1
  -msse
  -msse2
  -msse3
  -mssse3
  -msse4.1
  -msse4.2
  -mavx
  -msimd128
  # emscripten ports
  -sUSE_LIBPNG=1
  # enable when support improves (https://emscripten.org/docs/porting/exceptions.html):
  #-fwasm-exceptions
  # errors
  -Wfatal-errors
  # warnings
  -Wall
  -Warray-bounds
  -Wcast-align
  -Wconversion
  -Wdisabled-optimization
  -Wdouble-promotion
  -Wextra
  -Wformat
  -Winit-self
  -Winvalid-pch
  -Wlong-long
  -Wmissing-braces
  -Wmissing-declarations
  -Wmissing-include-dirs
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wpacked
  -Wpointer-arith
  -Wrange-loop-analysis
  -Wredundant-decls
  -Wredundant-move
  -Wshadow
  -Wsuggest-override
  -Wthread-safety-analysis
  -Wuninitialized
  -Wunused
)
# suppress warnings for external libraries built as part of include
file(GLOB_RECURSE include_files include/*)
set_source_files_properties(${include_files} PROPERTIES COMPILE_FLAGS "-w")


target_link_options(client PRIVATE
  -lwebsocket.js
  --bind
  -sALLOW_MEMORY_GROWTH=1
  -sUSE_GLFW=3
  -sMIN_WEBGL_VERSION=2
  -sMAX_WEBGL_VERSION=2
  -sMIN_WEBGL_VERSION=2
  -sFETCH=1
  -sUSE_LIBPNG=1
  # emulate legacy GL fixed pipeline:
  -sLEGACY_GL_EMULATION
  # turn off unsafe optimisations
  -s GL_UNSAFE_OPTS=0
  # optimisation - fixed pipeline only, no shaders:
  -sGL_FFP_ONLY
  # enable when support improves (https://emscripten.org/docs/porting/exceptions.html):
  #-fwasm-exceptions
  # enable exit handling code
  -sEXIT_RUNTIME=1
  # don't emit code for node.js etc
  -sENVIRONMENT=web
  --shell-file=${CMAKE_SOURCE_DIR}/html/client.html
)

set(CMAKE_EXECUTABLE_SUFFIX ".html")
