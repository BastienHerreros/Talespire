import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import cpp.models 1.0

ComboBox {
    id: control

    property var imodel
    property var regEx: new RegExp(".+")
    
    model: SortFilterProxyModel {
        sourceModel: imodel
        filterRole: 258
        filterRegExp: regEx
        filterCaseSensitivity: Qt.CaseInsensitive
    }

    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: assetName
            color: "black"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        highlighted: control.highlightedIndex === index
    }

    contentItem: TextArea {
        id: filterConditionText

        text: control.displayText
        font: control.font
        color: "black"
        verticalAlignment: Text.AlignVCenter

        Keys.onReleased: {
            if(!list.opened) {
                list.open();
            }
            
            control.updateRegex();
        }

        onPressed: {
            list.open();
        }
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: 40
        border.color: control.pressed ? "lightGrey" : "white"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        id: list

        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#21be2b"
            radius: 2
        }
    }

    function updateRegex() {
        if(filterConditionText.text === "") {
            control.regEx = new RegExp(".+")
        } else {
            control.regEx = new RegExp('\\b' + filterConditionText.text + '\\b', 'i')
        }
    }
}