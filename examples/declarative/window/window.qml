/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

import QtQuick
import QtQuick.Controls

QtObject {
    id: root
    property real defaultSpacing: 10
    property SystemPalette palette: SystemPalette { }

    property var controlWindow: Window {
        width: col.implicitWidth + root.defaultSpacing * 2
        height: col.implicitHeight + root.defaultSpacing * 2
        color: root.palette.window
        title: "Control Window"
        Column {
            id: col
            anchors.fill: parent
            anchors.margins: root.defaultSpacing
            spacing: root.defaultSpacing
            property real cellWidth: col.width / 3 - spacing
            Label { text: "Control the second window:" }
            Grid {
                id: grid
                columns: 3
                spacing: root.defaultSpacing
                width: parent.width
                Button {
                    id: showButton
                    width: col.cellWidth
                    text: root.testWindow.visible ? "Hide" : "Show"
                    onClicked: root.testWindow.visible = !root.testWindow.visible
                }
                //! [windowedCheckbox]
                CheckBox {
                    text: "Windowed"
                    height: showButton.height
                    width: col.cellWidth
                    Binding on checked { value: root.testWindow.visibility === Window.Windowed }
                    onClicked: root.testWindow.visibility = Window.Windowed
                }
                //! [windowedCheckbox]
                CheckBox {
                    height: showButton.height
                    width: col.cellWidth
                    text: "Full Screen"
                    Binding on checked { value: root.testWindow.visibility === Window.FullScreen }
                    onClicked: root.testWindow.visibility = Window.FullScreen
                }
                Button {
                    id: autoButton
                    width: col.cellWidth
                    text: "Automatic"
                    onClicked: root.testWindow.visibility = Window.AutomaticVisibility
                }
                CheckBox {
                    height: autoButton.height
                    text: "Minimized"
                    Binding on checked { value: root.testWindow.visibility === Window.Minimized }
                    onClicked: root.testWindow.visibility = Window.Minimized
                }
                CheckBox {
                    height: autoButton.height
                    text: "Maximized"
                    Binding on checked { value: root.testWindow.visibility === Window.Maximized }
                    onClicked: root.testWindow.visibility = Window.Maximized
                }
            }
            function visibilityToString(v) {
                switch (v) {
                case Window.Windowed:
                    return "windowed";
                case Window.Minimized:
                    return "minimized";
                case Window.Maximized:
                    return "maximized";
                case Window.FullScreen:
                    return "fullscreen";
                case Window.AutomaticVisibility:
                    return "automatic";
                case Window.Hidden:
                    return "hidden";
                }
                return "unknown";
            }
            Label {
                id: visibilityLabel
                text: "second window is " + (root.testWindow.visible ? "visible" : "invisible") +
                      " and has visibility " + parent.visibilityToString(root.testWindow.visibility)
            }
            Rectangle {
                color: root.palette.text
                width: parent.width
                height: 1
            }
            CurrentScreen { }
            Rectangle {
                color: root.palette.text
                width: parent.width
                height: 1
            }
            AllScreens { width: parent.width }
        }
    }

    property var testWindow: Window {
        width: 320
        height: 240
        color: "#215400"
        title: "Test Window with color " + color
        flags: Qt.Window | Qt.WindowFullscreenButtonHint
        Rectangle {
            anchors.fill: parent
            anchors.margins: root.defaultSpacing
            Label {
                anchors.centerIn: parent
                text: "Second Window"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.testWindow.color = "#e0c31e"
            }
            Button {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: root.defaultSpacing
                text: root.testWindow.visibility === Window.FullScreen ? "exit fullscreen" : "go fullscreen"
                width: 150
                onClicked: {
                    if (root.testWindow.visibility === Window.FullScreen)
                        root.testWindow.visibility = Window.AutomaticVisibility
                    else
                        root.testWindow.visibility = Window.FullScreen
                }
            }
            Button {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: root.defaultSpacing
                text: "X"
                width: 30
                onClicked: root.testWindow.close()
            }
        }
    }

    property var splashWindow: Splash {
        onTimeout: root.controlWindow.visible = true
    }
}
