import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    property alias type: header.text
    property alias message: body.text

    implicitHeight: 200
    implicitWidth: 500
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
        id: header

        width: parent.width
        height: 40
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font.bold: true
    }

    contentItem: Label {
        id: body

        anchors {
            fill: parent
            margins: 10
        }
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}