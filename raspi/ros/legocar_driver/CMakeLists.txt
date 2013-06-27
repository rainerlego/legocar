cmake_minimum_required(VERSION 2.8.3)
project(legocar_driver)

find_package(catkin REQUIRED COMPONENTS roscpp)
find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES legocar_driver
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(legocar_driver_node src/legocar_driver_node.cpp)
target_link_libraries(legocar_driver_node
   ${catkin_LIBRARIES}
 )