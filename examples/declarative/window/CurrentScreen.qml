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

Item {
    id: root
    width: 400
    height: propertyGrid.implicitHeight + 16

    function orientationToString(o) {
        switch (o) {
        case Qt.PrimaryOrientation:
            return "primary";
        case Qt.PortraitOrientation:
            return "portrait";
        case Qt.LandscapeOrientation:
            return "landscape";
        case Qt.InvertedPortraitOrientation:
            return "inverted portrait";
        case Qt.InvertedLandscapeOrientation:
            return "inverted landscape";
        }
        return "unknown";
    }

    Grid {
        id: propertyGrid
        columns: 2
        spacing: 8
        x: spacing
        y: spacing

        //! [screen]
        Label {
            text: "Screen \"" + Screen.name + "\":"
            font.bold: true
        }
        Item { width: 1; height: 1 } // spacer

        Label { text: "manufacturer" }
        Label { text: Screen.manufacturer ? Screen.manufacturer : "unknown" }

        Label { text: "model" }
        Label { text: Screen.model ? Screen.model : "unknown" }

        Label { text: "serial number" }
        Label { text: Screen.serialNumber ? Screen.serialNumber : "unknown" }

        Label { text: "dimensions" }
        Label { text: Screen.width + "x" + Screen.height }

        Label { text: "pixel density" }
        Label { text: Screen.pixelDensity.toFixed(2) + " dots/mm (" + (Screen.pixelDensity * 25.4).toFixed(2) + " dots/inch)" }

        Label { text: "logical pixel density" }
        Label { text: Screen.logicalPixelDensity.toFixed(2) + " dots/mm (" + (Screen.logicalPixelDensity * 25.4).toFixed(2) + " dots/inch)" }

        Label { text: "device pixel ratio" }
        Label { text: Screen.devicePixelRatio.toFixed(2) }

        Label { text: "available virtual desktop" }
        Label { text: Screen.desktopAvailableWidth + "x" + Screen.desktopAvailableHeight }

        Label { text: "position in virtual desktop" }
        Label { text: Screen.virtualX + ", " + Screen.virtualY }

        Label { text: "orientation" }
        Label { text: root.orientationToString(Screen.orientation) + " (" + Screen.orientation + ")" }

        Label { text: "primary orientation" }
        Label { text: root.orientationToString(Screen.primaryOrientation) + " (" + Screen.primaryOrientation + ")" }
        //! [screen]

        Label { text: "10mm rectangle" }
        Rectangle {
            color: "red"
            width: Screen.pixelDensity * 10
            height: width
        }
    }
}
