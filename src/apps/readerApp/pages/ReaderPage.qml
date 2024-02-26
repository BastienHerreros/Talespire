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

    RowLayout {
        anchors {
            fill: parent
            margins: 10
        }
        spacing: 10

        //Left panel
        Rectangle {
            color: "lightGrey"
            border {
                width: 2
                color: "black"
            }
            radius: 5

            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: gridHeader

                columns: 4
                rows: 2
                columnSpacing: 10
                rowSpacing: 10
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 20
                }
                height: 80

                readonly property real availableWidth: gridHeader.width - gridHeader.columnSpacing * (gridHeader.columns - 1)
                readonly property real availableHeight: gridHeader.height - gridHeader.rowSpacing * (gridHeader.columns - 1)
                readonly property real cellWidth: gridHeader.availableWidth / gridHeader.columns
                readonly property real cellHeight: gridHeader.availableHeight / gridHeader.rows

                function getPrefWidth(item){
                    return cellWidth * item.Layout.columnSpan + (item.Layout.columnSpan - 1) * gridHeader.columnSpacing
                }

                function getPrefHeight(item){
                    return cellHeight * item.Layout.rowSpan + (item.Layout.rowSpan - 1) * gridHeader.rowSpacing
                }

                Label {
                    text: "Taleweaver folder"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap

                    Layout.row: 0
                    Layout.column: 0
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)
                    Layout.alignment: Qt.AlignLeft
                }

                TextField {
                    id: taleweaverPath

                    text : settings.taleWeaver

                    Layout.row: 0
                    Layout.column: 1
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)
                }

                Button {
                    text: "Browse..."

                    Layout.row: 0
                    Layout.column: 2
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)

                    onClicked: {
                        folderDialog.open();
                    }
                }

                Button {
                    text: "Load"

                    Layout.row: 0
                    Layout.column: 3
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)

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
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)
                    Layout.alignment: Qt.AlignLeft
                }

                TextField {
                    id: slabCode

                    placeholderText : "paste taleweaver folder path here"
                    enabled: readerCtrl.dataBaseInitialized

                    Layout.row: 1
                    Layout.column: 1
                    Layout.columnSpan: 2
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)
                }

                Button {
                    text: "Convert"
                    enabled: readerCtrl.dataBaseInitialized

                    Layout.row: 1
                    Layout.column: 3
                    Layout.preferredWidth: gridHeader.getPrefWidth(this)
                    Layout.preferredHeight: gridHeader.getPrefHeight(this)

                    onClicked: {
                        readerCtrl.loadSlab(slabCode.text);
                    }
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

                anchors {
                    fill: parent
                    margins: 10
                }
                model: readerCtrl.model
                delegate: Rectangle {
                    id: delegate

                    width: listSlab.availableWidth
                    height: 150
                    color: "white"
                    border {
                        width: 2
                        color: "darkGrey"
                    }
                    radius: 5

                    RowLayout {
                        anchors {
                            fill: parent
                            margins: 5
                        }

                        Rectangle {
                            color: "white"
                            border {
                                width: 2
                                color: "darkGrey"
                            }
                            radius: 5

                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            ImageDisplayWidget {
                                image: model.qtImage

                                anchors {
                                    fill: parent
                                    margins: 2
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            Text {
                                text: model.assetName
                                anchors.fill: parent
                                horizontalAlignment: Qt.AlignLeft
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: 20
                            }
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