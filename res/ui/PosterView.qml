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
import QtGraphicalEffects 1.0

Item {
    id: posterView
    width: 150
    height: 225

    property string poster: "poster_image.png"
    property int radius: 4
    property int iconSize: 32

    signal clicked()

    Image {
        id: poster
        visible: false
        width: parent.width
        height: parent.height
        source: posterView.poster
        sourceSize: Qt.size(width, height);
        anchors.fill: parent
    }

    Rectangle {
        id: opacityMask
        visible: false
        color: "#ffffff"
        radius: posterView.radius
        anchors.fill: parent
    }

    Rectangle {
        id: blurMask
        visible: false
        color: "#00000000"
        anchors.fill: parent

        Rectangle {
            id: circle
            width: posterView.iconSize
            height: posterView.iconSize
            color: "#ffffff"
            radius: width * 0.5
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 5
        }
    }

    OpacityMask {
        id: opacityLayer
        source: poster
        maskSource: opacityMask
        anchors.fill: parent
    }

    MaskedBlur {
        id: blurLayer
        source: opacityLayer
        maskSource: blurMask
        radius: 16
        samples: 24
        anchors.fill: parent
    }

    Image {
        id: playButton
        width: posterView.iconSize
        height: width
        opacity: mouseArea.pressed ? 0.75 : 0.5;
        source: "qrc:/images/play.png"
        sourceSize: Qt.size(width, width)
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        MouseArea {
            id: mouseArea
            onClicked: {
                posterView.forceActiveFocus()
                posterView.clicked()
            }
            anchors.fill: parent
        }
    }
}