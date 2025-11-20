cmake_minimum_required(VERSION 3.10)

project(My_SoftWare_Renderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

aux_source_directory(src SRC)

add_executable(Renderer ${SRC})