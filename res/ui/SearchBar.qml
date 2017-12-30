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

import Movies.MovieLibrary 1.0

import QtQml 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id: searchBar
    width: 350
    height: 30

    property string text: ""
    property string placeholderText: "Search"
    property font font: Qt.font({family: "Arial", pixelSize: 14})
    property color textColor: "#fdc500"
    property color backgroundColor: "#ffffff"
    property int iconSize: 16
    property double backgroundOpacity: 0.2
    property int radius: 4
    property int padding: 5

    signal inputTextChanged(string inputText, int filter)

    Rectangle {
        id: backgroundLayer
        opacity: searchBar.backgroundOpacity
        color: searchBar.backgroundColor
        radius: searchBar.radius
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        cursorShape: Qt.IBeamCursor
        onClicked: textInput.forceActiveFocus()
        anchors.fill: parent
    }

    RowLayout {
        id: rowLayout
        spacing: searchBar.padding * 2
        anchors.topMargin: searchBar.padding
        anchors.bottomMargin: anchors.topMargin
        anchors.rightMargin: searchBar.padding * 2
        anchors.leftMargin: anchors.rightMargin
        anchors.fill: parent

        Image {
            id: searchIcon
            width: searchBar.iconSize
            height: width
            source: "qrc:/images/search.png"
            sourceSize: Qt.size(width, width)
        }

        TextInput {
            id: textInput
            text: searchBar.text
            color: searchBar.textColor
            selectedTextColor: searchBar.textColor
            selectionColor: Qt.darker(searchBar.textColor, 2.5);
            selectByMouse: true;
            clip: true;
            font: searchBar.font
            onTextChanged: {
                var explicityFilter = true

                for (var i = 0; i < movieLibrary.filterCount(); i++) {
                    var tag = movieLibrary.filterName(i) + ':'

                    if (text.startsWith(tag)) {
                        searchBar.inputTextChanged(text.substring(tag.length), i)
                        explicityFilter = false

                        break
                    }
                }

                if (explicityFilter)
                    searchBar.inputTextChanged(text, MovieLibrary.Title)
            }
            Layout.fillWidth: true
            Keys.onReturnPressed: backgroundLayer.forceActiveFocus()

            Text {
                id: placeholder
                visible: parent.text.length == 0
                text: searchBar.placeholderText
                color: parent.color
                horizontalAlignment: Text.AlignHCenter
                font: parent.font
                anchors.fill: parent
            }
        }

        Image {
            id: deleteIcon
            width: searchBar.iconSize
            height: width
            opacity: textInput.text.length == 0 ? 0 : 1.0
            source: "qrc:/images/delete.png"
            sourceSize: Qt.size(width, width)

            MouseArea {
                id: iconMouseArea
                visible: textInput.text.length != 0
                onClicked: textInput.text = ""
                anchors.fill: parent
            }
        }
    }
}