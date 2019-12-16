cmake_minimum_required(VERSION 2.8.3)
project(nus_quadruped)

include_directories(include)

## Declare a cpp library
add_library(controller_lib
		src/misc_functions.cpp
		src/odrive_can_functions.cpp
# 		src/motion_controller.cpp
)

add_definitions(-std=c++17)

add_executable(quadruped_controller src/controller.cpp)
add_dependencies(quadruped_controller controller_lib)
target_link_libraries(quadruped_controller controller_lib pthread)

