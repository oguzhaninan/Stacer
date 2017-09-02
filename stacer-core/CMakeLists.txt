cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(stacer-core)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/Info"
  "${CMAKE_CURRENT_SOURCE_DIR}/Tools"
  "${CMAKE_CURRENT_SOURCE_DIR}/Utils"
)

file(GLOB_RECURSE ${PROJECT_NAME}_srcs "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")

add_definitions(-DSTACERCORE_LIBRARY)

add_library(${PROJECT_NAME}  STATIC  ${${PROJECT_NAME}_srcs})
target_link_libraries(${PROJECT_NAME} Qt5::Core)
