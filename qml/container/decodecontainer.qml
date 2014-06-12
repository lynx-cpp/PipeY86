import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../text"

Item {
    property var icode:"0"; property var ifun:"0";
    property var rA   :"8"; property var rB  :"8";
    property var dstE :"8"; property var dstM:"8";
    property var srcE :"8"; property var srcM:"8";
    
    property var valA :"0x00000000"; property var valB:"0x00000000";
    property var valC :"0x00000000"; property var valP:"0x00000000";
    property var valE :"0x00000000"; property var valM:"0x00000000";
    
    property int inner_width: 430
    property int inner_height: 80
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
            
            /*
            Image {
                source: "/fetch.png"
                anchors.fill: parent
            }
            */
            
            AlignedText {
                id: icode_text
                //x: 12; y: 23
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left; anchors.leftMargin: 12
                textA: "icode"
                textB: icode
            }

            AlignedText {
                id: ifun_text
                //width: icode_text.width
                textA: "ifun"
                textB: ifun
                leftObj : icode_text
            }

            AlignedText {
                id: ra_text
                width: valp_text.width
                leftObj: ifun_text
                textA: "rA"
                textB: rA
            }

            AlignedText {
                id: rb_text
                width: valp_text.width
                textA: "rB"
                textB: rB
                leftObj: ra_text
            }
            
            AlignedText {
                id: valc_text
                textA: "valC"
                textB: valC
                leftObj: rb_text
            }
            
            AlignedText {
                id: valp_text
                textA: "valP"
                textB: valP
                leftObj: valc_text
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