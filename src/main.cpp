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

#include <Movies.h>

#include <QtSingleApplication>
#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QSplashScreen>
#include <QSize>
#include <QUrl>
#include <QPixmap>
#include <QStandardPaths>
#include <QDir>

MOVIES_NAMESPACE_USING

int main(int argc, char ** argv) {
    QtSingleApplication application(MOVIES_ID, argc, argv);

    if (application.isRunning()) {
        application.sendMessage("Application is already running.");
        return 0;
    }

    application.setApplicationName(MOVIES_NAME);
    application.setApplicationVersion(MOVIES_VERSION);

    QPixmap image(":/images/splash_screen.png");

    QSplashScreen splashScreen(image);
    splashScreen.show();

    qmlRegisterType<MovieLibrary>("Movies.MovieLibrary", 1, 0, "MovieLibrary");

    QQuickView view;
    QObject::connect(&application, &QtSingleApplication::messageReceived,
                     &view, &QQuickView::requestActivate);

    MovieLibrary movieLibrary(MOVIES_DATABASE_KEY);
    QQmlContext * context = view.rootContext();
    context->setContextProperty("movieLibrary", &movieLibrary);

    view.setTitle(MOVIES_NAME);
    view.setSource(QUrl("qrc:/ui/MainWindow.qml"));

    const QSize & viewSize = view.size();
    view.setMinimumSize(viewSize);
    view.setMaximumSize(viewSize);

    QObject * mainWindow = view.rootObject();
    QObject::connect(mainWindow, SIGNAL(directoryChanged(QString)),
                     &movieLibrary, SLOT(import(QString)));

    QObject * searchBar = mainWindow->findChild<QObject *>("searchBar");
    QObject::connect(searchBar, SIGNAL(inputTextChanged(QString, int)),
                     &movieLibrary, SLOT(filter(QString, int)));

    QObject * stackPanel = mainWindow->findChild<QObject *>("stackPanel");
    QObject::connect(stackPanel, SIGNAL(abortSync()), &movieLibrary, SLOT(abortSync()));

    QDir dataDirectory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dataDirectory.mkpath(".");

    const String & databaseFilename = dataDirectory.filePath(MOVIES_DATABASE_FILENAME);

    File file(databaseFilename);
    QObject::connect(&application, &QtSingleApplication::aboutToQuit, [&]() {
        file.open(databaseFilename);
        file.write(movieLibrary);
        file.close();
    });

    view.show();
    splashScreen.close();

    if (file.read(movieLibrary))
        movieLibrary.update();

    file.close();

    return application.exec();
}