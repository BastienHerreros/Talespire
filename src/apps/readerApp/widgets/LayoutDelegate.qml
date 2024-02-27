import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import widgets 1.0
import cpp.widgets 1.0

ItemDelegate {
    id: root

    signal layoutSelected(int id)

    property alias assetModel: assetList.model

    height: root.ListView.isCurrentItem ? 500 : 150

    background: Rectangle {
        color: root.ListView.isCurrentItem ? "lightGrey" : "white"
        border {
            width: 2
            color: root.ListView.isCurrentItem ? "darkBlue" : "darkGrey"
        }
        radius: 5
    }

    contentItem: ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }
    
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
                
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

            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: 30

                Text {
                    text: model.numberOfInstance
                    anchors.fill: parent
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: 20
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