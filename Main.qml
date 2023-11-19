import QtQuick
import QtQuick.Window
import QtQuick.VirtualKeyboard
import QtQuick.Controls

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Rectangle {
        id: root
        anchors.fill: parent
        color: "lightblue"

        Text {
            id: title

            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.075

            text: "Coletor de Dados"
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Column {
            id: dataColumn

            anchors {
                top: title.bottom
                topMargin: parent.height * 0.025
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.75

            spacing: height * 0.05

            Repeater {
                model: ["x", "y", "z"]
                delegate: Rectangle {
                    required property string modelData

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.3
                    height: parent.height * 0.16
                    radius: height * 0.05
                    color: "white"

                    Text {
                        anchors.fill: parent
                        text: "Aceleração " + parent.modelData + ":"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Repeater {
                model: ["latitude", "longitude"]
                delegate: Rectangle {
                    required property string modelData

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.3
                    height: parent.height * 0.16
                    radius: height * 0.05
                    color: "white"

                    Text {
                        anchors.fill: parent
                        text: modelData + ":"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.1

            Button {
                anchors.centerIn: parent
                width: parent.width * 0.3
                height: parent.height * 0.8

                text: "Iniciar jornada"
            }
        }
    }
}
