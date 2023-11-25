import QtQuick 2.15

Rectangle {
    property alias displayData: dataText.text
    anchors.horizontalCenter: parent.horizontalCenter
    radius: height * 0.05
    color: "white"
    border.color: "#3F0071"
    border.width: width * 0.02

    Text {
        id: dataText
        anchors.fill: parent
        font.pointSize: 18
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
