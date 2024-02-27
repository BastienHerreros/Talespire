import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ListView {
    id: root

    property real availableWidth : root.width - (scrollBar.policy === ScrollBar.AlwaysOn ? scrollBarWidth + 2 * 5 : 0)
    property real scrollBarWidth : 6
    property bool scrollBarDisplayed : ScrollBar.vertical.policy === ScrollBar.AlwaysOn

    anchors.margins: 10
    spacing: 10
    clip: true

    ScrollBar.vertical: ScrollBar {
        id: scrollBar

        policy: root.contentHeight > root.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
}