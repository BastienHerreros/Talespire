cmake_minimum_required(VERSION 3.27.0)

project(thirdparty LANGUAGES C CXX)

set(default_build_type "Release")
set(BUILD_SHARED_LIBS ON)

include(ExternalProject)

set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/external")

set(CMAKE_CORE_BUILD_FLAGS
    -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_DO_STRIP=${CMAKE_INSTALL_DO_STRIP}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
    
########################################
## Zlib
########################################

set(ZLIB_TARGET zlib)
ExternalProject_Add(${ZLIB_TARGET}
    GIT_REPOSITORY https://github.com/madler/zlib
    GIT_TAG v1.3.1
    PREFIX ${BUILD_DIR}
    BUILD_IN_SOURCE 0
    BUILD_ALWAYS 0
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib
    BINARY_DIR ${BUILD_DIR}/zlib_build
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND}
        ${CMAKE_CORE_BUILD_FLAGS}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <SOURCE_DIR>)
set(ZLIB_INCLUDE ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(BUILD_SHARED_LIBS)
            set(ZLIB_LIBRARY ${LIB_DIR}/zlib$<$<CONFIG:Debug>:d>${LIB_EXTENSION})
        else()
            set(ZLIB_LIBRARY ${LIB_DIR}/zlibstatic$<$<CONFIG:Debug>:d>${LIB_EXTENSION})
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(BUILD_SHARED_LIBS)
            set(ZLIB_LIBRARY ${LIB_DIR}/libzlib1${LIB_EXTENSION})
        else()
            set(ZLIB_LIBRARY ${LIB_DIR}/libzlibstatic${LIB_EXTENSION})
        endif()
    else()
        message(FATAL_ERROR "Unhandle compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()
else()
    set(ZLIB_LIBRARY ${LIB_DIR}/libz${LIB_EXTENSION})
endif()

set(ZLIB_CMAKE_FLAGS -DZLIB_ROOT=${CMAKE_INSTALL_PREFIX} -DZLIB_INCLUDE=${ZLIB_INCLUDE} -DZLIB_LIBRARY=${ZLIB_LIBRARY})

########################################
## Boost
########################################

set(BOOST_TARGET boost)

if(BUILD_SHARED_LIBS)
    set(BOOST_LINK_FLAGS link=shared)
else()
    set(BOOST_LINK_FLAGS link=static)
endif()

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(BOOST_TOOLSET msvc)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(BOOST_TOOLSET gcc)
    else()
        message(FATAL_ERROR "Unhandle compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()
else()
    set(BOOST_TOOLSET gcc)
endif()

set(BOOST_FLAGS architecture=x86 toolset=${BOOST_TOOLSET} variant=release cxxstd=20 address-model=64 threading=multi -sZLIB_LIBRARY_PATH="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" -sZLIB_INCLUDE=${ZLIB_INCLUDE} -sNO_ZSTD=1)
ExternalProject_Add(${BOOST_TARGET}
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.84.0
    GIT_SHALLOW ON
    PREFIX ${BUILD_DIR}
    BUILD_IN_SOURCE 0
    BUILD_ALWAYS 0
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost
    BINARY_DIR ${BUILD_DIR}/boost_build
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND cd <SOURCE_DIR>  && ./bootstrap.sh --prefix=<INSTALL_DIR> --with-toolset=${BOOST_TOOLSET} --with-libraries=iostreams,test
    BUILD_COMMAND ""
    INSTALL_COMMAND cd <SOURCE_DIR> && ./b2 --build-dir=<BINARY_DIR> --prefix=<INSTALL_DIR> ${BOOST_FLAGS} ${BOOST_LINK_FLAGS} -d1 -a -j8 -q install
    DEPENDS ${ZLIB_TARGET})

set(EIGEN_TARGET eigen)
ExternalProject_Add(${EIGEN_TARGET}
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen
    GIT_TAG 3.4.0
    PREFIX ${BUILD_DIR}
    BUILD_IN_SOURCE 0
    BUILD_ALWAYS 0
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/eigen
    BINARY_DIR ${BUILD_DIR}/eigen_build
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND}
        ${CMAKE_CORE_BUILD_FLAGS}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <SOURCE_DIR>)