import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.0

Item {
    width: 800;
    height: 600;
    
    ListModel {
        ListElement {
            Address: "0x00"
            Data: "00"
            Stage:""
            Code: "NOP"
            color: "red"
        }
        ListElement {
            Address: "0x01"
            Data: "10"
            Stage:""
            Code: "HALT"
            color: "blue"
        }
        id: insModel
    }
    
    TableView {
        id: insTable
        anchors.left: parent.left; anchors.top: parent.top;
        TableViewColumn{ role: "Address"  ; title: "Address" ; width: 100 }
        TableViewColumn{ role: "Data" ; title: "Data" ; width: 100}
        TableViewColumn{ role: "Stage" ; title: "Stage" ; width: 50}
        TableViewColumn{ role: "Code" ; title: "Code" ; width: 200}
        width: 455; height: 500
        rowDelegate: Component {
            Rectangle {
                width: parent.width
                color:  if (styleData.row == 0)
                    "lightgreen"
                 else
                     "lightgrey"
            }
        }
        model: insModel
    }
    
    Item {
        id: container;
        //anchors.centerIn: parent;
        anchors.left: insTable.right; anchors.leftMargin: 5
        width:  rect.width  + (2 * rectShadow.radius);
        height: rect.height + (2 * rectShadow.radius);
        
        Rectangle {
            id: rect
            width: 100;
            height: 50;
            color: "lightblue";
            radius: 0;
            antialiasing: true;
            anchors.centerIn: parent;
        }
    }
    DropShadow {
        id: rectShadow;
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
