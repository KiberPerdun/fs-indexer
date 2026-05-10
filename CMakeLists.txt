cmake_minimum_required(VERSION 3.20)

project(fs-indexer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

add_executable(${PROJECT_NAME} ${SOURCES}
        src/main.cpp
        include/server.h
        include/server.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -march=native -fsanitize=address -Wall -Wextra)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
