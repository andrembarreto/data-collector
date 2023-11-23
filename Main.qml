import QtQuick
import QtQuick.Window
import QtQuick.VirtualKeyboard
import QtQuick.Controls

Window {
    id: window
    width: 1080
    height: 2340
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
            font.pointSize: 26
            fontSizeMode: Text.Fit
            font.weight: Font.Bold
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
            height: parent.height * 0.7

            spacing: height * 0.05

            Repeater {
                model: ["x", "y", "z"]
                delegate: Rectangle {
                    required property string modelData

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.45
                    height: parent.height * 0.16
                    radius: height * 0.05
                    color: "white"

                    Text {
                        anchors.fill: parent
                        text: "Aceleração " + parent.modelData + ": " + (mobilityData.currentlyCollecting ?
                                                                         mobilityData.accelerationValues[modelData].toFixed(4) :
                                                                         "--")
                        font.pointSize: 18
                        fontSizeMode: Text.Fit
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
                    width: parent.width * 0.45
                    height: parent.height * 0.16
                    radius: height * 0.05
                    color: "white"

                    Text {
                        anchors.fill: parent
                        text: modelData + ": " + (mobilityData.currentlyCollecting ?
                                                  mobilityData.currentCoordinates[modelData] :
                                                  "--")
                        font.pointSize: 18
                        fontSizeMode: Text.Fit
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
            height: parent.height * 0.15

            color: "transparent"

            Button {
                anchors.centerIn: parent
                width: parent.width * 0.4
                height: parent.height * 0.8

                Text {
                    anchors.fill: parent
                    text: mobilityData.currentlyCollecting ? "Terminar Jornada" : "Iniciar jornada"
                    font.pointSize: 16
                    fontSizeMode: Text.Fit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    if(mobilityData.currentlyCollecting) {
                        mobilityData.stopCollecting();
                        collectionFinishedPopup.open();
                    }
                    else {
                        mobilityData.startCollecting();
                    }
                }
            }
        }
    }

    Popup {
        id: collectionFinishedPopup
        visible: false
        closePolicy: "NoAutoClose"

        anchors.centerIn: parent
        width: parent.width * 0.75
        height: parent.height * 0.25

        background: Rectangle {
            color: "lightgray"
            radius: width * 0.05
            border.color: "gray"
            border.width: width * 0.01
        }
        Rectangle {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.5

            color: "transparent"

            Text {
                anchors.centerIn: parent

                width: parent.width
                height: parent.height * 0.4

                text: "Concorda em enviar os dados?"
                font.pointSize: 20
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            anchors {
                top: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.2

            color: "transparent"

            Button {
                anchors {
                    right: parent.horizontalCenter
                    rightMargin: parent.width * 0.025
                }

                width: parent.width * 0.45
                height: parent.height

                text: "Sim"
                onClicked: {
                    mobilityData.sendRegisteredData();
                    collectionFinishedPopup.close();
                }
            }
            Button {
                anchors {
                    left: parent.horizontalCenter
                    leftMargin: parent.width * 0.025
                }

                width: parent.width * 0.45
                height: parent.height

                text: "Não"
                onClicked: {
                    mobilityData.discardRegisteredData();
                    collectionFinishedPopup.close();
                }
            }
        }
    }
}
