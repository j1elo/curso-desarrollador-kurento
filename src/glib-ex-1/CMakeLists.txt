# <#file>

# Build commands:
#     mkdir build && cd build
#     cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..

cmake_minimum_required(VERSION 3.0)
project(glib-ex-1 VERSION 1.0.0 LANGUAGES C)

message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

find_package(PkgConfig)
pkg_search_module(GLIB2 REQUIRED glib-2.0>=2.0)

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
