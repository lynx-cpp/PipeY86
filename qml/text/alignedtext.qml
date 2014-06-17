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
    FontLoader {
        id: defaultFont
        source: "/default_font.ttf"
    }
    
    property var color: "black"
    property alias textA: upper.text
    property alias textB: lower.text
    property alias fontA: upper.font
    property alias fontB: lower.font
    property var leftObj: undefined
    property int leftInterval: 5
    property int offset: 10
    property int org_width: upper.width>lower.width? upper.width : lower.width
    property int org_height: upper.height + offset + lower.height
    width: org_width
    height: org_height
    
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
        font.pointSize: 12
        anchors.top: parent.top; anchors.left: parent.left
        color: parent.color
        text: "upper"
    }
    Text {
        id: lower
        anchors.left: upper.left
        anchors.top: upper.bottom; anchors.topMargin: offset
        font.family: defaultFont.name
        font.pointSize: 10
        color: parent.color
        text: "lower"
    }
    
}