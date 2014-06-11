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
    width: upper.width>lower.width? upper.width : lower.width
    
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
        font.pointSize: 13
        anchors.top: parent.top; anchors.left: parent.left
        text: "upper"
    }
    Text {
        id: lower
        anchors.left: upper.left
        anchors.top: upper.bottom; anchors.topMargin: 5
        font.family: defaultFont.name
        font.pointSize: 13
        text: "lower"
    }
    
}