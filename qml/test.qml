import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.0
import "module"
import "container"

Item {
    id: main
    width: 1024;
    height: 768;
    FontLoader {
        id: defaultFont
        source: "/default_font.ttf"
    }
    property string fetchColor: "#c190ea"
    property string decodeColor: "#eb938e"
    property string executeColor: "#f7d75e"
    property string memoryColor: "#a6d864"
    property string writeBackColor: "#77bbdb"
    property string indicatorFontColor: "grey"
    
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
    
    function printList(list) {
        console.log(list[0]);
        console.log(list[1]);
    }
    
    function getStageObj(str) {
        switch (str) {
            case "decode": return decode_container;
            case "execute": return execute_container;
            case "memory": return memory_container;
            case "writeback": return writeback_container;
        }
        return undefined;
    }
    
    function writeContainer(containerName,list) {
        var object = getStageObj(containerName);
        console.log(list[0]);
        object.icode    = list[0]; object.ifun      = list[1];
        object.rA       = list[2]; object.rB        = list[3];
        object.dstE     = list[4]; object.dstM      = list[5];
        object.srcA     = list[6]; object.srcB      = list[7];
        
        object.valA     = list[8]; object.valB     = list[9];
        object.valC     = list[10]; object.valP   = list[11];
        object.valE     = list[12]; object.valM  = list[13];
    }
    
    Item {
        id:tableItem
        width: insTable.width + 2*tableShadow.radius; height: insTable.height + 2*tableShadow.radius
        anchors.left: parent.left; anchors.top: parent.top;
        
        TableView {
            id: insTable
            anchors.left: parent.left; anchors.top: parent.top;
            anchors.centerIn: parent
            //anchors.fill: parent
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 60 }
            TableViewColumn{ role: "Data" ; title: "Data" ; width: 120}
            TableViewColumn{ role: "Stage" ; title: "Stage" ; width: 50}
            TableViewColumn{ role: "Code" ; title: "Code" ; width: 220}
            width: 469; height: 520
            headerVisible: true
            style: TableViewStyle {
                backgroundColor: "white"
                highlightedTextColor: "grey"
                rowDelegate: Component {
                    id: rowCom
                    Rectangle {
                        width: parent.width
                        property string type: insModel.get(styleData.row).Stage
                        color: if (styleData.row==undefined)
                        backgroundColor
                        else if (type=="F") 
                            fetchColor
                        else if (type=="D")
                            decodeColor
                        else if (type=="E")
                            executeColor
                        else if (type=="M")
                            memoryColor
                        else if (type=="W")
                            writeBackColor
                        else if (styleData.alternate)
                            "#e5fff8"
                        else
                            "#fdffe5"
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
                    Item {
                        height: 20 
                        anchors.left: parent.left; anchors.top: parent.top
                        clip: false
                        Rectangle {
                            id: hh
                            height: 20
                            width: parent.width
                            color: "lightblue"
                            anchors.left: parent.left; anchors.top: parent.top
                            //border.width: 1
                            //border.color: "white"
                            
                            Text {
                                color:"#333"
                                text: styleData.value
                                font.family: defaultFont.name
                                font.pointSize: 10
                                anchors.left: parent.left; anchors.leftMargin: 1
                            }
                        }
                        
                    }
                    
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
        width: memView.width + 2*memShadow.radius; height: memView.height + 2*memShadow.radius
        anchors.left: parent.left; anchors.top: tableItem.bottom;
        
        TableView {
            id: memView
            //anchors.left: parent.left; anchors.top: parent.top;
            anchors.centerIn: parent
            TableViewColumn{ role: "Address"  ; title: "Address" ; width: 150}
            TableViewColumn{ role: "Value"  ; title: "Value" ; width: 310}
            width: 469; height: 180;
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
                            anchors.leftMargin: 1
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
        nameFilters: [ "Y86 Output files (*.yo)", "Y86 ASM files (*.ys)","All files (*)" ]
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
    
    StageIndicator {
        id: decodeIndicator
        anchors.left: tableItem.right; anchors.leftMargin: 5
        anchors.top: openButton.bottom; anchors.topMargin:3
        color: decodeColor
        fontColor: indicatorFontColor
        text: "D"
    }
    
    StageIndicator {
        id: executeIndicator
        anchors.left: decodeIndicator.left
        anchors.top: decodeIndicator.bottom; 
        color: executeColor
        fontColor: indicatorFontColor
        text: "E"
    }
    
    StageIndicator {
        id: memoryIndicator
        anchors.left: executeIndicator.left
        anchors.top: executeIndicator.bottom; 
        color: memoryColor
        fontColor: indicatorFontColor
        text: "M"
    }
    
    StageIndicator {
        id: writeBackIndicator
        anchors.left: memoryIndicator.left
        anchors.top: memoryIndicator.bottom; 
        color: writeBackColor
        fontColor: indicatorFontColor
        text: "W"
    }
    
    property var container_width: 430
    
    DecodeContainer {
        id: decode_container;
        property var indicator: decodeIndicator
        anchors.left: indicator.right; anchors.leftMargin: -16
        anchors.top: indicator.top
        inner_height: indicator.inner_height
        inner_width: container_width
    }    
    
    ExecuteContainer {
        id: execute_container;
        property var indicator: executeIndicator
        anchors.left: indicator.right; anchors.leftMargin: -16
        anchors.top: indicator.top
        inner_height: indicator.inner_height
        inner_width: container_width
    }
    
    MemoryContainer {
        id: memory_container
        property var indicator: memoryIndicator
        anchors.left: indicator.right; anchors.leftMargin: -16
        anchors.top: indicator.top
        inner_height: indicator.inner_height
        inner_width: container_width
    }
    
    WritebackContainer {
        id: writeback_container;
        property var indicator: writeBackIndicator
        anchors.left: indicator.right; anchors.leftMargin: -16
        anchors.top: indicator.top
        inner_height: indicator.inner_height
        inner_width: container_width
    }
    
    
    Rectangle {
        x: 475; y: 8
        height: 20
        width: 1
        color: "lightblue"
    }
    
    Rectangle {
        x: 459; y: 8
        height: 20
        width: 1
        color: "lightblue"
    }
    
}
