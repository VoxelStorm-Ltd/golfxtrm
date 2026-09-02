cmake_minimum_required(VERSION 3.15)

project(GolfXTRM LANGUAGES CXX)

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

# Boost is used as a header-only dependency. A cross-build must not add a host
# path such as /usr/include directly, because that mixes the host C runtime
# headers with MinGW's headers. Locate host Boost explicitly, then expose only
# its boost directory through an isolated include root in the build tree.
if(CMAKE_CROSSCOMPILING)
  find_path(GOLFXTRM_HOST_BOOST_INCLUDE_DIR
    NAMES boost/version.hpp
    HINTS ENV BOOST_ROOT ENV BOOST_INCLUDEDIR
    PATHS /usr/local/include /usr/include
    PATH_SUFFIXES include
    NO_CMAKE_FIND_ROOT_PATH
  )
  if(NOT GOLFXTRM_HOST_BOOST_INCLUDE_DIR)
    message(FATAL_ERROR
      "Boost headers could not be located on the build host")
  endif()

  get_filename_component(_boost_header_source
    "${GOLFXTRM_HOST_BOOST_INCLUDE_DIR}/boost" REALPATH)
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
else()
  if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
  endif()
  find_package(Boost QUIET CONFIG)
  if(NOT Boost_FOUND)
    find_package(Boost REQUIRED MODULE)
  endif()
  set(GOLFXTRM_BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
endif()

add_executable(GolfXTRM
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

target_link_options(GolfXTRM PRIVATE
  -static
  -static-libgcc
  -static-libstdc++

  $<$<CONFIG:Release>:-s>
  $<$<CONFIG:Release>:-fuse-linker-plugin>
  $<$<CONFIG:Release>:-flto>
  $<$<CONFIG:Release>:-funsafe-math-optimizations>
)

set(GOLFXTRM_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(GolfXTRM PRIVATE
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
