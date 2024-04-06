set(CPACK_GENERATOR "ZIP")

set(CPACK_PACKAGE_NAME "Talespire-SlabCode-Reader-App")

set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MAJOR 0)

SET(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_INSTALL_PREFIX})

#Force cpack to pack all the subdirectories (because there is an issue with the symbolic links and Windows)
list(APPEND CPACK_INSTALLED_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/bin/;bin/")
list(APPEND CPACK_INSTALLED_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib/;lib/")

## Checksum
set(CPACK_PACKAGE_CHECKSUM SHA512)

include(CPack)