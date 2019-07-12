# <#file>

cmake_minimum_required(VERSION 3.0)
project(demo VERSION 1.0.0 LANGUAGES C)

add_library(make-calc SHARED
    make-intro-calc.c
)
target_link_libraries(make-calc
    PUBLIC m
)

add_executable(make-intro
    make-intro-main.c
)
target_link_libraries(make-intro
    PRIVATE make-calc
)

install(
    TARGETS make-intro make-calc
    DESTINATION ${CMAKE_SOURCE_DIR}
)

add_custom_target(run
    COMMAND make-intro
)
