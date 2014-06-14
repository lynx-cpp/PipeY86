import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../text"

Item {
    property var icode:"0"; property var ifun:"0";
    property var rA   :"8"; property var rB  :"8";
    property var dstE :"8"; property var dstM:"8";
    property var srcA :"8"; property var srcB:"8";
    
    property var valA :"0x00000000"; property var valB:"0x00000000";
    property var valC :"0x00000000"; property var valP:"0x00000000";
    property var valE :"0x00000000"; property var valM:"0x00000000";
    
    property int inner_width: 200
    property int inner_height: 60
    Item {
        id: decodeContainer;
        //anchors.centerIn: parent;
        //anchors.left: decodeIndicator.right; anchors.leftMargin: -16
        //anchors.top: decodeIndicator.top
        //anchors.top: openButton.bottom; anchors.topMargin:3
        width:  rect.width  + (2 * rectShadow.radius);
        height: rect.height + (2 * rectShadow.radius);
        
        Rectangle {
            id: rect
            width: inner_width;
            height: inner_height;
            color: "white";
            radius: 0;
            antialiasing: true;
            anchors.centerIn: parent;
            
            AlignedText {
                id: valp_text
                offset: 5
                //x: 12; y: 23
                fontA.pointSize: 14
                fontB.pointSize: 12
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left; anchors.leftMargin: 25
                textA: "PredPC"
                textB: valP
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
        source: decodeContainer;
    }
}