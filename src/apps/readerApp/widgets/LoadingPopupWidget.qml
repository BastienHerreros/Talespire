import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    implicitHeight: 200
    implicitWidth: 500
    closePolicy: Popup.NoAutoClose
    dim: true
    modal: true
    
    background: Rectangle {
        radius: 10
        color: "white"
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
        font.bold: true
    }

    contentItem: Item {
        anchors.fill: parent

        BusyIndicator {
            running: true
            anchors.centerIn: parent
            width: 100
            height: 100
        }
    }
}