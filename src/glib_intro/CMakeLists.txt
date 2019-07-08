# <#file>

# $ mkdir build && cd build
# $ cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..

cmake_minimum_required(VERSION 3.0)
project(glib_intro VERSION 1.0.0 LANGUAGES C)

find_package(PkgConfig)
pkg_search_module(GLIB2 REQUIRED glib-2.0>=2.0)

message("GLIB2_FOUND: ${GLIB2_FOUND}")
message("GLIB2_LIBRARIES: ${GLIB2_LIBRARIES}")
message("GLIB2_LIBRARY_DIRS: ${GLIB2_LIBRARY_DIRS}")
message("GLIB2_LDFLAGS: ${GLIB2_LDFLAGS}")
message("GLIB2_LDFLAGS_OTHER: ${GLIB2_LDFLAGS_OTHER}")
message("GLIB2_INCLUDE_DIRS: ${GLIB2_INCLUDE_DIRS}")
message("GLIB2_CFLAGS: ${GLIB2_CFLAGS}")
message("GLIB2_CFLAGS_OTHER: ${GLIB2_CFLAGS_OTHER}")

add_executable(glib_intro
    main.c)
target_link_libraries(glib_intro
    PRIVATE ${GLIB2_LIBRARIES})
target_include_directories(glib_intro
    PRIVATE ${GLIB2_INCLUDE_DIRS})
target_compile_options(glib_intro
    PRIVATE -Wall -Wextra -Wpedantic -Werror)
