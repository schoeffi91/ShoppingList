import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import "qrc:/"

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
            anchors.margins: 10

            ListView {
                id: lv
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ShoppingModel
                clip: true
                spacing: 10
                currentIndex: -1
                delegate: Rectangle {
                    id: rootArticleItem
                    property bool detailedView: false
                    width: rootRect.width
                    height: layout.implicitHeight
                    color: model.selected ? "lightblue" : "white"
                    opacity: model.bought ? 0.5 : 1
                    border.color: "black"
                    border.width: model.selected ? 2 : 0

                    ColumnLayout {
                        id: layout
                        width: parent.width
                        spacing: 0

                        Item {
                            Layout.preferredHeight: myText.implicitHeight + 10
                            Layout.fillWidth: true
                            Text {
                                id: myText
                                text: model.article
                                anchors.fill: parent
                                anchors.margins: 10
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                text: model.count
                                anchors.right: parent.right
                                anchors.rightMargin: 30
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                verticalAlignment: Text.AlignVCenter
                                anchors.leftMargin: 10
                            }

                            Rectangle {
                                width: parent.width - 10
                                height: 1
                                color: myText.color
                                anchors.centerIn: parent
                                visible: model.bought
                            }
                        }

                        Item {
                            Layout.preferredHeight: infoText.implicitHeight + 10
                            Layout.fillWidth: true
                            visible: rootArticleItem.detailedView
                            Text {
                                id: infoText
                                anchors.fill: parent
                                anchors.margins: 10
                                verticalAlignment: Text.AlignVCenter
                                text: model.infotext
                                wrapMode: Text.Wrap
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            detailedView = !detailedView;
                        }

                        onDoubleClicked: {
                            model.bought = !model.bought;
                        }

                        onPressAndHold: {
                            model.selected = !model.selected
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
                        imageSource: "qrc:/addItem.png"
                        onClicked: {
                            newArticleDialog.visible = true;

                            article.text = "";
                            count.text = "";
                            infos.text = "";
                            article.focus = true
                        }
                    }

                    MyButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: qsTr("Löschen");
                        onClicked: lv.model.removeSelectedItems();

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
                        onClicked: lv.model.removeAllItems();
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
                        Keys.onReturnPressed: {
                            article.focus = false
                            count.focus = true;
                        }
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
                        inputMethodHints: Qt.ImhDigitsOnly
                        Keys.onReturnPressed: {
                            count.focus = false
                            infos.focus = true;
                        }
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
                    Layout.fillHeight: true
                    Layout.maximumHeight: 40
                    Layout.columnSpan: newArticleDialog.width > 800 ? 2 : 1
                    color: "white"
                    imageSource: "qrc:/addItem.png"
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
