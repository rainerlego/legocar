cmake_minimum_required(VERSION 2.8.3)
project(pinky_driver)


find_package(catkin REQUIRED COMPONENTS ackermann_msgs roscpp)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES pinky_driver
#  CATKIN_DEPENDS ackermann_msgs roscpp
#  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(pinky_driver_node src/pinky_driver_node.cpp)

target_link_libraries(pinky_driver_node
   pigpio
   pthread
   rt
   ${catkin_LIBRARIES}
)