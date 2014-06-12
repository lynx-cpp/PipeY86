import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: indicatorRect.width + 2*rectShadow.radius
    height: indicatorRect.height + 2*rectShadow.radius
    property alias font: indicatorText.font
    property alias text: indicatorText.text
    property alias color: indicatorRect.color
    property alias fontColor: indicatorText.color
    property alias inner_width: indicatorRect.width
    property alias inner_height: indicatorRect.height
    Item{
        id: indicatorItem
        anchors.fill: parent
        //anchors.left: tableItem.right; anchors.leftMargin: 5
        //anchors.top: openButton.bottom; anchors.topMargin:3
        //width: indicatorRect.width + 2*rectShadow.radius
        //height: indicatorRect.height + 2*rectShadow.radius
        
        Rectangle {
            id: indicatorRect
            color: "yellow"
            anchors.centerIn: parent
            width: 80; height: 80
            Text{
                id: indicatorText
                anchors.centerIn: parent
                text: "D"
                color: "grey"
                font.family: defaultFont.name
                font.pointSize: 40
            }
        }
    }    
    DropShadow {
        id: rectShadow
        anchors.fill: source
        cached: true;
        horizontalOffset: 3;
        verticalOffset: 3;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: indicatorItem
    }
    
}