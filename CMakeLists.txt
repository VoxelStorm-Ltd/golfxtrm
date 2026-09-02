cmake_minimum_required(VERSION 3.15)

project(GolfXTRM LANGUAGES CXX)

# The CodeBlocks project defines Debug and Release targets and defaults to
# Release. Keep the same configurations for both single- and multi-config
# generators.
get_property(_is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_is_multi_config)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING
    "Available build configurations" FORCE)
elseif(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Build type: Debug or Release" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Boost is used as a header-only dependency. Prefer its package configuration
# when available, with the classic FindBoost module as a fallback.
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()
find_package(Boost QUIET CONFIG)
if(NOT Boost_FOUND)
  find_package(Boost REQUIRED MODULE)
endif()

# A cross-build must not add a host path such as /usr/include directly: doing
# so mixes the host C runtime headers with MinGW's headers. Expose only Boost's
# own directory through an isolated include root in the build tree.
set(GOLFXTRM_BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
if(CMAKE_CROSSCOMPILING)
  unset(_boost_header_source)
  foreach(_boost_include_dir IN LISTS Boost_INCLUDE_DIRS)
    if(EXISTS "${_boost_include_dir}/boost/version.hpp")
      get_filename_component(
        _boost_header_source "${_boost_include_dir}/boost" REALPATH)
      break()
    endif()
  endforeach()

  if(NOT _boost_header_source)
    message(FATAL_ERROR
      "Boost was found, but its boost/version.hpp header could not be located")
  endif()

  string(MD5 _boost_header_path_hash "${_boost_header_source}")
  set(_boost_isolated_root
    "${CMAKE_BINARY_DIR}/boost-include-${_boost_header_path_hash}")
  set(_boost_isolated_link "${_boost_isolated_root}/boost")
  file(MAKE_DIRECTORY "${_boost_isolated_root}")

  if(NOT EXISTS "${_boost_isolated_link}")
    file(CREATE_LINK
      "${_boost_header_source}"
      "${_boost_isolated_link}"
      SYMBOLIC
      RESULT _boost_link_result)
    if(NOT _boost_link_result STREQUAL "0")
      message(FATAL_ERROR
        "Could not create isolated Boost include link: ${_boost_link_result}")
    endif()
  endif()

  set(GOLFXTRM_BOOST_INCLUDE_DIRS "${_boost_isolated_root}")
endif()

# Mirrors the retained <Unit> entries in golf client.cbp. Headers are included
# so IDE generators expose the same project files as CodeBlocks.
set(GOLFXTRM_SOURCES
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
  world.cpp
  worldcomponents_render.cpp
  worldcomponents_update.cpp

  vectorstorm/vectorstorm.cpp

  global_objects.h
  globaldefs.h
  globalvars_client.h
  globalvars_client_extern.h
  golfer.h
  holdable.h
  landscape_features.h
  objloader.h
  particle.h
  perlin.h
  progressbar.h
  terrain.h
  version.h
  worldcomponents.h
)

add_executable(GolfXTRM ${GOLFXTRM_SOURCES})

# CodeBlocks creates a console executable named "GolfXTRM debug" for Debug,
# and a GUI executable named "GolfXTRM" for Release.
set_target_properties(GolfXTRM PROPERTIES
  OUTPUT_NAME_DEBUG "GolfXTRM debug"
  OUTPUT_NAME_RELEASE "GolfXTRM"
  WIN32_EXECUTABLE "$<CONFIG:Release>"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG
    "${PROJECT_SOURCE_DIR}/bin/Win32_Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE
    "${PROJECT_SOURCE_DIR}/bin/Win32_Release/GolfXTRM"
)

target_include_directories(GolfXTRM PRIVATE
  "${PROJECT_SOURCE_DIR}"
)
target_include_directories(GolfXTRM SYSTEM PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
  ${GOLFXTRM_BOOST_INCLUDE_DIRS}
)

# Global and configuration-specific compiler settings from the CBP.
target_compile_options(GolfXTRM PRIVATE
  -Wall
  -msse2

  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-Og>

  $<$<CONFIG:Release>:-fomit-frame-pointer>
  $<$<CONFIG:Release>:-fexpensive-optimizations>
  $<$<CONFIG:Release>:-Ofast>
  $<$<CONFIG:Release>:-fgcse-sm>
  $<$<CONFIG:Release>:-fgcse-las>
  $<$<CONFIG:Release>:-fgcse-after-reload>
  $<$<CONFIG:Release>:-fsched-spec-load>
  $<$<CONFIG:Release>:-fsched-spec-load-dangerous>
  $<$<CONFIG:Release>:-fprofile-generate>
  $<$<CONFIG:Release>:-fbranch-probabilities>
  $<$<CONFIG:Release>:-fprofile-arcs>
  $<$<CONFIG:Release>:-fprofile-use>
  $<$<CONFIG:Release>:-fprofile-values>
  $<$<CONFIG:Release>:-fsched2-use-traces>
  $<$<CONFIG:Release>:-fpeel-loops>
  $<$<CONFIG:Release>:-funroll-loops>
  $<$<CONFIG:Release>:-funswitch-loops>
)

target_compile_definitions(GolfXTRM PRIVATE
  GLEW_STATIC
  $<$<CONFIG:Debug>:SKIPINTRO>
  $<$<CONFIG:Release>:INTRO>
  $<$<CONFIG:Release>:NDEBUG>
)

# Static MinGW runtime linkage is used by both original targets. The remaining
# options belong only to the release target.
target_link_options(GolfXTRM PRIVATE
  -static
  -static-libgcc
  -static-libstdc++

  $<$<CONFIG:Release>:-s>
  $<$<CONFIG:Release>:-fprofile-arcs>
  $<$<CONFIG:Release>:-fuse-linker-plugin>
  $<$<CONFIG:Release>:-flto>
  $<$<CONFIG:Release>:-funsafe-math-optimizations>
)

# Preserve the CBP's bundled-library order. These archives target 32-bit
# MinGW, so a matching compiler/toolchain is required for a successful link.
set(GOLFXTRM_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(GolfXTRM PRIVATE
  $<$<CONFIG:Release>:gcov>
  "${GOLFXTRM_LIB_DIR}/libirrKlang.a"
  "${GOLFXTRM_LIB_DIR}/libSOIL.a"
  "${GOLFXTRM_LIB_DIR}/libglew32.a"
  "${GOLFXTRM_LIB_DIR}/libglfw3.a"
  "${GOLFXTRM_LIB_DIR}/libglu32.a"
  "${GOLFXTRM_LIB_DIR}/libopengl32.a"
  "${GOLFXTRM_LIB_DIR}/libuser32.a"
  "${GOLFXTRM_LIB_DIR}/libws2_32.a"
  "${GOLFXTRM_LIB_DIR}/libgdi32.a"
  "${GOLFXTRM_LIB_DIR}/libwinmm.a"
  "${GOLFXTRM_LIB_DIR}/libsetupapi.a"
)
