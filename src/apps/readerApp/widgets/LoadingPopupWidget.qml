import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    implicitHeight: 500
    implicitWidth: 500

    background: Rectangle {
        radius: 10
        color: "lightGrey"
        border {
            color: "darkGrey"
            width: 2
        }
    }

    header: Label {
        width: parent.width
        height: 40
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        text: "Loading..."
    }

    contentItem: BusyIndicator {
        anchors {
            fill: parent
            margins: 10
            centerIn: parent
        }
        running: true
    }
}