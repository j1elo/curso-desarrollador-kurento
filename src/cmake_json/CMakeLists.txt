# <#file>

cmake_minimum_required(VERSION 3.0)
project(cmake_json VERSION 1.0.0 LANGUAGES C)

list(APPEND CMAKE_PREFIX_PATH /opt/cJSON/lib/cmake/cJSON)
find_package(cJSON 1.7.0 REQUIRED CONFIG)  # Adds library "cjson"

add_executable(cmake_json
    main.c
)
target_include_directories(cmake_json SYSTEM
    PRIVATE ${CJSON_INCLUDE_DIRS}
)
target_link_libraries(cmake_json
    PRIVATE cjson
)
