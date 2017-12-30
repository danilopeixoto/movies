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
import QtQuick.Layouts 1.3

Item {
    id: moviePanel
    width: 350
    height: columnLayout.height + 2 * padding

    property font font: Qt.font({family: "Arial", pixelSize: 14})
    property font secondaryFont: Qt.font({family: "Arial", pixelSize: 13})
    property color textColor: "#ffffff"
    property color secondaryTextColor: Qt.darker(textColor, 1.75)
    property color backgroundColor: "#ffffff"
    property double backgroundOpacity: 0.15
    property int radius: 4
    property int padding: 5
    property size posterSize: Qt.size(150, 225)

    property alias title: titleContent.text
    property alias year: yearContent.text
    property alias overview: overviewContent.text
    property alias director: directorContent.text
    property alias writers: writersContent.text
    property alias actors: actorsContent.text
    property alias genre: genreContent.text
    property alias runtime: runtimeContent.text
    property alias poster: posterView.poster
    property string filename: "movie.mp4"

    Rectangle {
        id: backgroundLayer
        opacity: moviePanel.backgroundOpacity
        color: moviePanel.backgroundColor
        radius: moviePanel.radius
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: moviePanel.padding

        GridLayout {
            id: headerLayout
            rows: 2
            columns: 3

            PosterView {
                id: posterView
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: moviePanel.posterSize.width
                Layout.maximumHeight: moviePanel.posterSize.height
                Layout.rowSpan: 2
                onClicked: Qt.openUrlExternally(moviePanel.filename)
            }

            Text {
                id: titleContent
                color: moviePanel.textColor
                font: moviePanel.font
                elide: Text.ElideRight
                Layout.maximumWidth: overviewContent.width * 0.8
            }

            Text {
                id: yearContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                id: overviewContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                horizontalAlignment: Text.AlignJustify
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                Layout.columnSpan: 2
                Layout.maximumHeight: posterView.height - titleContent.height - 5
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        Rectangle {
            id: horizontalLine
            height: 1
            opacity: moviePanel.backgroundOpacity * 0.5
            color: moviePanel.backgroundColor
            Layout.fillWidth: true
        }

        GridLayout {
            id: detailsLayout
            rows: 6
            columns: 2

            Text {
                id: detailsLabel
                text: "Details"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Item {
                id: horizontalSpacer
                Layout.fillWidth: true
            }

            Text {
                id: directorLabel
                text: "Director"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Text {
                id: directorContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                id: writersLabel
                text: "Writers"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Text {
                id: writersContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                id: actorsLabel
                text: "Actors"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Text {
                id: actorsContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                id: genreLabel
                text: "Genre"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Text {
                id: genreContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                id: runtimeLabel
                text: "Runtime"
                color: moviePanel.textColor
                font: moviePanel.font
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Text {
                id: runtimeContent
                color: moviePanel.secondaryTextColor
                font: moviePanel.secondaryFont
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }
    }
}