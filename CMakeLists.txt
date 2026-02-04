cmake_minimum_required(VERSION 3.20)
project(image_ppm)

add_library(stb_image STATIC "external/stb_image.cpp")
target_include_directories(stb_image PUBLIC "external")

add_executable(image_ppm "source/main.cpp")
target_link_libraries(image_ppm PRIVATE stb_image)
set_target_properties(image_ppm PROPERTIES CXX_STANDARD 17)