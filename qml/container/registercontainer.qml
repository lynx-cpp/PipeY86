import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../text"

Item {
    FontLoader {
        id: defaultFont
        source: "/default_font.ttf"
    }
 
    //property var icode:"0"; property var ifun:"0";
    //property var rA   :"8"; property var rB  :"8";
    //property var dstE :"8"; property var dstM:"8";
    //property var srcA :"8"; property var srcB:"8";
    
    property var eax: "0x00000000"; property var ecx: "0x00000000";
    property var edx: "0x00000000"; property var ebx:"0x00000000";
    property var esp: "0x00000000"; property var ebp:"0x00000000";
    property var esi: "0x00000000"; property var edi:"0x00000000";
    
    property var srcA: "4"; property var srcB: "7";
    
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
        property var normalColor: "black"
        property var highlightColor: "red"
        
        Rectangle {
            id: rect
            width: inner_width;
            height: inner_height;
            color: "white";
            radius: 0;
            antialiasing: true;
            anchors.centerIn: parent;
            property var interval: 25
            
            Rectangle {
                id: indicator
                anchors.left: ebx_text.right; anchors.leftMargin: 10
                anchors.top: parent.top; anchors.bottom: parent.bottom;
                anchors.right: parent.right
                color: "#ebe4c7"
                property var fontSize: 30;
                property var fontColor: "grey";
                
                Text {
                    id: r_text
                    anchors.top: parent.top; anchors.topMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "R"
                    font.family: defaultFont.name
                    font.pointSize: parent.fontSize
                    color: parent.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                Text {
                    id: e_text
                    //anchors.top: r_text.bottom; anchors.topMargin: 5
                    //anchors.horizontalCenter: parent.horizontalCenter
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -3
                    text: "e"
                    font.family: defaultFont.name
                    font.pointSize: parent.fontSize + 2
                    color: parent.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                Text {
                    id: g_text
                    anchors.bottom: parent.bottom; anchors.bottomMargin: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "g"
                    font.family: defaultFont.name
                    font.pointSize: parent.fontSize + 2
                    color: parent.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
            }
            
            property var asize: 13
            property var bsize: 11
            
            AlignedText {
                id: eax_text
                property var reg_num: "0"
                //x: 12; y: 23
                anchors.left: parent.left; anchors.leftMargin: 12
                anchors.top: parent.top; anchors.topMargin: 18
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                textA: "%eax"
                textB: eax
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }

            AlignedText {
                id: ecx_text
                property var reg_num: "1"
                textA: "%ecx"
                textB: ecx
                leftObj: eax_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: edx_text
                property var reg_num: "2"
                textA: "%edx"
                textB: edx 
                leftObj: ecx_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }

            AlignedText {
                id: ebx_text
                property var reg_num: "3"
                textA: "%ebx"
                textB: ebx
                leftObj: edx_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }

            AlignedText {
                id: esp_text
                property var reg_num: "4"
                textA: "%esp"
                textB: esp
                //anchors.top: eax_text.bottom; 
                anchors.bottom: parent.bottom; anchors.bottomMargin: 18
                anchors.left: eax_text.left;
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                //leftObj: edx_text
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: ebp_text
                property var reg_num: "5"
                textA: "%ebp"
                textB: ebp
                leftObj: esp_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: esi_text
                property var reg_num: "6"
                textA: "%esi"
                textB: esi
                leftObj: ebp_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
                leftInterval: parent.interval
            }
            
            AlignedText {
                id: edi_text
                property var reg_num: "7"
                textA: "%edi"
                textB: edi
                leftObj: esi_text
                color: if (srcA==reg_num || srcB==reg_num) container.highlightColor
                else container.normalColor;
                fontA.pointSize: parent.asize; fontB.pointSize: parent.bsize;
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