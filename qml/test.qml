/*
 *   Copyright (C) 2014 by Yuquan Fang<lynx.cpp@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
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
    
    /*MessageDialog {
        id: about_dialog
        title: "About"
        text: "
Infrastructure & Graphics:            \tYuquan Fang
Instruction Implementation:\tKaiqiang Song 
Art Direction:\t\t\tQi Liu"
        visible: false
    }*/
    Window {
        width: 760
        height: 632
        //x:280; y:50
        x: main.x + 250
        y: main.y + 68
        id: about_dialog
        visible: false
        flags: Qt.FramelessWindowHint | Qt.Window
        /*Text {
            text: "
Infrastructure & Graphics:            \tYuquan Fang
Instruction Implementation:\tKaiqiang Song 
Art Direction:\t\t\tQi Liu"
        }*/
        Image {
            source: "/about.png"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: about_dialog.visible = false
        }
    }
    
    signal load(string str)
    signal start(int latency)
    signal step()
    signal pause()
    signal reset()
    signal back()
    signal startWithoutLatency()
    signal setLatency(int latency)
    signal setBreakPoint(int row)
    signal unsetBreakPoint(int row)
    signal fastStart()
    
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
        insModel.append({"Address":addr,"Data":data,"Stage":stage,"Code":code,"break":false});
    }
    
    function clearMemTable(){
        memoryModel.clear();
    }
    
    function addMemoryElement(addr,data){
        memoryModel.append({"Address":addr,"Data":data});
        //console.log(addr,data)
    }
    
    function showStopDialog() {
        setStartButtonToPaused(false);
        stopDialog.visible = true;
    }
    
    function setStartButtonToPaused(flag) {
        startButton.paused = (!flag);
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
            case "register": return register_container;
        }
        return undefined;
    }
    
    function writeContainer(containerName,list) {
        var object = getStageObj(containerName);
        //console.log(list[0]);
        if (containerName=="register"){
            object.eax = list[0]; object.ecx = list[1];
            object.edx = list[2]; object.ebx = list[3];
            object.esp = list[4]; object.ebp = list[5];
            object.esi = list[6]; object.edi = list[7];
        } else {
            object.icode    = list[0]; object.ifun      = list[1];
            object.rA       = list[2]; object.rB        = list[3];
            object.dstE     = list[4]; object.dstM      = list[5];
            object.srcA     = list[6]; object.srcB      = list[7];
            
            object.valA     = list[8]; object.valB     = list[9];
            object.valC     = list[10]; object.valP   = list[11];
            object.valE     = list[12]; object.valM  = list[13];
        }
        
        if (containerName=="execute"){
            register_container.srcA = object.srcA;
            register_container.srcB = object.srcB;
            //console.log(object.srcA,object.srcB);
        }
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
            onDoubleClicked: {
                if (insModel.get(row).break==true){
                    insModel.get(row).break = false;
                    unsetBreakPoint(row);
                }
                else{
                    insModel.get(row).break = true;
                    setBreakPoint(row);
                }
            }
            style: TableViewStyle {
                backgroundColor: "white"
                //highlightedTextColor: "grey"
                rowDelegate: Component {
                    id: rowCom
                    Rectangle {
                        width: parent.width
                        property string type: (insModel.get(styleData.row)==undefined) ? "" : insModel.get(styleData.row).Stage
                        color: if (styleData.row==undefined)
                            backgroundColor
                        else if (insModel.get(styleData.row).break==true)
                            "#7f7f7f"
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
                        font.pointSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                        color: if (styleData.selected){
                            if (insModel.get(styleData.row).break==true) "white"
                                else "grey"
                        } else
                            styleData.textColor
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
            TableViewColumn{ role: "Data"  ; title: "Value" ; width: 310}
            model: memoryModel;
            width: 469; height: 190;
            style: TableViewStyle {
                itemDelegate: Item {
                    anchors.leftMargin: 3
                    Text {
                        font.family: defaultFont.name
                        anchors.verticalCenter: parent.verticalCenter
                        color: if (styleData.selected) "grey"
                        else styleData.textColor
                        elide: styleData.elideMode
                        text: styleData.value
                        //anchors.centerIn: parent
                    }
                }
                rowDelegate: Component {
                    id: memRow;
                    Rectangle {
                        width: parent.width;
                        color: if (styleData.alternate)
                            "#e5fff8"
                        else
                            "#fdffe5"
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
                            anchors.leftMargin: 3
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
    
    MessageDialog {
        id: stopDialog 
        title: "Pipeline stopped."
        text: "Y86 Pipeline Simulator stopped."
        onAccepted: {
            Qt.quit()
        }
        //Component.onCompleted: visible = true
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
    
    Action {
        id: open_action
        onTriggered: fileDialog.open()
        shortcut: "Ctrl+o"
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
    
    Action {
        id: reset_action
        onTriggered: reset()
        shortcut: "Ctrl+r"
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
                height: 30
                width: 30
                source: "/circle.png"
                anchors.left: f5hz.left;  anchors.leftMargin: -13
                //anchors.bottom: parent.bottom
                anchors.verticalCenter: parent.verticalCenter; anchors.verticalCenterOffset: 2
            }
            
            Text {
                id: f1hz
                text: "1Hz"
                anchors.left: parent.left; anchors.leftMargin: 13
                anchors.verticalCenter: parent.verticalCenter; //anchors.topMargin: 10
                font.family: defaultFont.name
                font.pointSize:15
            }
            MouseArea {
                anchors.fill: f1hz
                onClicked: {
                    freqSelector.freq = 1000;
                    circle.anchors.left = f1hz.left;  
                    setLatency(1000);
                }
            }
            
            Text {
                id: f5hz
                text: "5Hz"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -5
                anchors.top: f1hz.top
                font.family: defaultFont.name
                font.pointSize:15
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
                anchors.right: parent.right; anchors.rightMargin: 10
                anchors.top: f5hz.top
                font.family: defaultFont.name
                font.pointSize:15
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
    
    Action {
        id: pause_action
        onTriggered: pause()
        shortcut: "Ctrl+c"
    }
    
    Action {
        id: start_action
        onTriggered: start(freqSelector.freq)
        shortcut: "Shift+F9"
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
        onClicked: back()
        text: "Back"
    }
    
    Action {
        id: back_action
        onTriggered: back()
        shortcut: "F6"
    }
    
    Button {
        id: continueButton
        anchors.left: backButton.right; //anchors.leftMargin: -5
        anchors.top: backButton.top
        button_width: 80
        button_height: 40
        onClicked: fastStart()
        font.family: defaultFont.name
        font.pointSize:13
        text: "Continue"
    }
    
    Action {
        id: continue_action
        onTriggered: fastStart()
        shortcut: "F9"
    }
    
     Button {
        id: aboutButton
        anchors.left: continueButton.left; //anchors.leftMargin: -5
        anchors.top: continueButton.bottom
        button_width: 80
        button_height: 40
        onClicked: about_dialog.visible = true
        font.family: defaultFont.name
        font.pointSize:18
        text: "About"
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
    
    RegisterContainer {
        id: register_container;
        anchors.left: writeBackIndicator.left
        anchors.top: writeBackIndicator.bottom
        inner_height: 148
        inner_width: container_width + decodeIndicator.inner_width
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
