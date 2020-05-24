cmake_minimum_required(VERSION 3.6)
project(glad)

include_directories("include/")

set(SOURCE src/glad.c)
add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(glad PUBLIC ".")
target_link_libraries(glad)