cmake_minimum_required(VERSION 3.0.2)
project(lidar_img_syn_ws)
 
## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages OpenCV
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  message_filters
  cv_bridge
  message_generation
  pcl_ros
  genmsg
)
find_package(PCL 1.8 REQUIRED)
find_package(OpenCV REQUIRED )
#find_package(OpenCV)
#find_library(OpenCV_LIBS opencv_core "/usr/local/lib" )
 catkin_package(
    CATKIN_DEPENDS
    roscpp
    std_msgs
    sensor_msgs
    message_filters
    cv_bridge 
    )
    #${OpenCV_INCLUDE_DIRS}
include_directories( ${catkin_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} )
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
# include_directories(include ${PCL_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
add_executable(lidar_img_syn_lmx src/lidar_img_syn_lmx.cpp)
target_link_libraries (lidar_img_syn_lmx ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(lidar_img_syn_node src/lidar_img_syn_node.cpp)
target_link_libraries (lidar_img_syn_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
