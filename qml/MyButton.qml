import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: myButtonRect

    color: enabled ? "white" : "grey"

    property alias text: myText.text
    property alias imageSource: myImage.source

    signal clicked

    radius: 5

    scale: myMouseArea.pressed ? 0.9 : 1
    implicitHeight:  myText.implicitHeight

    RowLayout {
        anchors.centerIn: parent

        Image {
            id: myImage
            Layout.preferredHeight: myText.implicitHeight
            Layout.preferredWidth: implicitWidth
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: myText
            Layout.fillHeight: true
            Layout.fillWidth: true
            font.pixelSize: 20
        }
    }

    MouseArea {
        id: myMouseArea
        anchors.fill: parent
        onClicked: myButtonRect.clicked()
    }
}
