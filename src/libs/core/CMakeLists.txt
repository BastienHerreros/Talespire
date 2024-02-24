set(CORE_SOURCES
            log.cpp)

set(CORE_HEADERS
            log.hpp)

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
    PRIVATE_LINKS
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS)

# add_subdirectory(tests)