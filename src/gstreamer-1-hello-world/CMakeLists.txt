# <#file>

# Build commands:
#   mkdir build && cd build
#   cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..
#   make
#
# Run variables (export):
#   GST_DEBUG_DUMP_DOT_DIR="$PWD" -- Enables exporting GStreamer DOT graphs
#   GST_DEBUG="3" -- Enables Warning and Error logs from GStreamer
#   G_DEBUG="fatal-warnings" -- Breaks on GLib assertions
#
# Dependencies:
#   libgstreamer1.0-dev
#   gstreamer1.0-plugins-good

cmake_minimum_required(VERSION 3.0)
project(gstreamer-demo VERSION 1.0.0 LANGUAGES C)

message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

find_package(PkgConfig)
pkg_search_module(GST REQUIRED gstreamer-1.0)

add_executable(${PROJECT_NAME}
    main.c
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${GST_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${GST_INCLUDE_DIRS}
)
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wno-unused-parameter #-Werror
)
