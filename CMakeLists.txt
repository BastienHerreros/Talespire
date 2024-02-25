cmake_minimum_required(VERSION 3.27.0)

project(talespire LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(default_build_type "Release")
set(BUILD_SHARED_LIBS ON)
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

include(GNUInstallDirs)
include(cmake/helper.cmake)

set(OUTPUT_FOLDER_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${OUTPUT_FOLDER_NAME}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${OUTPUT_FOLDER_NAME}")

######################################
## Boost
######################################
set(BOOST_COMPONENTS iostreams unit_test_framework serialization)

if(NOT BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS ON)
else()
  set(Boost_USE_STATIC_LIBS OFF)
endif()

find_package(Boost 1.84.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
message(STATUS "Boost found, version ${Boost_LIB_VERSION}")

######################################
## EIGEN
######################################
find_package(Eigen3 3.4.0 REQUIRED CONFIG)
message(STATUS "Eigen3 found, version ${Eigen3_VERSION}")

######################################
## OPENCV
######################################
find_package(OpenCV 4.5.5 REQUIRED COMPONENTS opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)

message(STATUS "OpenCV found, version ${OpenCV_VERSION}")

add_subdirectory(src)