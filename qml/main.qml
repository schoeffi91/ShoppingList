import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Rectangle {
    id: rootRect
    visible:true
    height: 500
    width: 500

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        ColumnLayout {
            anchors.fill: parent

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ShoppingModel
                clip: true
                delegate: Item {
                    property bool elapsed: false
                    width: rootRect.width
                    height: 50

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 10
                        color: "white"
                        opacity: model.bought ? 0.5 : 1

                        Text {
                            id: myText
                            text: model.article
                            anchors.fill: parent
                            anchors.margins: 10
                            verticalAlignment: Text.AlignVCenter
                        }

                        Rectangle {
                            width: parent.width - 10
                            height: 1
                            color: myText.color
                            anchors.centerIn: parent
                            visible: model.bought
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                model.bought = !model.bought;
                            }
                        }
                    }
                }

                Connections {
                    target: lv.model
                    onErrorHappend: {
                        messageDialog.text = message;
                        messageDialog.title = title
                        messageDialog.icon = StandardIcon.Critical;
                        messageDialog.visible = true;
                    }
                    onInfoHappend: {
                        messageDialog.text = message;
                        messageDialog.title = title
                        messageDialog.icon = StandardIcon.Information;
                        messageDialog.visible = true;
                    }
                }

                MessageDialog {
                    id: messageDialog
                    Component.onCompleted: visible = false
                }
             }

            Item {
                Layout.preferredHeight: rootRect.height * 0.15
                Layout.fillWidth: true

                RowLayout {
                    spacing: 10
                    anchors.fill: parent
                    anchors.margins: 10
                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Hinzufügen");
                    }

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Löschen");
                    }
                }
            }
        }
    }
}
