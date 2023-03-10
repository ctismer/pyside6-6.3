/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt for Python examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.12

Item {
    id: slider
    height: 26
    // default drag range is 180: divisible by 2, 3, 4, 5, 6, 9, 10, ...
    width: sliderName.width + 223 + handle.width / 2

    property real min: 0
    property real max: 1
    property real value: min + (max - min) * dragHandler.value
    property real init: min+(max-min)/2
    property string name: "Slider"
    property color color: "#0066cc"
    property real minLabelWidth: 44

    DragHandler {
        id: dragHandler
        target: handle
        xAxis.minimum: Math.round(-handle.width / 2 + 3)
        xAxis.maximum: Math.round(groove.width - handle.width / 2 - 3)
        property real value: (handle.x - xAxis.minimum) / (xAxis.maximum - xAxis.minimum)
    }

    Component.onCompleted: setValue(init)
    function setValue(v) {
        if (min < max) {
            handle.x = Math.round( v / (max - min) *
                                (dragHandler.xAxis.maximum - dragHandler.xAxis.minimum)
                                + dragHandler.xAxis.minimum);
//            console.log(name, v, "-> handle.x", handle.x, "from fraction", (v / (max - min)),
//                "of drag range", (dragHandler.xAxis.maximum - dragHandler.xAxis.minimum), "px", min, ":", max)
        }
    }
    Rectangle {
        id:sliderName
        anchors.left: parent.left
        anchors.leftMargin: 16
        height: childrenRect.height
        width: Math.max(slider.minLabelWidth, childrenRect.width)
        anchors.verticalCenter: parent.verticalCenter
        Text {
            text: slider.name + ":"
            font.pointSize: 12
            color: "#333"
        }
    }

    Rectangle {
        id: groove
        width: parent.width - 8 - sliderName.width
        color: "#eee"
        height: 7
        radius: 3
        antialiasing: true
        border.color: Qt.darker(color, 1.2)
        anchors.left: sliderName.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 24
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            height: parent.height
            anchors.left: parent.left
            anchors.right: handle.horizontalCenter
            color: slider.color
            radius: 3
            border.width: 1
            border.color: Qt.darker(color, 1.3)
            opacity: 0.8
        }
        Image {
            id: handle
            source: "images/slider_handle.png"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
