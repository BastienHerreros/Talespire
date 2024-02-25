set(CORE_SOURCES
            Asset.cpp
            AssetsDatabase.cpp
            Layout.cpp
            utils.cpp)

set(CORE_HEADERS
            Asset.hpp
            AssetsDatabase.hpp
            Layout.hpp
            log.hpp
            utils.hpp)

helper_add_library(talespire_core
    SOURCES
        ${CORE_SOURCES}
    HEADERS
        ${CORE_HEADERS}
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${generated_dir}>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
        Eigen3::Eigen
        opencv_core
    PRIVATE_LINKS
        Boost::boost
        opencv_imgcodecs
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS)

# add_subdirectory(tests)