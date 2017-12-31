// Copyright (c) 2018, Danilo Ferreira. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import QtQml 2.0
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Item {
    id: menuButton
    width: rowLayout.width + padding * 2
    height: rowLayout.height + padding * 2
    Keys.onSpacePressed: clicked()

    property string text: "MenuButton"
    property font font: Qt.font({family: "Arial", pixelSize: 14})
    property color textColor: "#fdc500"
    property string icon: "icon.png"
    property int iconSize: 16
    property int padding: 5

    signal clicked()

    RowLayout {
        id: rowLayout
        anchors.centerIn: parent

        Image {
            id: icon
            visible: false
            width: menuButton.iconSize
            height: width
            source: menuButton.icon
            sourceSize: Qt.size(width, width)
        }

        ColorOverlay {
            id: colorLayer
            source: icon
            color: mouseArea.pressed ? Qt.darker(menuButton.textColor, 1.75) : menuButton.textColor
            anchors.fill: icon
        }

        Text {
            id: label
            text: menuButton.text
            color: colorLayer.color
            font: menuButton.font
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
    }

    MouseArea {
        id: mouseArea
        onClicked: {
            menuButton.forceActiveFocus()
            menuButton.clicked()
        }
        anchors.fill: parent
    }
}