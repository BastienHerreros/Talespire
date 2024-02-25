import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import widgets 1.0
import cpp.widgets 1.0

Rectangle {
    id: root

    color: "darkGrey"

    RowLayout {
        anchors {
            fill: parent
            margins: 10
        }
        spacing: 10

        Rectangle {
            color: "lightGrey"
            border {
                width: 2
                color: "black"
            }
            radius: 5

            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors {
                    fill: parent
                    margins: 10
                }

                TextField {
                    placeholderText : "paste here"

                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    Layout.alignment: Qt.AlignTop
                }

                Button {
                    text: "Load..."

                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    Layout.alignment: Qt.AlignTop

                    onClicked: {

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

            Layout.fillWidth: true
            Layout.fillHeight: true

            ListViewWidget {
                id: listSlab

                anchors {
                    fill: parent
                    margins: 10
                }
                model: [1, 2, 3]
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
                                image: null

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
                                text: "placeholder"
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
}