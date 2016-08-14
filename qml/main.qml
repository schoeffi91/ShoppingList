import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

Rectangle {
    id: rootRect
    visible:true
    height: 500
    width: 850

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        ColumnLayout {
            anchors.fill: parent

            ListView {
                id: lv
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ShoppingModel
                clip: true
                currentIndex: -1
                delegate: Item {
                    property bool elapsed: false
                    width: rootRect.width
                    height: 50

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 10
                        color: model.selected ? "lightblue" : "white"
                        opacity: model.bought ? 0.5 : 1
                        border.color: "black"
                        border.width: model.selected ? 2 : 0

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

                            onPressAndHold: {
                                model.selected = !model.selected
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
                    anchors.fill: parent
                    anchors.margins: 10

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Hinzufügen");
                        onClicked: {
                            newArticleDialog.visible = true;
                        }
                    }

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Löschen");
                        onClicked: {
                            lv.model.removeSelectedItems();
                        }
                    }

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Speichern");
                        onClicked: lv.model.saveToDevice();
                    }

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Neue Einkaufsliste");
                    }
                }
            }
        }
    }

    Dialog {
        id: newArticleDialog
        visible: false
        title: qsTr("Neuen Artikel hinzufügen:")
        contentItem: Rectangle {
            color: "lightgrey"

            implicitHeight: rootRect.height * 0.5
            implicitWidth: rootRect.width * 0.5

            GridLayout {
                anchors.fill: parent
                anchors.margins: 10
                columns: newArticleDialog.width > 400 ? 2 : 1

                Text {
                    text: qsTr("Name:")
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: article.implicitHeight * 2
                    color: "white"
                    border.color: "black"
                    border.width: 1
                    TextInput {
                        id: article
                        anchors.fill: parent
                        anchors.margins: 5
                    }
                }

                Text {
                    text: qsTr("Anzahl:")
                    Layout.preferredHeight: implicitHeight
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: count.implicitHeight * 2
                    color: "white"
                    border.color: "black"
                    border.width: 1
                    TextInput {
                        id: count
                        anchors.fill: parent
                        anchors.margins: 5
                        validator: DoubleValidator { }
                    }
                }

                Text {
                    text: qsTr("Nähere Informationen:")
                    Layout.preferredHeight: implicitHeight
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: infos.implicitHeight * 2
                    color: "white"
                    border.color: "black"
                    border.width: 1
                    TextInput {
                        id: infos
                        anchors.fill: parent
                        anchors.margins: 5
                    }
                }

                MyButton {
                    text: "Hinzufügen"
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                    Layout.columnSpan: newArticleDialog.width > 800 ? 2 : 1
                    color: "white"
                    onClicked: {
                        lv.model.insertArticle(article.text,
                                               infos.text,
                                               parseInt(count.text));
                        newArticleDialog.visible = false
                    }
                }
            }
        }
    }
}
