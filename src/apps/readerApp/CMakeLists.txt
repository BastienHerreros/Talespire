
set(APP_SOURCES
        main.cpp)

set(APP_HEADERS)

set(QML_RESOURCES
    qml.qrc)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(ENABLE_QT_QML_DEBUG -DQT_QML_DEBUG=1)
endif()

helper_add_executable(readerApp
    SOURCES
        ${APP_SOURCES}
        ${APP_HEADERS}
        ${QML_RESOURCES}
    INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/>
    LINKS
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
        Qt5::Widgets
        Qt5::QuickControls2
    DEFINITIONS
        ${ENABLE_QT_QML_DEBUG}
    RESOURCES)