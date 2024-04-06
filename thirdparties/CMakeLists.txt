cmake_minimum_required(VERSION 3.27.0)

project(thirdparty LANGUAGES C CXX)

include(GNUInstallDirs)
include(CMakeDependentOption)
include(ExternalProject)

set(default_build_type "Release")
set(BUILD_SHARED_LIBS ON)

set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/external")

set(CMAKE_CORE_BUILD_FLAGS
    -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_DO_STRIP=${CMAKE_INSTALL_DO_STRIP}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
    
cmake_dependent_option(BUILD_BOOST "Build boost" ON "UNIX;NOT APPLE;NOT WIN32" OFF)

file(TO_NATIVE_PATH "/" PATH_SEPARATOR)
if("${PATH_SEPARATOR}" STREQUAL "/")
    set(SCRIPT_EXTENSION sh)
else()
    set(SCRIPT_EXTENSION bat)
endif()

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
        set(LIB_EXTENSION ${CMAKE_STATIC_LIBRARY_SUFFIX})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(BUILD_SHARED_LIBS)
            set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
            set(LIB_EXTENSION ${CMAKE_SHARED_LIBRARY_SUFFIX})
        else()
            set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
            set(LIB_EXTENSION ${CMAKE_STATIC_LIBRARY_SUFFIX})
        endif()
    else()
        message(FATAL_ERROR "Unhandle compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()
else()
    set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
    if(BUILD_SHARED_LIBS)
        set(LIB_EXTENSION ${CMAKE_SHARED_LIBRARY_SUFFIX})
    else()
        set(LIB_EXTENSION ${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
    
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

if(BUILD_BOOST)
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
        CONFIGURE_COMMAND cd <SOURCE_DIR>  && ./bootstrap.sh --prefix=<INSTALL_DIR> --with-toolset=${BOOST_TOOLSET} --with-libraries=iostreams,test,serialization
        BUILD_COMMAND ""
        INSTALL_COMMAND cd <SOURCE_DIR> && ./b2 --build-dir=<BINARY_DIR> --prefix=<INSTALL_DIR> ${BOOST_FLAGS} ${BOOST_LINK_FLAGS} -d1 -a -j8 -q install
        DEPENDS ${ZLIB_TARGET})
endif()

########################################
## Eigen
########################################

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

set(EIGEN_CMAKE_FLAGS -DEigen3_DIR:PATH=${CMAKE_INSTALL_PREFIX}/share/eigen3/cmake -DEIGEN3_INCLUDE_DIR=${BUILD_DIR}/eigen_build/include/eigen3 -DEIGEN_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include/eigen3)

########################################
## OpenCV
########################################

set(OPENCV_TARGET opencv)
set(OPENCV_FILENAME 4.5.5.tar.gz)

ExternalProject_Add(${OPENCV_TARGET}
    URL https://github.com/opencv/opencv/archive/${OPENCV_FILENAME}
    DOWNLOAD_NAME opencv-${OPENCV_FILENAME}
    PREFIX ${BUILD_DIR}
    BUILD_IN_SOURCE 0
    BUILD_ALWAYS 0
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/opencv
    BINARY_DIR ${BUILD_DIR}/opencv_build
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND}
        ${CMAKE_CORE_BUILD_FLAGS}
        ${ZLIB_CMAKE_FLAGS}
        ${EIGEN_CMAKE_FLAGS}
        ${PNG_CMAKE_FLAGS}
        -DBUILD_ZLIB=OFF
        -DWITH_EIGEN=${LSURGAR_BUILD_EIGEN}
        -DWITH_TIFF=ON
        -DBUILD_TIFF=ON
        -DWITH_PNG=ON
        -DBUILD_PNG=ON
        -DWITH_OPENEXR=ON
        -DBUILD_OPENEXR=ON
        -DWITH_JPEG=ON
        -DBUILD_JPEG=ON
        -DWITH_PROTOBUF=ON
        -DBUILD_PROTOBUF=ON
        -DWITH_FFMPEG=OFF
        -DOPENCV_ENABLE_NONFREE=OFF
        -DWITH_OPENMP=ON
        -DWITH_CUDA=OFF
        -DWITH_CUBLAS=ON
        -DWITH_CUFFT=ON
        -DWITH_OPENCL=ON
        -DWITH_IPP=OFF
        -DWITH_IPP_A=OFF
        -DWITH_PTHREADS_PF=ON
        -DBUILD_TBB=OFF
        -DWITH_TBB=OFF
        -DWITH_TBB=OFF
        -DWITH_V4L=ON
        -DWITH_GTK_2_X=ON
        -DWITH_OPENGL=ON
        -DWITH_PVAPI=ON
        -DWITH_CUDNN=OFF
        -DBUILD_CUDA_STUBS=OFF
        -DBUILD_DOCS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_PERF_TESTS=OFF
        -DINSTALL_C_EXAMPLES=OFF
        -DBUILD_TESTS=OFF
        -DINSTALL_TESTS=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <SOURCE_DIR>
        ${OPENCV_CONFIGURE_FLAGS}
    DEPENDS ${ZLIB_TARGET} ${EIGEN_TARGET} ${PNG_TARGET})

    # OpenCV is configured in two steps otherwise the cmake command may exceed the maximum size of a command allowed by windows using the nmake generator
ExternalProject_Add_Step(${OPENCV_TARGET} OpenCV_configure_cmake
    WORKING_DIRECTORY ${BUILD_DIR}/opencv_build
    COMMAND ${CMAKE_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/opencv
    -DBUILD_opencv_alphamat=OFF
    -DBUILD_opencv_apps=OFF
    -DBUILD_opencv_aruco=OFF
    -DBUILD_opencv_barcode=OFF
    -DBUILD_opencv_bgsegm=OFF
    -DBUILD_opencv_bioinspired=OFF
    -DBUILD_opencv_calib3d=OFF
    -DBUILD_opencv_ccalib=OFF
    -DBUILD_opencv_core=ON
    -DBUILD_opencv_cudaarithm=OFF
    -DBUILD_opencv_cudabgsegm=OFF
    -DBUILD_opencv_cudacodec=OFF
    -DBUILD_opencv_cudafeatures2d=OFF
    -DBUILD_opencv_cudafilters=OFF
    -DBUILD_opencv_cudaimgproc=OFF
    -DBUILD_opencv_cudalegacy=OFF
    -DBUILD_opencv_cudaobjdetect=OFF
    -DBUILD_opencv_cudaoptflow=OFF
    -DBUILD_opencv_cudastereo=OFF
    -DBUILD_opencv_cudawarping=OFF
    -DBUILD_opencv_cudev=OFF
    -DBUILD_opencv_datasets=OFF
    -DBUILD_opencv_dnn=OFF
    -DBUILD_opencv_dnn_objdetect=OFF
    -DBUILD_opencv_dnn_superres=OFF
    -DBUILD_opencv_dpm=OFF
    -DBUILD_opencv_face=OFF
    -DBUILD_opencv_features2d=OFF
    -DBUILD_opencv_flann=ON
    -DBUILD_opencv_freetype=OFF
    -DBUILD_opencv_fuzzy=OFF
    -DBUILD_opencv_gapi=OFF
    -DBUILD_opencv_hfs=OFF
    -DBUILD_opencv_highgui=ON
    -DBUILD_opencv_img_hash=OFF
    -DBUILD_opencv_imgcodecs=ON
    -DBUILD_opencv_imgproc=ON
    -DBUILD_opencv_intensity_transform=OFF
    -DBUILD_opencv_java_bindings_generator=OFF
    -DBUILD_opencv_js=OFF
    -DBUILD_opencv_js_bindings_generator=OFF
    -DBUILD_opencv_line_descriptor=OFF
    -DBUILD_opencv_mcc=OFF
    -DBUILD_opencv_ml=OFF
    -DBUILD_opencv_objc_bindings_generator=OFF
    -DBUILD_opencv_objdetect=OFF
    -DBUILD_opencv_optflow=OFF
    -DBUILD_opencv_phase_unwrapping=OFF
    -DBUILD_opencv_photo=OFF
    -DBUILD_opencv_plot=OFF
    -DBUILD_opencv_python3=OFF
    -DBUILD_opencv_python3=OFF
    -DBUILD_opencv_python_bindings_generator=OFF
    -DBUILD_opencv_python_tests=OFF
    -DBUILD_opencv_quality=OFF
    -DBUILD_opencv_rapid=OFF
    -DBUILD_opencv_reg=OFF
    -DBUILD_opencv_rgbd=OFF
    -DBUILD_opencv_saliency=OFF
    -DBUILD_opencv_shape=OFF
    -DBUILD_opencv_stereo=OFF
    -DBUILD_opencv_stitching=OFF
    -DBUILD_opencv_structured_light=OFF
    -DBUILD_opencv_superres=OFF
    -DBUILD_opencv_surface_matching=OFF
    -DBUILD_opencv_text=OFF
    -DBUILD_opencv_tracking=OFF
    -DBUILD_opencv_video=OFF
    -DBUILD_opencv_videoio=OFF
    -DBUILD_opencv_videostab=OFF
    -DBUILD_opencv_viz=OFF
    -DBUILD_opencv_wechat_qrcode=OFF
    -DBUILD_opencv_world=OFF
    -DBUILD_opencv_xfeatures2d=OFF
    -DBUILD_opencv_ximgproc=OFF
    -DBUILD_opencv_xobjdetect=OFF
    -DBUILD_opencv_xphoto=OFF
    DEPENDEES configure
    DEPENDERS build)