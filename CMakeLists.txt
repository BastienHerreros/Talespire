cmake_minimum_required(VERSION 3.27.0)

project(talespire LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(default_build_type "Release")
set(BUILD_SHARED_LIBS ON)
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

include(GNUInstallDirs)

set(OUTPUT_FOLDER_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${OUTPUT_FOLDER_NAME}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${OUTPUT_FOLDER_NAME}")

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MINGW64_BIN_FOLDER_PATH CACHE PATH "The path to the mingw bin folder (Usually C:/..path_to_msys../mingw64/bin)")
    set(INTERNAL_MINGW64_BIN_FOLDER_PATH ${MINGW64_BIN_FOLDER_PATH} CACHE INTERNAL "INTERNAL_MINGW64_BIN_FOLDER_PATH")
endif()

include(cmake/package.cmake)
include(cmake/helper.cmake)

enable_testing()

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

######################################
## Qt
######################################
set(LSURGAR_QT_REQUIRED_VERSION 5.15)
find_package(Qt5 ${LSURGAR_QT_REQUIRED_VERSION} REQUIRED COMPONENTS Qml Quick Svg QuickControls2)
if(Qt5_FOUND)
    message(STATUS "Qt found, version ${Qt5_VERSION}")
endif()


message(STATUS "OpenCV found, version ${OpenCV_VERSION}")

add_subdirectory(src)