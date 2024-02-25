import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import pages 1.0

ApplicationWindow {
    id: root

    Material.theme: Material.Dark
    Material.accent: "white"

    title: "Reader App"
    width: 1280
    height: 720
    visibility: Window.Maximized
    visible: true
    minimumWidth: 1280
    minimumHeight: 720

    Loader {
        id: pageLoader

        sourceComponent: readerPageComponent

        anchors.fill: parent

        Component {
            id: readerPageComponent

            ReaderPage {
                anchors.fill: parent
            }
        }
    }
}