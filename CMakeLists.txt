cmake_minimum_required(VERSION 3.7)

project(life)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TFF_INCLUDE_DIRS} include)

add_subdirectory(src)
