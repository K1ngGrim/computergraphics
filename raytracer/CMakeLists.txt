cmake_minimum_required (VERSION 3.9)
project (raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

add_executable(geometry_test geometry/geometry_test.cc math/math.cc geometry/geometry.cc)
target_link_libraries(geometry_test gtest gtest_main)

add_executable(math_test math/math_test.cc math/math.cc)
target_link_libraries(math_test gtest gtest_main)

add_executable(raytracer raytracer.cc math/math.cc view/window.cc view/camera.cc utility/color.cc utility/light.cc geometry/geometry.cc world/world.cc)
target_link_libraries(raytracer SDL2)