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
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Item {
    id: stackPanel
    width: 390
    height: 420

    property font font: Qt.font({family: "Arial", pixelSize: 13})
    property color textColor: "#929292"
    property color backgroundColor: "#ffffff"
    property color progressBarColor: "#fdc500"
    property double backgroundOpacity: 0.15
    property int radius: 4
    property int spacing: 5

    property alias page: stackLayout.currentIndex
    property alias model: listView.model
    property alias delegate: listView.delegate
    property alias progress: progressBar.value
    property alias currentItem: listView.currentItem
    property alias itemCount: listView.count

    function positionAtBeginning() {
        listView.positionViewAtBeginning()
    }

    Rectangle {
        id: stackBackground
        visible: stackLayout.children[stackLayout.currentIndex].objectName !== "listView"
        opacity: stackPanel.backgroundOpacity
        color: stackPanel.backgroundColor
        radius: stackPanel.radius
        anchors.fill: parent
    }

    Rectangle {
        id: listViewMask
        visible: false
        color: "#ffffff"
        radius: stackPanel.radius
        anchors.fill: parent
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Text {
            id: errorMessage
            text: "No content available.\nPlease check your network connection."
            color: stackPanel.textColor
            font: stackPanel.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
        }

        ListView {
            id: listView
            objectName: "listView"
            clip: true
            orientation: Qt.Vertical
            spacing: stackPanel.spacing
            highlightFollowsCurrentItem: !scrollBar.active
            snapMode: ListView.SnapToItem
            onMovementEnded: updateCurrentIndex()
            onFlickStarted: forceActiveFocus()
            layer.enabled: true
            layer.effect: OpacityMask {
                id: opacityEffect
                maskSource: listViewMask
            }
            Layout.fillWidth: true
            Layout.fillHeight: true

            function updateCurrentIndex() {
                currentIndex = indexAt(contentX + width * 0.5, contentY + height * 0.5)
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                visible: listView.contentHeight > listView.height
                policy: ScrollBar.AlwaysOn
                contentItem: Rectangle {
                    id: scrollBarIndicator
                    implicitWidth: 5
                    implicitHeight: 100
                    opacity: scrollBar.pressed ? 0.5 : 0.25
                    color: stackPanel.backgroundColor
                    radius: width * 0.5
                }
                onPressedChanged: {
                    listView.updateCurrentIndex()
                    listView.forceActiveFocus()
                }
                anchors.right: listView.right
                anchors.top: listView.top
                anchors.bottom: listView.bottom
            }

            MouseArea {
                id: mouseArea
                propagateComposedEvents: true
                onClicked: {
                    listView.forceActiveFocus()
                    mouse.accepted = false
                }
                onPressed: clicked(mouse)
                anchors.fill: parent
            }
        }

        ColumnLayout {
            id: progressLayout
            anchors.centerIn: parent
            Layout.preferredWidth: parent.width * 0.8
            Layout.minimumWidth: Layout.preferredWidth
            Layout.maximumWidth: Layout.preferredWidth

            ProgressBar {
                id: progressBar
                height: 5;
                background: Rectangle {
                    id: progressBarBackground
                    width: progressBar.width
                    height: progressBar.height
                    color: Qt.darker(stackPanel.backgroundColor, 2.0)
                    radius: height * 0.5
                }
                contentItem: Item {
                    width: progressBar.width
                    height: progressBar.height

                    Rectangle {
                        id: progressBarIndicator
                        width: progressBar.visualPosition * progressBar.width
                        height: progressBar.height
                        color: stackPanel.progressBarColor
                        radius: progressBarBackground.radius
                    }
                }
                Layout.fillWidth: true
            }

            Text {
                id: progressMessage
                text: "Syncronizing..."
                color: stackPanel.textColor
                font: stackPanel.font
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }

        Text {
            id: searchMessage
            text: "No match results."
            color: stackPanel.textColor
            font: stackPanel.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
        }
    }
}