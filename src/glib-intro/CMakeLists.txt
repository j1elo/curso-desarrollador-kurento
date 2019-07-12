# <#file>

# Build commands:
#     mkdir build && cd build
#     cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..

cmake_minimum_required(VERSION 3.0)
project(glib-intro VERSION 1.0.0 LANGUAGES C)

message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

find_package(PkgConfig)
pkg_search_module(GLIB2 REQUIRED glib-2.0>=2.0)

message(STATUS "GLIB2_FOUND: ${GLIB2_FOUND}")
message(STATUS "GLIB2_LIBRARIES: ${GLIB2_LIBRARIES}")
message(STATUS "GLIB2_LIBRARY_DIRS: ${GLIB2_LIBRARY_DIRS}")
message(STATUS "GLIB2_LDFLAGS: ${GLIB2_LDFLAGS}")
message(STATUS "GLIB2_LDFLAGS_OTHER: ${GLIB2_LDFLAGS_OTHER}")
message(STATUS "GLIB2_INCLUDE_DIRS: ${GLIB2_INCLUDE_DIRS}")
message(STATUS "GLIB2_CFLAGS: ${GLIB2_CFLAGS}")
message(STATUS "GLIB2_CFLAGS_OTHER: ${GLIB2_CFLAGS_OTHER}")

add_executable(${PROJECT_NAME}
    main.c
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${GLIB2_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${GLIB2_INCLUDE_DIRS}
)
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic #-Werror
)
