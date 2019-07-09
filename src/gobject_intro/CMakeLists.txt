# <#file>

# Build commands:
#     mkdir build && cd build
#     cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..

cmake_minimum_required(VERSION 3.0)
project(gobject_intro VERSION 1.0.0 LANGUAGES C)

message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

find_package(PkgConfig)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
message(STATUS "GOBJECT_LIBRARIES: ${GOBJECT_LIBRARIES}")
message(STATUS "GOBJECT_INCLUDE_DIRS: ${GOBJECT_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME}
    main.c
    myapp_person.h myapp_person.c
    myapp_student.h myapp_student.c
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${GOBJECT_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${GOBJECT_INCLUDE_DIRS}
)
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic #-Werror
)
