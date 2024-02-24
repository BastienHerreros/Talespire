cmake_minimum_required(VERSION 3.27.0)

project(talespire LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(default_build_type "Release")
set(BUILD_SHARED_LIBS ON)
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

include(GNUInstallDirs)
include(cmake/helper.cmake)

######################################
## Boost
######################################
set(BOOST_COMPONENTS iostreams unit_test_framework)

if(NOT BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS ON)
else()
  set(Boost_USE_STATIC_LIBS OFF)
endif()

find_package(Boost 1.84.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
message(STATUS "Boost found, version ${Boost_LIB_VERSION}")

add_subdirectory(src)