import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
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
    
    function clearInsTable(){
        insModel.clear()
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
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 60 }
            TableViewColumn{ role: "Data" ; title: "Data" ; width: 120}
            TableViewColumn{ role: "Stage" ; title: "Stage" ; width: 50}
            TableViewColumn{ role: "Code" ; title: "Code" ; width: 220}
            width: 470; height: 520
            headerVisible: true
            style: TableViewStyle {
                highlightedTextColor: "grey"
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
                itemDelegate: Item {
                    Text {
                        font.family: defaultFont.name
                        anchors.verticalCenter: parent.verticalCenter
                        color: styleData.textColor
                        elide: styleData.elideMode
                        text: styleData.value
                        //anchors.centerIn: parent
                    }
                }
                headerDelegate : Component {
                    //source: "../fetch.png" //in examples/quick/controls/tableview/images
                    //border{left:2;right:2;top:2;bottom:2}
                            Rectangle {
                                id: hh
                                height: 20
                                color: "lightblue"
                                anchors.left: parent.left; anchors.top: parent.top
                                Text {//this is how to add text on top of image, will be "name", "age", "gender"
                                    color:"#333"
                                    text: styleData.value
                                    font.family: defaultFont.name
                                    font.pointSize: 10
                                    anchors.left: parent.left
                                }
                            }
                        /*
                        DropShadow {
                            id: headerShadow
                            anchors.fill: source
                            cached: true;
                            horizontalOffset: 3;
                            verticalOffset: 3;
                            radius: 8.0;
                            samples: 16;
                            color: "#80000000";
                            smooth: true;
                            source: header;
                        }*/
                }
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
    
    ListModel {
        id: memoryModel
    }
    
    Item {
        id: memItem
        width: memView.width + memShadow.radius; height: memView.height + memShadow.radius
        anchors.left: parent.left; anchors.top: tableItem.bottom;
        
        TableView {
            id: memView
            anchors.left: parent.left; anchors.top: parent.top;
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 150}
            TableViewColumn{ role: "Value"  ; title: "Value" ; width: 310}
            width: 470; height: 200;
            style: TableViewStyle {
                itemDelegate: Item {
                    Text {
                        font.family: defaultFont.name
                        anchors.verticalCenter: parent.verticalCenter
                        color: styleData.textColor
                        elide: styleData.elideMode
                        text: styleData.value
                        //anchors.centerIn: parent
                    }
                }
                headerDelegate : Component {
                    Rectangle {
                        id: hh
                        height: 20
                        color: "lightblue"
                        anchors.left: parent.left; anchors.top: parent.top
                        Text {//this is how to add text on top of image, will be "name", "age", "gender"
                            color:"#333"
                            text: styleData.value
                            font.family: defaultFont.name
                            font.pointSize: 10
                            anchors.left: parent.left
                        }
                    }
                }
                    
            }
        }
    }
    DropShadow {
        id: memShadow
        anchors.fill: source
        cached: true;
        horizontalOffset: 3;
        verticalOffset: 3;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: memItem;
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
        font.pointSize:18
        text: "Open"
    }
    
    Button {
        id: resetButton
        anchors.left: openButton.right; anchors.leftMargin: 5
        button_width: 80
        button_height: 40 
        onClicked: reset()
        font.family: defaultFont.name
        font.pointSize:18
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
            width: 460;
            height: 80;
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
