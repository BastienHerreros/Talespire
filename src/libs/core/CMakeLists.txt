set(CORE_SOURCES
            Layout.cpp
            Asset.cpp)

set(CORE_HEADERS
            log.hpp
            Layout.hpp
            Asset.hpp)

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
    PRIVATE_LINKS
        Boost::boost
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS)

# add_subdirectory(tests)