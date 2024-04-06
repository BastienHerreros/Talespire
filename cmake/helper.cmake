#
# Create a new library
#
# Usage:
#   helper_add_library(
#                SOURCES [item1...]
#                HEADERS [item1...]
#                [PUBLIC_INCLUDES [item1...] ]
#                [PRIVATE_INCLUDES [item1...] ]
#                [PUBLIC_LINKS [item1...] ]
#                [PRIVATE_LINKS [item1...] ]
#                [PUBLIC_DEFINITIONS  [item1 ...]]
#                [PRIVATE_DEFINITIONS [item1 ...]])
#
# SOURCES are the sources of the library
# HEADERS are the headers files associated to the library
# PUBLIC_INCLUDES an optional list of include directories that are public dependencies
# PRIVATE_INCLUDES an optional list of include directories that are private dependencies
# PUBLIC_LINKS an optional list of targets/libraries to link with as public dependencies
# PRIVATE_LINKS an optional list of targets/libraries to link with as private dependencies
# PUBLIC_DEFINITIONS an optional list of public definitions to add to the target
# PRIVATE_DEFINITIONS an optional list of private definitions to add to the target
#
#
function(helper_add_library param_name)
    set(_options "")
    set(_singleValues "")
    set(_multipleValues SOURCES HEADERS PUBLIC_INCLUDES PRIVATE_INCLUDES PUBLIC_LINKS PRIVATE_LINKS PUBLIC_DEFINITIONS PRIVATE_DEFINITIONS)

    cmake_parse_arguments(_param "${_options}" "${_singleValues}" "${_multipleValues}" ${ARGN})

    if(NOT param_name)
        message(FATAL_ERROR "You must provide the library name in 'helper_add_library'")
    endif()

    if(NOT _param_SOURCES AND NOT _param_HEADERS)
        message(FATAL_ERROR "You must provide the library SOURCES and/or HEADERS in 'helper_add_library'")
    endif()

    add_library(${param_name} ${_param_SOURCES} ${_param_HEADERS} )

    target_link_libraries(${param_name}
        PUBLIC
        ${_param_PUBLIC_LINKS}
        PRIVATE
        ${_param_PRIVATE_LINKS}
        ${COVERAGE_LINK_OPTIONS}
        )

    target_include_directories(${param_name}
        PUBLIC
        ${_param_PUBLIC_INCLUDES}
        PRIVATE
        ${_param_PRIVATE_INCLUDES}
        )

    target_compile_options(${param_name} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
        -Wdouble-promotion
        -Wformat=2
        -Wno-format-y2k
        -Werror
        -Wno-unused-parameter
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op>)

    target_compile_definitions(${param_name} PUBLIC ${_param_PUBLIC_DEFINITIONS} PRIVATE ${_param_PRIVATE_DEFINITIONS})

    install(TARGETS ${param_name}
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
endfunction()

#
# Create a new executable
#
# Usage:
#   helper_add_executable(
#                SOURCES <sources>
#                [INCLUDE_DIRS [item1...] ]
#                [LINKS [item1...] ]
#                [DEFINITIONS [item1...] ])
#
# SOURCES are the sources of the executable
# INCLUDE_DIRS an optional list of include directories
# LINKS an optional list of target/libraries to link with
# DEFINITIONS an optional list of private definitions to add to the executable
#
#
function(helper_add_executable param_name)
    set(_options "")
    set(_singleValues "")
    set(_multipleValues SOURCES INCLUDE_DIRS LINKS DEFINITIONS)

    cmake_parse_arguments(_param "${_options}" "${_singleValues}" "${_multipleValues}" ${ARGN})

    if(NOT param_name)
        message(FATAL_ERROR "You must provide the software name in 'helper_add_executable'")
    endif()

    if(NOT _param_SOURCES)
        message(FATAL_ERROR "You must provide the software SOURCES in 'helper_add_executable'")
    endif()

    add_executable(${param_name} ${_param_SOURCES} ${_appIcon})

    target_link_libraries(${param_name}
        PRIVATE ${_param_LINKS}
        )

    target_include_directories(${param_name}
        PRIVATE ${_param_INCLUDE_DIRS}
        )

        target_compile_options(${param_name} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wmisleading-indentation
            -Wdouble-promotion
            -Wformat=2
            -Wno-format-y2k
            -Werror
            -Wno-unused-parameter
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op>)

        target_compile_definitions(${param_name} PRIVATE ${_param_DEFINITIONS})

        install(TARGETS ${param_name}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
endfunction()

#
# Create an executable test from source files.
#
# Usage:
#   helper_add_test(
#                NAME <name>
#                SOURCES <sources>
#                [PREFIX <string>]
#                [INCLUDE_DIRS [item1...] ]
#                [LINKS [item1...]]
#                [DEFINITIONS [item1...] ])
#
# NAME is the name of the test
# PREFIX an optional string to append to the name of the test (e.g. the module name)
#     For example if foo.cpp is passed as source, and "bar" as PREFIX the executable
#     will be named named "bar_foo". Without PREFIX it will be just "foo"
# SOURCES are the source files contaning the test
# INCLUDE_DIRS an optional list of include directories
# LINKS an optional list of targets/libraries to link with
# DEFINITIONS an optional list private definitions to add to the executable
#
# The function parses each test file and extract the BOOST_AUTO_TEST_SUITEs so
# that each suite is run and visualised as a separated test. This means that no
# BOOST_AUTO_TEST_CASE must be left outside the suite or it will not be run during make test
#
# Function based on https://eb2.co/blog/2015/06/driving-boost-dot-test-with-cmake/
#
function(helper_add_test)
    set(_options "")
    set(_oneValueArgs NAME PREFIX)
    set(_multiValueArgs SOURCES INCLUDE_DIRS LINKS DEFINITIONS)
    cmake_parse_arguments(_param "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

    if(NOT ${_param_PREFIX} STREQUAL "")
        set(_testExecutableName "${_param_PREFIX}_${_param_NAME}")
    endif()

    add_executable(${_testExecutableName} ${_param_SOURCES})

    target_include_directories(${_testExecutableName} PRIVATE ${_param_INCLUDE_DIRS})

    target_compile_options(${_testExecutableName} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
        -Wdouble-promotion
        -Wformat=2
        -Wno-format-y2k
        -Werror
        -Wno-unused-parameter
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op>)

    if(BUILD_SHARED_LIBS)
        list(APPEND _param_DEFINITIONS -DBOOST_TEST_DYN_LINK)
    endif()
    if(_param_DEFINITIONS)
        target_compile_definitions(${_testExecutableName} PRIVATE ${_param_DEFINITIONS})
    endif()

    target_link_libraries(${_testExecutableName}
        ${_param_LINKS}
        Boost::unit_test_framework)

    add_test(NAME ${_testExecutableName}
        COMMAND ${_testExecutableName} --catch_system_error=yes)
endfunction()
