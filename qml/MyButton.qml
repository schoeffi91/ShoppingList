import QtQuick 2.0

Rectangle {
    id: myButtonRect

    color: enabled ? "white" : "grey"

    property alias text: myText.text

    signal clicked

    scale: myMouseArea.pressed ? 0.9 : 1
    implicitHeight:  myText.implicitHeight

    Text {
        id: myText
        anchors.centerIn: parent
        font.pixelSize: 20
    }

    MouseArea {
        id: myMouseArea
        anchors.fill: parent
        onClicked: myButtonRect.clicked()
    }
}
