cmake_minimum_required(VERSION 2.8.3)
project(nus_quadruped)

include_directories(include)

## Declare a cpp library
add_library(controller_lib
		src/misc_functions.cpp
		src/odrive_can_functions.cpp
 		src/task.cpp
 		src/request_msg.cpp
		src/signal_generator.cpp

)

add_definitions(-std=c++17)
add_definitions(-DMOTOR_COUNT=1 -DN_READ=1 -DN_WRITE=1 -DANALOG_SCALE=200 -DRT_PERIOD_1=2000000 -DRT_PERIOD_2=1000000 -DSCHED_P_1=81 -DSCHED_P_2=80 -DK_P=0.004 -DCURR_FF=1.5)	

add_executable(quadruped_controller src/main.cpp)
add_dependencies(quadruped_controller controller_lib)
target_link_libraries(quadruped_controller controller_lib pthread comedi)


# add_executable(526_test src/526_test.cpp)
# add_dependencies(526_test controller_lib)
# target_link_libraries(526_test controller_lib comedi)
