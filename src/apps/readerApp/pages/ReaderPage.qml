import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import Qt.labs.settings 1.0

import widgets 1.0
import cpp.widgets 1.0

import ReaderCtrl 1.0

Rectangle {
    id: root

    color: "darkGrey"

    ReaderCtrl {
        id: readerCtrl

        onError: {
            loadingPopup.close();
            errorPopup.message = message;
            errorPopup.open();
        }

        onInitDatabaseBegin: {
            loadingPopup.open();
        }

        onInitDatabaseEnd: {
            loadingPopup.close();
        }

        onNewSlabCode: {
            newSlabCodeTextField.text = newCode;
        }
    }

    component Title: Rectangle {
        property alias text: labelTitle.text

        color: "lightGrey"
        border {
            width: 2
            color: "black"
        }
        radius: 5

        Layout.fillWidth: true
        Layout.preferredHeight: labelTitle.implicitHeight + 20

        Label {
            id: labelTitle

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font {
                bold: true
                pixelSize: 15
            }
            anchors {
                left: parent.left
                right: parent.right
                centerIn: parent
            }
        }
    }

    RowLayout {
        anchors {
            fill: parent
            margins: 10
        }
        spacing: 10

        //Left panel

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent

                Title {
                    text: "Import"
                }

                Rectangle {
                    color: "lightGrey"
                    border {
                        width: 2
                        color: "black"
                    }
                    radius: 5

                    Layout.fillWidth: true
                    Layout.preferredHeight: gridImport.implicitHeight + 40

                    GridLayout {
                        id: gridImport

                        columns: 4
                        rows: 2
                        columnSpacing: 5
                        rowSpacing: 5
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 20
                            verticalCenter: parent.verticalCenter
                        }
                        height: 80

                        readonly property real availableWidth: gridImport.width - gridImport.columnSpacing * (gridImport.columns - 1)
                        readonly property real availableHeight: gridImport.height - gridImport.rowSpacing * (gridImport.columns - 1)
                        readonly property real cellWidth: gridImport.availableWidth / gridImport.columns
                        readonly property real cellHeight: gridImport.availableHeight / gridImport.rows

                        function getPrefWidth(item){
                            return cellWidth * item.Layout.columnSpan + (item.Layout.columnSpan - 1) * gridImport.columnSpacing
                        }

                        function getPrefHeight(item){
                            return cellHeight * item.Layout.rowSpan + (item.Layout.rowSpan - 1) * gridImport.rowSpacing
                        }

                        Label {
                            text: "Taleweaver folder"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.row: 0
                            Layout.column: 0
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)
                            Layout.alignment: Qt.AlignLeft
                        }

                        TextField {
                            id: taleweaverPath

                            text : settings.taleWeaver

                            Layout.row: 0
                            Layout.column: 1
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)
                        }

                        Button {
                            text: "Browse..."

                            Layout.row: 0
                            Layout.column: 2
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)

                            onClicked: {
                                folderDialog.open();
                            }
                        }

                        Button {
                            text: "Load"

                            Layout.row: 0
                            Layout.column: 3
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)

                            onClicked: {
                                settings.taleWeaver = taleweaverPath.text;
                                readerCtrl.initDatabase(taleweaverPath.text);
                            }
                        }

                        Label {
                            text: "Slab code"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.row: 1
                            Layout.column: 0
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)
                            Layout.alignment: Qt.AlignLeft
                        }

                        TextField {
                            id: slabCode

                            placeholderText : "paste taleweaver folder path here"
                            enabled: readerCtrl.dataBaseInitialized

                            Layout.row: 1
                            Layout.column: 1
                            Layout.columnSpan: 2
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)
                        }

                        Button {
                            text: "Load slab code"
                            enabled: readerCtrl.dataBaseInitialized

                            Layout.row: 1
                            Layout.column: 3
                            Layout.preferredWidth: gridImport.getPrefWidth(this)
                            Layout.preferredHeight: gridImport.getPrefHeight(this)

                            onClicked: {
                                readerCtrl.loadSlab(slabCode.text);
                            }
                        }
                    }
                }

                Title {
                    text: "Swap assets"
                }

                Rectangle {
                    color: "lightGrey"
                    border {
                        width: 2
                        color: "black"
                    }
                    radius: 5
                    enabled: readerCtrl.model.rowCount() > 0

                    Layout.fillWidth: true
                    Layout.preferredHeight: gridReplace.implicitHeight + 40

                    GridLayout {
                        id: gridReplace
                        
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 20
                            verticalCenter: parent.verticalCenter
                        }
                        height: 80
                        columns: 5
                        rows: 1
                        columnSpacing: 5
                        rowSpacing: 5

                        readonly property real availableWidth: gridReplace.width - gridReplace.columnSpacing * (gridReplace.columns - 1)
                        readonly property real availableHeight: gridReplace.height - gridReplace.rowSpacing * (gridReplace.columns - 1)
                        readonly property real cellWidth: gridReplace.availableWidth / gridReplace.columns
                        readonly property real cellHeight: gridReplace.availableHeight / gridReplace.rows

                        function getPrefWidth(item){
                            return cellWidth * item.Layout.columnSpan + (item.Layout.columnSpan - 1) * gridReplace.columnSpacing
                        }

                        function getPrefHeight(item){
                            return cellHeight * item.Layout.rowSpan + (item.Layout.rowSpan - 1) * gridReplace.rowSpacing
                        }

                        // First row
                        Label {
                            text: "Replace"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.row: 0
                            Layout.column: 0
                            Layout.preferredWidth: gridReplace.getPrefWidth(this)
                            Layout.preferredHeight: gridReplace.getPrefHeight(this)
                            Layout.alignment: Qt.AlignLeft
                        }

                        SearchableComboBox {
                            id: comboBoxReplaceFrom

                            imodel: readerCtrl.model
                            textRole: "assetName"
                            valueRole: "listIndex"
                            
                            Layout.row: 0
                            Layout.column: 1
                            Layout.preferredWidth: gridReplace.getPrefWidth(this)
                            Layout.preferredHeight: gridReplace.getPrefHeight(this)
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Label {
                            text: "with"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            Layout.row: 0
                            Layout.column: 2
                            Layout.preferredWidth: gridReplace.getPrefWidth(this)
                            Layout.preferredHeight: gridReplace.getPrefHeight(this)
                            Layout.alignment: Qt.AlignHCenter
                        }

                        SearchableComboBox {
                            id: comboBoxReplaceTo

                            imodel: readerCtrl.fullModel
                            textRole: "assetName"
                            valueRole: "listIndex"
                            width: 200

                            Layout.row: 0
                            Layout.column: 3
                            Layout.preferredWidth: gridReplace.getPrefWidth(this)
                            Layout.preferredHeight: gridReplace.getPrefHeight(this)
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Button {
                            text: "Replace"
                            enabled: comboBoxReplaceFrom.currentIndex !== -1 && comboBoxReplaceTo.currentIndex !== -1 

                            Layout.row: 0
                            Layout.column: 4
                            Layout.preferredWidth: gridReplace.getPrefWidth(this)
                            Layout.preferredHeight: gridReplace.getPrefHeight(this)
                            Layout.alignment: Qt.AlignRight

                            onClicked: {
                                readerCtrl.replaceAsset(comboBoxReplaceFrom.currentValue, comboBoxReplaceTo.currentValue);

                                comboBoxReplaceFrom.reset();
                                comboBoxReplaceTo.reset();
                            }
                        }
                    }
                }

                Rectangle {
                    color: "lightGrey"
                    border {
                        width: 2
                        color: "black"
                    }
                    radius: 5
                    enabled: readerCtrl.model.rowCount() > 0

                    Layout.fillWidth: true
                    Layout.preferredHeight: gridRemove.implicitHeight + 40

                    GridLayout {
                        id: gridRemove
                        
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 20
                            verticalCenter: parent.verticalCenter
                        }
                        height: 80
                        columns: 3
                        rows: 1
                        columnSpacing: 5
                        rowSpacing: 5

                        readonly property real availableWidth: gridRemove.width - gridRemove.columnSpacing * (gridRemove.columns - 1)
                        readonly property real availableHeight: gridRemove.height - gridRemove.rowSpacing * (gridRemove.columns - 1)
                        readonly property real cellWidth: gridRemove.availableWidth / gridRemove.columns
                        readonly property real cellHeight: gridRemove.availableHeight / gridRemove.rows

                        function getPrefWidth(item){
                            return cellWidth * item.Layout.columnSpan + (item.Layout.columnSpan - 1) * gridRemove.columnSpacing
                        }

                        function getPrefHeight(item){
                            return cellHeight * item.Layout.rowSpan + (item.Layout.rowSpan - 1) * gridRemove.rowSpacing
                        }

                        // First row
                        Label {
                            text: "Remove"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.row: 0
                            Layout.column: 0
                            Layout.preferredWidth: gridRemove.getPrefWidth(this)
                            Layout.preferredHeight: gridRemove.getPrefHeight(this)
                            Layout.alignment: Qt.AlignLeft
                        }

                        SearchableComboBox {
                            id: comboBoxRemove

                            imodel: readerCtrl.model
                            textRole: "assetName"
                            valueRole: "listIndex"
                            
                            Layout.row: 0
                            Layout.column: 1
                            Layout.preferredWidth: gridRemove.getPrefWidth(this)
                            Layout.preferredHeight: gridRemove.getPrefHeight(this)
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Button {
                            text: "Remove"
                            enabled: comboBoxRemove.currentIndex !== -1

                            Layout.row: 0
                            Layout.column: 2
                            Layout.preferredWidth: gridRemove.getPrefWidth(this)
                            Layout.preferredHeight: gridRemove.getPrefHeight(this)
                            Layout.alignment: Qt.AlignRight

                            onClicked: {
                                readerCtrl.removeAsset(comboBoxRemove.currentValue);

                                comboBoxRemove.reset();
                            }
                        }
                    }
                }

                Rectangle {
                    color: "lightGrey"
                    border {
                        width: 2
                        color: "black"
                    }
                    radius: 5
                    enabled: readerCtrl.model.rowCount() > 0

                    Layout.fillWidth: true
                    Layout.preferredHeight: gridReplace.implicitHeight + 40

                    GridLayout {
                        id: gridNewCode
                        
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 20
                            verticalCenter: parent.verticalCenter
                        }
                        height: 80
                        columns: 3
                        rows: 1
                        columnSpacing: 5
                        rowSpacing: 5

                        readonly property real availableWidth: gridNewCode.width - gridNewCode.columnSpacing * (gridNewCode.columns - 1)
                        readonly property real availableHeight: gridNewCode.height - gridNewCode.rowSpacing * (gridNewCode.columns - 1)
                        readonly property real cellWidth: gridNewCode.availableWidth / gridNewCode.columns
                        readonly property real cellHeight: gridNewCode.availableHeight / gridNewCode.rows

                        function getPrefWidth(item){
                            return cellWidth * item.Layout.columnSpan + (item.Layout.columnSpan - 1) * gridNewCode.columnSpacing
                        }

                        function getPrefHeight(item){
                            return cellHeight * item.Layout.rowSpan + (item.Layout.rowSpan - 1) * gridNewCode.rowSpacing
                        }

                        Label {
                            text: "New slab code"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.row: 0
                            Layout.column: 0
                            Layout.preferredWidth: gridNewCode.getPrefWidth(this)
                            Layout.preferredHeight: gridNewCode.getPrefHeight(this)
                            Layout.alignment: Qt.AlignLeft
                        }

                        TextField {
                            id: newSlabCodeTextField

                            Layout.row: 0
                            Layout.column: 1
                            Layout.preferredWidth: gridNewCode.getPrefWidth(this)
                            Layout.preferredHeight: gridNewCode.getPrefHeight(this)
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Button {
                            text: "Copy code"
                            enabled: newSlabCodeTextField.text !== ""

                            Layout.row: 0
                            Layout.column: 2
                            Layout.preferredWidth: gridNewCode.getPrefWidth(this)
                            Layout.preferredHeight: gridNewCode.getPrefHeight(this)
                            Layout.alignment: Qt.AlignRight

                            onClicked: {
                                readerCtrl.copyToClipboard(newSlabCodeTextField.text);
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        // Right panel
        Rectangle {
            color: "lightGrey"
            border {
                width: 2
                color: "black"
            }
            radius: 5

            Layout.preferredWidth: parent.width/3
            Layout.fillHeight: true

            ListViewWidget {
                id: listSlab

                model: readerCtrl.model
                anchors {
                    fill: parent
                    margins: 10
                }
                currentIndex: -1

                delegate: LayoutDelegate {
                    width: listSlab.availableWidth
                    assetModel: model.assets

                    onLayoutSelected: {
                        if(listSlab.currentIndex === id) {
                            listSlab.currentIndex = -1;
                        } else {
                            listSlab.currentIndex = id;
                        }
                    }
                }
            }
        }
    }

    Settings {
        id: settings

        category: "path"
        property string taleWeaver
    }

    FolderDialog {
        id: folderDialog

        options: FolderDialog.ShowDirsOnly | FolderDialog.ReadOnly
        currentFolder: settings.taleWeaver

        onAccepted: {
            console.log(folderDialog.folder);
            taleweaverPath.text = folder;
        }
    }

    PopupWidget {
        id: errorPopup

        anchors.centerIn: root
        type: "ERROR"
    }

    LoadingPopupWidget {
        id: loadingPopup

        anchors.centerIn: root
    }
}