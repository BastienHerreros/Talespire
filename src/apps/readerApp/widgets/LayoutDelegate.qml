import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import widgets 1.0
import cpp.widgets 1.0

ItemDelegate {
    id: root

    signal layoutSelected(int id)

    property alias assetModel: assetList.model

    height: root.ListView.isCurrentItem ? 250 : 75

    background: Rectangle {
        color: "white"
        border {
            width:  root.ListView.isCurrentItem ? 4 : 2
            color: root.ListView.isCurrentItem ? "darkBlue" : "darkGrey"
        }
        radius: 5
    }

    contentItem: ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 75

            RowLayout {
                anchors.fill: parent
                    
                Rectangle {
                    color: "white"
                    border {
                        width: 2
                        color: "darkGrey"
                    }
                    radius: 5

                    Layout.fillHeight: true
                    Layout.preferredWidth: height

                    ImageDisplayWidget {
                        image: model.qtImage

                        anchors {
                            fill: parent
                            margins: 2
                        }
                    }
                }

                Text {
                    text: model.assetName + " - (" + model.assetType + ")"
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: 15

                    Layout.fillHeight: true
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    text: model.numberOfInstance
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: 15

                    Layout.fillHeight: true
                }
            }
        }
       
        Rectangle {
            color: "lightGrey"
            border {
                width: 2
                color: "darkGrey"
            }
            visible: root.ListView.isCurrentItem

            Layout.fillHeight: true
            Layout.fillWidth: true

            ListViewWidget {
                id: assetList

                delegate: AssetDelegate {
                    width: assetList.availableWidth
                }
                anchors {
                    fill: parent
                    margins: 10
                }
            }
        }

    }

    onClicked: {
        layoutSelected(index);
    }
}