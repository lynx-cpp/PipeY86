import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.0

Item {
    width: 800;
    height: 600;
    Rectangle {
        id: background
        anchors.fill: parent
        color: "#adbcc1"
    }
    
    ListModel {
        id: insModel
        ListElement {
            Address: "0x00"
            Data: "00"
            Stage: "F"
            Code: "NOP"
        }
        ListElement {
            Address: "0x01"
            Data: "10"
            Stage: "D"
            Code: "HALT"
        }
    }
    
    function removeStageLabel(idx){
        insModel.setProperty(idx,"Stage","");
    }
    
    function setStageLabel(idx,label){
        insModel.setProperty(idx,"Stage",label);
    }
    
    function addElement(addr,data,stage,code){
        insModel.append({"Address":addr,"Data":data,"Stage":stage,"Code":code});
    }
    
    Item {
        id:tableItem
        width: 455 + tableShadow.radius; height: 500 + tableShadow.radius
        anchors.left: parent.left; anchors.top: parent.top;
        
        TableView {
            id: insTable
            anchors.left: parent.left; anchors.top: parent.top;
            //anchors.fill: parent
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 100 }
            TableViewColumn{ role: "Data" ; title: "Data" ; width: 100}
            TableViewColumn{ role: "Stage" ; title: "Stage" ; width: 50}
            TableViewColumn{ role: "Code" ; title: "Code" ; width: 200}
            width: 455; height: 500
            rowDelegate: Component {
                Rectangle {
                    width: parent.width
                    property string type: insModel.get(styleData.row).Stage
                    color: if (styleData.row==undefined)
                    "lightgrey"
                    else if (type=="F") 
                        "lightgreen"
                        else if (type=="D")
                            "yellow"
                            else
                                "lightblue"
                }
                //color: insModel.get(styleData.row).color
            }
            model: insModel
        }
    }
    DropShadow {
        id: tableShadow
        anchors.fill: source
        cached: true;
        horizontalOffset: 3;
        verticalOffset: 3;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: tableItem;
    }
    
    
    Item {
        id: container;
        //anchors.centerIn: parent;
        anchors.left: tableItem.right; anchors.leftMargin: 5
        width:  rect.width  + (2 * rectShadow.radius);
        height: rect.height + (2 * rectShadow.radius);
        
        Rectangle {
            id: rect
            width: 100;
            height: 50;
            color: "white";
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
