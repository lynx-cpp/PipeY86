import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import "module"

Item {
    id: main
    width: 1024;
    height: 768;
    FontLoader {
        id: defaultFont
        source: "../default_font.ttf"
    }
    
    signal load(string str)
    signal start(int latency)
    signal pause()
    signal reset()
    signal startWithoutLatency()
    
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
        width: insTable.width + tableShadow.radius; height: insTable.height + tableShadow.radius
        anchors.left: parent.left; anchors.top: parent.top;
        
        TableView {
            id: insTable
            anchors.left: parent.left; anchors.top: parent.top;
            //anchors.fill: parent
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 80 }
            TableViewColumn{ role: "Data" ; title: "Data" ; width: 120}
            TableViewColumn{ role: "Stage" ; title: "Stage" ; width: 50}
            TableViewColumn{ role: "Code" ; title: "Code" ; width: 200}
            width: 455; height: 520
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
                     else if (styleData.alternate)
                         "lightblue"
                     else
                         "lightyellow"
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
    
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            load(fileDialog.fileUrls)
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        //Component.onCompleted: visible = false
    }
    
    Button {
        id: openButton
        anchors.left: tableItem.right; anchors.leftMargin: 5
        button_width: 80
        button_height: 40 
        onClicked: fileDialog.open()
        font.family: defaultFont.name
        font.pointSize:20
        text: "Open"
    }
    
    Button {
        id: resetButton
        anchors.left: openButton.right; anchors.leftMargin: 5
        button_width: 80
        button_height: 40 
        onClicked: reset()
        font.family: defaultFont.name
        font.pointSize:20
        text: "Reset"
    }
    
    
    Item {
        id: container;
        //anchors.centerIn: parent;
        anchors.left: tableItem.right; anchors.leftMargin: 5
        anchors.top: openButton.bottom; anchors.topMargin:3
        width:  rect.width  + (2 * rectShadow.radius);
        height: rect.height + (2 * rectShadow.radius);
        
        Rectangle {
            id: rect
            width: 540;
            height: 100;
            color: "white";
            radius: 0;
            antialiasing: true;
            anchors.centerIn: parent;
            
            Image {
                source: "../fetch.png"
                anchors.fill: parent
            }
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
