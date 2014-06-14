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
    //property string fetchColor: "#c190ea"
    property string fetchColor: "#d2b4d4"
    property string decodeColor: "#eb938e"
    property string executeColor: "#f7d75e"
    property string memoryColor: "#a6d864"
    property string writeBackColor: "#77bbdb"
    property string indicatorFontColor: "grey"
    
    signal load(string str)
    signal start(int latency)
    signal step()
    signal pause()
    signal reset()
    signal startWithoutLatency()
    signal setLatency(int latency)
    
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
    
    /*function printList(list) {
        console.log(list[0]);
        console.log(list[1]);
    }*/
    
    function getStageObj(str) {
        switch (str) {
            case "fetch": return fetch_container;
            case "decode": return decode_container;
            case "execute": return execute_container;
            case "memory": return memory_container;
            case "writeback": return writeback_container;
        }
        return undefined;
    }
    
    function writeContainer(containerName,list) {
        var object = getStageObj(containerName);
        //console.log(list[0]);
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
                        property string type: (insModel.get(styleData.row)==undefined) ? "" : insModel.get(styleData.row).Stage
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
                    anchors.leftMargin: 1
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
                    anchors.leftMargin: 1
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
        anchors.left: openButton.left; //anchors.leftMargin: 5
        anchors.top: openButton.bottom; //anchors.topMargin: -12
        button_width: 80
        button_height: 40
        onClicked: reset()
        font.family: defaultFont.name
        font.pointSize:18
        text: "Reset"
    }    
    
    Item {
        id: freqSelector
        anchors.left: openButton.right; //anchors.leftMargin: -5
        anchors.top: openButton.top; anchors.topMargin: 0
        width: freqSelectorRect.width + 2*selectorShadow.radius
        height: freqSelectorRect.height + 2*selectorShadow.radius
        property int freq: 200;
        Rectangle{
            id: freqSelectorRect
            anchors.centerIn: parent
            //anchors.left: openButton.right; //anchors.leftMargin: -5
            //anchors.top: openButton.top; anchors.topMargin: 6
            //anchors.bottom: openButton.bottom; anchors.bottomMargin: -8
            color: "white"
            //radius: 5
            //border.width: 1
            //border.color: "black"
            //button_width: 160
            //button_height: 40
            width: 160
            height: 50
            //font.family: defaultFont.name
            //font.pointSize:18
            //text: "5Hz       20Hz"
            Image {
                id: circle
                height: 50
                width: 50
                source: "/circle.png"
                anchors.left: f5hz.left;  anchors.leftMargin: -23
            }
            
            Text {
                id: f5hz
                text: "5Hz"
                anchors.left: parent.left; anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter; //anchors.topMargin: 10
                font.family: defaultFont.name
                font.pointSize:18
            }
            
            MouseArea {
                anchors.fill: f5hz
                onClicked: {
                    freqSelector.freq = 200;
                    circle.anchors.left = f5hz.left;  
                    setLatency(200);
                }
            }
            
            Text {
                id: f20hz
                text: "20Hz"
                anchors.right: parent.right; anchors.rightMargin: 20
                anchors.top: f5hz.top
                font.family: defaultFont.name
                font.pointSize:18
            }
            
            MouseArea {
                anchors.fill: f20hz
                onClicked: {
                    freqSelector.freq = 50;
                    circle.anchors.left = f20hz.left;  
                    setLatency(50);
                }
            }
        }
    }
    
    DropShadow {
        id: selectorShadow
        anchors.fill: source
        cached: true;
        horizontalOffset: 3;
        verticalOffset: 3;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: freqSelector;
    }
    
    Button {
        id: startButton
        //anchors.left: resetButton.right; //anchors.leftMargin: -5
        //anchors.horizontalCenter: freqSelector.horizontalCenter
        anchors.top: freqSelector.bottom; anchors.topMargin: -16
        //anchors.bottom: resetButton.bottom; anchors.bottomMargin: 8
        anchors.left: freqSelector.left; 
        button_width: freqSelectorRect.width
        button_height: 46
        onClicked: startButtonClicked()
        font.family: defaultFont.name
        font.pointSize:18
        text: paused ? "Start" : "Pause"
        
        property var paused: true;
    }
   
    function startButtonClicked() {
        if (startButton.paused){
            main.start(freqSelector.freq);
            startButton.paused = false;
        }
        else {
            main.pause();
            startButton.paused = true;
        }
    }
    
     Button {
        id: backButton
        anchors.left: freqSelector.right; //anchors.leftMargin: -5
        anchors.top: openButton.top
        button_width: 80
        button_height: 40
        //onClicked: 
        font.family: defaultFont.name
        font.pointSize:18
        text: "Back"
    }
    
    Button {
        id: continueButton
        anchors.left: backButton.right; //anchors.leftMargin: -5
        anchors.top: backButton.top
        button_width: 80
        button_height: 40
        //onClicked: 
        font.family: defaultFont.name
        font.pointSize:13
        text: "Continue"
    }
        
    Button {
        id: stepButton
        anchors.left: freqSelector.right; //anchors.leftMargin: -5
        anchors.top: resetButton.top
        button_width: 80
        button_height: 40
        onClicked: step()
        font.family: defaultFont.name
        font.pointSize:18
        text: "Step"
    }
    
    Action {
        id: stepAction
        text: "Step"
        onTriggered: step()
        shortcut: "F7"
        tooltip: "Step into"
    }
    
    StageIndicator {
        id: fetchIndicator
        anchors.left: tableItem.right; anchors.leftMargin: 5
        anchors.top: resetButton.bottom; anchors.topMargin: 5
        color: fetchColor;
        fontColor: indicatorFontColor
        inner_width: 100
        inner_height: 60
        font.pointSize: 23
        text: "Fetch"
    }
    
    StageIndicator {
        id: decodeIndicator
        anchors.left: fetchIndicator.left; 
        anchors.top: fetchIndicator.bottom;
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
    
    FetchContainer {
        id: fetch_container;
        property var indicator: fetchIndicator
        anchors.left: indicator.right; anchors.leftMargin: -16
        anchors.top: indicator.top
        inner_height: indicator.inner_height
        inner_width: 200
    }
    
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
