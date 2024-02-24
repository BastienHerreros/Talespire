set(READER_SOURCES
            base64.cpp
            gzip.cpp
            reader.cpp)

set(READER_HEADERS
            base64.hpp
            gzip.hpp
            reader.hpp)

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
        talespire_core
    PRIVATE_LINKS
        Boost::iostreams
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS)

add_subdirectory(tests)