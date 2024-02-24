set(READER_SOURCES)

set(READER_HEADERS)

helper_add_library(talespire_reader
    SOURCES
        ${READER_SOURCES}
    HEADERS
        ${READER_HEADERS}
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${generated_dir}>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
    PRIVATE_LINKS
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS)

add_subdirectory(tests)