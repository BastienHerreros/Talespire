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
        Layout.preferredHeight: labelTitle.implicitHeight

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
                margins: 2
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
                        columnSpacing: 10
                        rowSpacing: 10
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
                            text: "Convert"
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
                    text: "Replace"
                }

                Rectangle {
                    color: "lightGrey"
                    border {
                        width: 2
                        color: "black"
                    }
                    radius: 5

                    Layout.fillWidth: true
                    Layout.preferredHeight: rowReplace.implicitHeight + 40

                    RowLayout {
                        id: rowReplace

                        spacing: 10
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 20
                            verticalCenter: parent.verticalCenter
                        }
                        height: 40

                        Label {
                            text: "Replace "
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                        }

                        ComboBox {
                            id: comboBoxReplaceFrom

                            model: readerCtrl.model
                            textRole: "assetName"
                            valueRole: "assetName"
                        }

                        Label {
                            text: "with"
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                        }

                        ComboBox {
                            id: comboBoxReplaceTo

                            model: readerCtrl.fullModel
                            textRole: "assetName"
                            valueRole: "assetName"
                        }

                        Button {
                            text: "Replace"
                            enabled: comboBoxReplaceFrom.currentIndex !== -1 && comboBoxReplaceTo.currentIndex !== -1 

                            onClicked: {

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

            Layout.fillWidth: true
            Layout.fillHeight: true

            ListViewWidget {
                id: listSlab

                model: readerCtrl.model
                anchors {
                    fill: parent
                    margins: 10
                }

                delegate: LayoutDelegate {
                    width: listSlab.availableWidth
                    assetModel: model.assets

                    onLayoutSelected: {
                        listSlab.currentIndex = id;
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