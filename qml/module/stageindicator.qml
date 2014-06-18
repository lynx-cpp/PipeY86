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
    width: indicatorRect.width + 2*rectShadow.radius
    height: indicatorRect.height + 2*rectShadow.radius
    property alias font: indicatorText.font
    property alias text: indicatorText.text
    property alias color: indicatorRect.color
    property alias fontColor: indicatorText.color
    property alias inner_width: indicatorRect.width
    property alias inner_height: indicatorRect.height
    Item{
        id: indicatorItem
        anchors.fill: parent
        //anchors.left: tableItem.right; anchors.leftMargin: 5
        //anchors.top: openButton.bottom; anchors.topMargin:3
        //width: indicatorRect.width + 2*rectShadow.radius
        //height: indicatorRect.height + 2*rectShadow.radius
        
        Rectangle {
            id: indicatorRect
            color: "yellow"
            anchors.centerIn: parent
            width: 80; height: 80
            Text{
                id: indicatorText
                anchors.centerIn: parent
                text: "D"
                color: "grey"
                font.family: defaultFont.name
                font.pointSize: 40
            }
        }
    }    
    DropShadow {
        id: rectShadow
        anchors.fill: source
        cached: true;
        horizontalOffset: 3;
        verticalOffset: 3;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: indicatorItem
    }
    
}