
set(APP_SOURCES
        main.cpp
        controllers/ReaderCtrl.cpp
        models/AssetModel.cpp
        models/LayoutModel.cpp
        widgets/ImageDisplayWidget.cpp)

set(APP_HEADERS
        controllers/ReaderCtrl.hpp
        models/AssetModel.hpp
        models/LayoutModel.hpp
        widgets/ImageDisplayWidget.hpp)

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
        talespire_reader
        talespire_core
        Boost::boost
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
        Qt5::Widgets
        Qt5::QuickControls2
        opencv_core
    DEFINITIONS
        ${ENABLE_QT_QML_DEBUG}
    RESOURCES)