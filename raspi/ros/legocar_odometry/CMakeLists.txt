cmake_minimum_required(VERSION 2.8.3)
project(legocar_odometry)


find_package(catkin REQUIRED COMPONENTS roscpp nav_msgs tf)
find_package(Boost REQUIRED COMPONENTS system)


catkin_package(
   INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp nav_msgs tf
  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(legocar_odometry_node src/legocar_odometry_node.cpp)

target_link_libraries(legocar_odometry_node
   ${catkin_LIBRARIES}
   pigpio
   pthread
   rt
)