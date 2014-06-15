import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../text"

Item {
    //property var icode:"0"; property var ifun:"0";
    //property var rA   :"8"; property var rB  :"8";
    //property var dstE :"8"; property var dstM:"8";
    //property var srcA :"8"; property var srcB:"8";
    
    property var eax: "0x00000000"; property var ecx: "0x00000000";
    property var edx: "0x00000000"; property var ebx:"0x00000000";
    property var esp: "0x00000000"; property var ebp:"0x00000000";
    property var esi: "0x00000000"; property var edi:"0x00000000";
    
    property int inner_width: 430
    property int inner_height: 80
    Item {
        id: container;
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
            property var interval: 30
            
            AlignedText {
                id: eax_text
                //x: 12; y: 23
                anchors.left: parent.left; anchors.leftMargin: 12
                anchors.top: parent.top; anchors.topMargin: 18
                textA: "%eax"
                textB: eax
                leftInterval: parent.interval
            }

            AlignedText {
                id: ecx_text
                textA: "%ecx"
                textB: ecx
                leftObj: eax_text
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: edx_text
                textA: "%edx"
                textB: edx 
                leftObj: ecx_text
                leftInterval: parent.interval
            }

            AlignedText {
                id: ebx_text
                textA: "%ebx"
                textB: ebx
                leftObj: edx_text
                leftInterval: parent.interval
            }

            AlignedText {
                id: esp_text
                textA: "%esp"
                textB: esp
                //anchors.top: eax_text.bottom; 
                anchors.bottom: parent.bottom; anchors.bottomMargin: 18
                anchors.left: eax_text.left;
                //leftObj: edx_text
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: ebp_text
                textA: "%ebp"
                textB: ebp
                leftObj: esp_text
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: esi_text
                textA: "%esi"
                textB: esi
                leftObj: ebp_text
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: edi_text
                textA: "%edi"
                textB: edi
                leftObj: esi_text
                leftInterval: parent.interval
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
        source: container;
    }
}