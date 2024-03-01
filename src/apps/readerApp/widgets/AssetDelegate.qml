import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root

    height: 50
    color: "white"
    border {
        width: 2
        color: "darkGrey"
    }
    radius: 5

    ColumnLayout {
        id: mainCol

        anchors {
            fill: parent
            margins: 10
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "Position: "
                horizontalAlignment: Qt.AlignLeft
            }

            Label {
                text: "(%1, %2, %3)".arg(model.position.x).arg(model.position.y).arg(model.position.z)
                horizontalAlignment: Qt.AlignLeft
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "Rotation: "
                horizontalAlignment: Qt.AlignLeft
            }

            Label {
                text: model.rotation
                horizontalAlignment: Qt.AlignLeft
            }
        }
    }
}