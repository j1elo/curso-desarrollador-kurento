# <#file>

cmake_minimum_required(VERSION 3.0)
project(demo VERSION 1.0.0 LANGUAGES C)

add_library(make_calc SHARED
    make_intro_calc.c
)

target_link_libraries(make_calc m)

add_executable(make_intro
    make_intro_main.c
)

target_link_libraries(make_intro make_calc)

install(TARGETS make_intro make_calc DESTINATION ${CMAKE_SOURCE_DIR})

add_custom_target(run COMMAND make_intro)
