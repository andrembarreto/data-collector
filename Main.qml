import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material

import "components"

Window {
    id: window
    width: 1080
    height: 2340
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Rectangle {
        id: root
        anchors.fill: parent
        color: "black"

        Text {
            id: title

            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.075

            text: "Coletor de Dados"
            color: "silver"
            font.pointSize: 26
            fontSizeMode: Text.Fit
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Button {
            anchors {
                top: parent.top
                right: parent.right
            }
            background: Rectangle {
                anchors.fill: parent
                color: "black"
            }
            width: parent.width * 0.1
            height: parent.height * 0.05
            icon.source: "qrc:icons/bars-solid.svg"
            icon.color: "silver"
            onClicked: configsMenu.open()
        }

        Drawer {
            id: configsMenu
            edge: Qt.RightEdge
            height: parent.height
            width: parent.width * 0.3
            background: Rectangle {
                anchors.fill: parent
                border.color: "#150050"
                border.width: width * 0.01
                color: "black"
            }

            Column {
                anchors.fill: parent
                topPadding: height * 0.025

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height * 0.1
                    width: parent.width
                    background: Rectangle {
                        color: "transparent"
                        anchors.fill: parent
                    }
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Trajetos salvos")
                        color: "silver"
                        font.pointSize: 16
                        font.weight: Font.Medium
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                    onClicked: {
                        configsMenu.close();
                        savedTripsPopup.open();
                    }
                }
            }
        }

        Popup {
            id: savedTripsPopup
            property int chosenLogIndex

            onOpened: stack.replace(pageLogs)

            anchors.centerIn: parent
            width: parent.width * 0.8
            height: parent.height * 0.4
            background: Rectangle {
                anchors.fill: parent
                color: "black"
                border.width: width * 0.02
                border.color: "#150050"
                radius: width * 0.02
            }

            clip: true

            StackView {
                id: stack
                anchors.fill: parent
                initialItem: pageLogs
            }

            Component {
                id: pageLogs

                Page {
                    Column {
                        anchors.fill: parent
                        padding: height * 0.05
                        spacing: height * 0.05

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width
                            height: parent.height * 0.075

                            text: qsTr("Trajetos salvos")
                            color: "silver"
                            font.pointSize: 20
                            fontSizeMode: Text.Fit
                            font.weight: Font.DemiBold
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        ListView {
                            id: logs
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width
                            height: parent.height * 0.8
                            spacing: height * 0.05

                            model: mobilityData.journeyLogs
                            delegate: Rectangle {
                                color: control.pressed ? "transparent": "white"
                                anchors.horizontalCenter: parent.horizontalCenter
                                height: logs.height * 0.2
                                width: logs.width

                                Text {
                                    text: modelData
                                    anchors.fill: parent
                                    font.pointSize: 14
                                    fontSizeMode: Text.Fit
                                    color: control.pressed ? "yellow" : "black"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                MouseArea {
                                    id: control
                                    anchors.fill: parent
                                    onClicked: {
                                        savedTripsPopup.chosenLogIndex = index;
                                        stack.push(pageSend);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: pageSend

                Page {
                    Button {
                        anchors {
                            left: parent.left
                            top: parent.top
                            topMargin: height * 0.25
                            leftMargin: width * 0.25
                        }
                        width: parent.width * 0.3
                        height: parent.height * 0.2

                        Text {
                            anchors.fill: parent
                            text: qsTr("Voltar")
                            color: "silver"
                            font.pointSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: stack.pop()
                    }
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Deseja enviar o conteúdo de \n") + mobilityData.journeyLogs[savedTripsPopup.chosenLogIndex] + "?"
                        font.pointSize: 14
                        fontSizeMode: Text.Fit
                        color: "silver"
                    }
                    Button {
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                            bottomMargin: height * 0.25
                            rightMargin: width * 0.25
                        }
                        width: parent.width * 0.3
                        height: parent.height * 0.2

                        Text {
                            anchors.fill: parent
                            text: qsTr("Sim")
                            color: "silver"
                            font.pointSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            mobilityData.sendLoggedData(savedTripsPopup.chosenLogIndex);
                            savedTripsPopup.close();
                        }
                    }
                }
            }
        }

        Grid {
            id: axisReadingsGrid

            anchors {
                top: title.bottom
                topMargin: parent.height * 0.025
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.5

            columns: 2
            columnSpacing: width * 0.05

            Column {
                width: parent.width * 0.475
                height: parent.height
                spacing: height * 0.05

                Repeater {
                    model: ["x", "y", "z"]
                    delegate: DataField {
                        required property string modelData
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width * 0.8
                        height: parent.height * 0.3

                        displayData: "Aceleração " + modelData + ": " + (mobilityData.currentlyCollecting ?
                                                                         mobilityData.accelerationValues[modelData].toFixed(4) :
                                                                         "--")
                    }
                }
            }

            Column {
                width: parent.width * 0.475
                height: parent.height
                spacing: height * 0.05

                Repeater {
                    model: ["x", "y", "z"]
                    delegate: DataField {
                        required property string modelData
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width * 0.8
                        height: parent.height * 0.3

                        displayData: "Rotação " + modelData + ": " + (mobilityData.currentlyCollecting ?
                                                                      mobilityData.rotationValues[modelData].toFixed(4) :
                                                                      "--")
                    }
                }
            }
        }

        DataField {
            anchors {
                top: axisReadingsGrid.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: parent.height * 0.05
                topMargin: parent.height * 0.05
            }

            width: parent.width * 0.8
            height: parent.height * 0.1

            property double latitude: mobilityData.currentCoordinates['latitude']
            property double longitude: mobilityData.currentCoordinates['longitude']

            displayData: "Coordenada: " + (mobilityData.currentlyCollecting ? latitude + "," + longitude : "--")
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.15

            color: root.color

            Button {
                anchors.centerIn: parent
                width: parent.width * 0.4
                height: parent.height * 0.8

                background: Rectangle {
                    color: "#3F0071"
                    radius: width * 0.02
                }

                Text {
                    anchors.fill: parent
                    text: mobilityData.currentlyCollecting ? "Terminar Jornada" : "Iniciar jornada"
                    color: "silver"
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
                        setBusLinePopup.open();
                    }
                }
            }
        }
    }

    Popup {
        id: setBusLinePopup
        visible: false

        anchors.centerIn: parent
        width: parent.width * 0.75
        height: parent.height * 0.25

        background: Rectangle {
            color: "#09070d"
            radius: width * 0.05
            border.color: "#150050"
            border.width: width * 0.01
        }

        Text {
            anchors {
                top: parent.top
                topMargin: parent.height * 0.1
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: parent.height * 0.2

            text: "Qual a linha de ônibus?"
            color: "silver"
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
        }

        TextField {
            id: busLineInput
            anchors {
                top: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width * 0.4
            height: parent.height * 0.2

            font.pointSize: 10
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            placeholderText: "Digite aqui..."
            focus: true

            background: Rectangle {
                anchors.fill: parent
                color: "silver"
                radius: width * 0.02
            }
            onAccepted: mobilityData.setBusLine(busLineInput.text)
        }

        Button {
            enabled: busLineInput.text !== ""

            anchors {
                bottom: parent.bottom
                bottomMargin: parent.height * 0.05
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width * 0.5
            height: parent.height * 0.15

            background: Rectangle {
                color: "#150050"
                radius: width * 0.02
            }

            Text {
                anchors.fill: parent
                text: "Pronto"
                color: "silver"
                font.pointSize: 14
                fontSizeMode: Text.Fit
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                mobilityData.startCollecting();
                setBusLinePopup.close();
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
            color: "#09070d"
            radius: width * 0.05
            border.color: "#150050"
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
                color: "silver"
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

                background: Rectangle {
                    color: "#150050"
                    radius: width * 0.02
                }

                Text {
                    anchors.fill: parent
                    text: "Sim"
                    color: "silver"
                    font.pointSize: 14
                    fontSizeMode: Text.Fit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

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

                background: Rectangle {
                    color: "#150050"
                    radius: width * 0.02
                }

                Text {
                    anchors.fill: parent
                    text: "Não"
                    color: "silver"
                    font.pointSize: 14
                    fontSizeMode: Text.Fit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    mobilityData.discardRegisteredData();
                    collectionFinishedPopup.close();
                }
            }
        }
    }
}
