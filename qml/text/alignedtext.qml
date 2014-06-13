import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    FontLoader {
        id: defaultFont
        source: "/default_font.ttf"
    }
    
    property alias textA: upper.text
    property alias textB: lower.text
    property var leftObj: undefined
    property int leftInterval: 5
    property int offset: 10
    property int org_width: upper.width>lower.width? upper.width : lower.width
    property int org_height: upper.height + offset + lower.height
    width: org_width
    height: org_height
    
    /*function setLeftObj(obj) {
        upper.anchors.left = obj.right; upper.anchors.leftMargin = 5
        upper.anchors.top: obj.top
    }*/
    anchors.left: if (leftObj!=undefined) leftObj.right;
    anchors.leftMargin: if (leftObj!=undefined) leftInterval
    anchors.top: if (leftObj!=undefined) leftObj.top;
    
    Text {
        id: upper
        font.family: defaultFont.name
        font.pointSize: 12
        anchors.top: parent.top; anchors.left: parent.left
        text: "upper"
    }
    Text {
        id: lower
        anchors.left: upper.left
        anchors.top: upper.bottom; anchors.topMargin: offset
        font.family: defaultFont.name
        font.pointSize: 10
        text: "lower"
    }
    
}