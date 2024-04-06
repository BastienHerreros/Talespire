#
# Function for retrieving a list of directories to search for libs
#
# Usage:
#   getDirsToSearch(
#            outVar name
#
# outVar the returned list
#
function(getDirsToSearch outVar)
    set(_libFolders lib)
    if(WIN32)
        list(APPEND _libFolders "bin")
    endif()

    set(${outVar}
        "D:/programs/msys2/mingw64/bin"
        ${EXECUTABLE_OUTPUT_PATH})

    if(Qt5_DIR)
        foreach(_folder ${_libFolders})
            list(APPEND ${outVar} ${Qt5_DIR}/../../../${_folder})
        endforeach()
    endif()

    foreach(_prefixPath ${CMAKE_PREFIX_PATH})
        foreach(_folder ${_libFolders})
            set(_path ${_prefixPath}/${_folder})
            if(EXISTS ${_path})
                list(APPEND ${outVar} ${_path})
            endif()
        endforeach()
    endforeach()

    if(WIN32)
        foreach(_prefixPath ${CMAKE_PREFIX_PATH})
            foreach(_folder ${_libFolders})
                set(_path ${_prefixPath}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/${_folder})
                if(EXISTS ${_path})
                    list(APPEND ${outVar} ${_path})
                endif()
            endforeach()
        endforeach()
    endif()

    # Add build folders relative to submodules
    file(GLOB _externals RELATIVE ${CMAKE_SOURCE_DIR}/src/external ${CMAKE_SOURCE_DIR}/src/external/*)
    foreach(_external ${_externals})
        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/src/external/${_external})
            list(APPEND ${outVar} "${CMAKE_BINARY_DIR}/src/external/${_external}/${USURGAR_OUTPUT_FOLDER_NAME}")
        endif()
    endforeach()

    return(PROPAGATE ${outVar})
endfunction()


#
# Function for installing a target
#
# Usage:
#   install_target(
#            target_name name
#
# target_name the name of the target
#
function(install_target targetName)
    if(UNIX AND NOT APPLE)
        set(_libDest ${CMAKE_INSTALL_LIBDIR})
    elseif(WIN32)
        set(_libDest ${CMAKE_INSTALL_BINDIR})
    endif()

    install(TARGETS ${targetName}
        RUNTIME_DEPENDENCY_SET ${targetName}_deps
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${_libDest}
    )

    getDirsToSearch(_dirsToSearch)

    install(RUNTIME_DEPENDENCY_SET ${targetName}_deps
        PRE_EXCLUDE_REGEXES
            [[libc\.so\..*]]
            [[libgcc_s\.so\..*]]
            [[libm\.so\..*]]
            [[libstdc\+\+\.so\..*]]
            [[api-ms-win-.*]]
            [[ext-ms-.*]]
            [[kernel32\.dll]]
            [[/usr/lib/]]
            [[/usr/local/lib/]]
            [[libz.1.dylib]]
        POST_EXCLUDE_REGEXES
            [[/\.\./]]
            [[x86_64-linux-gnu]]
            [[system32]]
            [[sytemResources]]
            [[sysWOW64]]
            [[/usr/lib/]]
            [[/usr/local/lib/]]
        DIRECTORIES
            ${_dirsToSearch}
        RUNTIME DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION
            ${_libDest}
        FRAMEWORK DESTINATION
            ${_libDest}
    )

    if(WIN32)
        get_target_property(_exeSourceDir ${param_name} SOURCE_DIR)
        install(CODE "execute_process(COMMAND ${Qt5_DIR}/../../../bin/windeployqt.exe --dir ${CMAKE_INTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} --qmldir ${_exeSourceDir} --libdir ${CMAKE_INTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} --plugindir ${CMAKE_INTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} --no-translations --no-system-d3d-compiler --no-virtualkeyboard --no-webkit2 --no-compiler-runtime ${EXECUTABLE_OUTPUT_PATH}/${targetName}.exe)")
    endif()
endfunction()
