cmake_minimum_required(VERSION 3.20)
project(img2tileset)

add_library(stb_image STATIC "external/stb_image.cpp")
target_include_directories(stb_image PUBLIC "external")

add_executable(img2tileset "source/main.cpp")
target_link_libraries(img2tileset PRIVATE stb_image)
set_target_properties(img2tileset PROPERTIES CXX_STANDARD 17)