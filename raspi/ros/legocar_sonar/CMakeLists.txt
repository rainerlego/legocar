cmake_minimum_required(VERSION 2.8.3)
project(legocar_sonar)


find_package(catkin REQUIRED COMPONENTS roscpp sensor_msgs)
find_package(Boost REQUIRED COMPONENTS system)


catkin_package(
   INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp sensor_msgs
  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(legocar_sonar_node src/legocar_sonar_node.cpp)

target_link_libraries(legocar_sonar_node
   ${catkin_LIBRARIES}
)