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

Item {
    id: button
    width:  button_width  + (2 * radius);
    height: button_height + (2 * radius);
    property real radius: 8.0
    //property string color: "#e6e4d5"
    //property string color: "#f5e5a7"
    property string color: "#ebe4c7"
    //property string enter_color: "#dafffe"
    //property string enter_color: "#e0dabf"
    property string enter_color: "#ebe4c7"
    property int offset: 3
    property int button_width: 100
    property int button_height: 50
    //property string enter_color: "#f3fffe"
    property alias text: centerText.text
    property alias font: centerText.font
    
    
    //property alias buttonWidth: button_width
    //property alias buttonHeight: button_height
    
    signal clicked()
    
    Item {
        id: container;
        anchors.fill: parent
        clip: true
        
        Rectangle {
            id: rect
            width: button.button_width;
            height: button.button_height;
            color: button.color
            property color dest_color: color;
            gradient: Gradient{
                GradientStop { position: 0.7; color: rect.color}
                GradientStop { position: 0.0; color: rect.dest_color}
            }
            radius: 0;
            antialiasing: true;
            //anchors.centerIn: parent;
            x: button.radius; y: button.radius
        }
        Text {
            id: centerText
            anchors.centerIn: rect
            antialiasing: true
        }
        
        Item {
            id: shadowContainer
            width: button.button_width;
            height: button.button_height;
            x: button.radius; y: button.radius
        }
        
        MouseArea {
            anchors.fill: container
            hoverEnabled: true
            onClicked: button.clicked();
            onEntered: 
            { 
                rect.color = button.enter_color 
                rect.dest_color = "white";
            }
            onExited: { 
                rect.color = button.color 
                rect.dest_color = button.color;
            }
            onPressed: { 
                rect.x = rectShadow.radius + button.offset - 1; rect.y = rectShadow.radius + button.offset - 1
                rectShadow.horizontalOffset = 0;
                rectShadow.verticalOffset = 0;
            }
            onReleased: {
                rect.x = rectShadow.radius; rect.y = rectShadow.radius
                rectShadow.horizontalOffset = button.offset;
                rectShadow.verticalOffset = button.offset;
            }
        }
    }
    DropShadow {
        id: rectShadow;
        anchors.fill: container
        cached: true;
        horizontalOffset: button.offset;
        verticalOffset: button.offset;
        radius: button.radius;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: container;
    }
}    