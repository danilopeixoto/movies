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
import QtGraphicalEffects 1.0

Rectangle {
    id: mainWindow
    width: 400
    height: 500
    color: "#000000"

    property double backgroundOpacity: 0.15
    property double animationDuration: 500.0
    property int padding: 5

    signal directoryChanged(string directory)

    Connections {
        id: statusConnection
        target: movieLibrary
        onStatusChanged: {
            var status = movieLibrary.getStatus()

            stackPanel.page = status

            searchBar.visible = status !== MovieLibrary.Syncronizing
            menuBar.visible = searchBar.visible

            blurLayer.opacity = status === MovieLibrary.Available ? mainWindow.backgroundOpacity : 0
        }
    }

    Connections {
        id: progressConnection
        target: movieLibrary
        onProgressChanged: stackPanel.progress = movieLibrary.getProgress()
    }

    DirectoryDialog {
        id: directoryDialog
        onAccepted: mainWindow.directoryChanged(getDirectory())
        onRejected: mainWindow.directoryChanged(null)
    }

    MouseArea {
        id: mouseArea
        onClicked: backgroundLayer.forceActiveFocus()
        anchors.fill: parent
    }

    Image {
        id: backgroundLayer
        visible: false
        fillMode: Image.Stretch
        sourceSize: Qt.size(parent.width, parent.height)
        anchors.fill: parent
    }

    FastBlur {
        id: blurLayer
        opacity: mainWindow.backgroundOpacity
        radius: 100
        source: backgroundLayer
        anchors.fill: parent

        PropertyAnimation {
            id: fadeInTransition
            target: blurLayer
            property: "opacity"
            duration: mainWindow.animationDuration * 0.5
            to: mainWindow.backgroundOpacity
        }

        PropertyAnimation {
            id: fadeOutTransition
            target: blurLayer
            property: "opacity"
            duration: mainWindow.animationDuration * 0.5
            to: 0
            onStopped: {
                var currentItem = stackPanel.currentItem

                if (currentItem != null) {
                    backgroundLayer.source = currentItem.poster
                    fadeInTransition.running = true;
                }
            }
        }
    }

    ColumnLayout {
        id: columnLayout
        spacing: mainWindow.padding
        anchors.margins: mainWindow.padding
        anchors.fill: parent

        SearchBar {
            id: searchBar
            objectName: "searchBar"
            onInputTextChanged: stackPanel.positionAtBeginning()
            Layout.fillWidth: true
        }

        StackPanel {
            id: stackPanel
            model: movieLibrary
            delegate: MoviePanel {
                id: moviePanel
                width: stackPanel.width
                title: titleProperty
                year: yearProperty
                overview: overviewProperty
                director: directorProperty
                writers: writersProperty
                actors: actorsProperty
                genre: genreProperty
                runtime: runtimeProperty
                poster: posterProperty
                filename: filenameProperty
            }
            onCurrentItemChanged: fadeOutTransition.running = true
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        MenuBar {
            id: menuBar
            menuItems: MenuButton {
                id: importLibraryButton
                text: "Import Library"
                icon: "qrc:/images/import_library.png"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: directoryDialog.open()
            }
            Layout.fillWidth: true
        }
    }
}