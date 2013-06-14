cmake_minimum_required(VERSION 2.8.3)
project(legocar_wiimote)


find_package(catkin REQUIRED COMPONENTS roscpp turtlesim ackermann_msgs wiimote)
find_package(Boost REQUIRED COMPONENTS system)



catkin_package(
   INCLUDE_DIRS include
   CATKIN_DEPENDS roscpp turtlesim ackermann_msgs wiimote
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(legocar_wiimote_node src/legocar_wiimote_node.cpp)

target_link_libraries(legocar_wiimote_node ${catkin_LIBRARIES})
