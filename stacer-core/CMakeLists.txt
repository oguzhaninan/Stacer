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

find_package(Qt5  COMPONENTS  Core Network REQUIRED)

add_library(${PROJECT_NAME}  STATIC  ${${PROJECT_NAME}_srcs})
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
