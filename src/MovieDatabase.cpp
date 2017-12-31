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

#include <MovieDatabase.h>
#include <MovieData.h>

#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

MOVIES_NAMESPACE_BEGIN

const String MovieDatabase::Key::status("success");
const String MovieDatabase::Key::results("results");
const String MovieDatabase::Key::id("id");
const String MovieDatabase::Key::title("title");
const String MovieDatabase::Key::releaseDate("release_date");
const String MovieDatabase::Key::overview("overview");
const String MovieDatabase::Key::credits("credits");
const String MovieDatabase::Key::cast("cast");
const String MovieDatabase::Key::name("name");
const String MovieDatabase::Key::order("order");
const String MovieDatabase::Key::crew("crew");
const String MovieDatabase::Key::job("job");
const String MovieDatabase::Key::department("department");
const String MovieDatabase::Key::genres("genres");
const String MovieDatabase::Key::runtime("runtime");
const String MovieDatabase::Key::poster("poster_path");

const String MovieDatabase::Value::director("Director");
const String MovieDatabase::Value::writing("Writing");

const UInt MovieDatabase::timeout = 5000;

const QNetworkRequest MovieDatabase::authenticationRequest(const String & key) {
    String requestPattern("https://api.themoviedb.org/3/authentication/token/new?api_key=%1");
    return QNetworkRequest(QUrl(requestPattern.arg(key)));
}
const QNetworkRequest MovieDatabase::searchRequest(const String & key, const String & title, UInt year) {
    String requestPattern("https://api.themoviedb.org/3/search/movie?api_key=%1&query=%2");

    if (year != emptyYear)
        requestPattern.append(String("&year=%1").arg(year));

    return QNetworkRequest(QUrl(requestPattern.arg(key).arg(title)));
}
const QNetworkRequest MovieDatabase::detailRequest(const String & key, UInt id) {
    String requestPattern("https://api.themoviedb.org/3/movie/%2?api_key=%1&append_to_response=credits");
    return QNetworkRequest(QUrl(requestPattern.arg(key).arg(id)));
}
const QNetworkRequest MovieDatabase::posterRequest(const String & filename) {
    String requestPattern("https://image.tmdb.org/t/p/w185/%1");
    return QNetworkRequest(QUrl(requestPattern.arg(filename)));
}

Bool MovieDatabase::waiting(QNetworkReply * response) {
    QObject::connect(response, &QNetworkReply::downloadProgress, [&]() {timer.start();});
    eventLoop.exec();

    if (timer.isActive())
        timer.stop();

    return response->isRunning();
}
Bool MovieDatabase::authenticate(const String & key) {
    QNetworkReply * response = networkManager.get(authenticationRequest(key));

    if (waiting(response)) {
        response->abort();
        delete response;

        return false;
    }

    const QJsonDocument & document = QJsonDocument::fromJson(response->readAll());
    delete response;

    if (document.isNull())
        return false;

    const QJsonObject & documentObject = document.object();
    return documentObject[Key::status].toBool();
}
String MovieDatabase::retrievePoster(const String & filename) {
    QNetworkReply * response = networkManager.get(posterRequest(filename));

    if (waiting(response)) {
        response->abort();
        delete response;

        return String();
    }

    QString poster = String::fromLatin1(response->readAll().toBase64().data());

    if (!poster.isNull())
        poster.prepend("data:image/jpg;base64,");

    return poster;
}

const UInt MovieDatabase::emptyYear = 0;

MovieDatabase::MovieDatabase() {
    timer.setInterval(timeout);

    QObject::connect(&timer, &QTimer::timeout,
                     &eventLoop, &QEventLoop::quit);
    QObject::connect(&networkManager, &QNetworkAccessManager::finished,
                     &eventLoop, &QEventLoop::quit);
}
MovieDatabase::MovieDatabase(const String & key) : MovieDatabase() {
    open(key);
}
MovieDatabase::~MovieDatabase() {
    close();
}

const String & MovieDatabase::getKey() const {
    return key;
}
Bool MovieDatabase::isOpen() {
    return authenticate(key);
}

Bool MovieDatabase::open(const String & key) {
    this->key = key;
    return authenticate(key);
}
MovieDatabase & MovieDatabase::close() {
    key.clear();
    return *this;
}

Bool MovieDatabase::search(const String & title, UInt year, SearchResults & searchResults) {
    if (!isOpen())
        return false;

    QNetworkReply * response = networkManager.get(searchRequest(key, title, year));

    if (waiting(response)) {
        response->abort();
        delete response;

        return false;
    }

    const QJsonDocument & document = QJsonDocument::fromJson(response->readAll());
    delete response;

    if (document.isNull())
        return false;

    const QJsonObject & documentObject = document.object();
    const QJsonArray & results = documentObject[Key::results].toArray();

    if (results.isEmpty())
        return false;

    searchResults.clear();

    for (const QJsonValue & result : results) {
        const QJsonObject & resultObject = result.toObject();
        SearchResult searchResult;
        const String & releaseDate = resultObject[Key::releaseDate].toString();

        searchResult.id = resultObject[Key::id].toInt();
        searchResult.title = resultObject[Key::title].toInt();
        searchResult.year = releaseDate.mid(0, releaseDate.indexOf('-')).toUInt();

        searchResults.append(searchResult);
    }

    return !searchResults.isEmpty();
}
Bool MovieDatabase::getDetails(UInt id, MovieData & movieData) {
    if (!isOpen())
        return false;

    QNetworkReply * response = networkManager.get(detailRequest(key, id));

    if (waiting(response)) {
        response->abort();
        delete response;

        return false;
    }

    const QJsonDocument & document = QJsonDocument::fromJson(response->readAll());
    delete response;

    if (document.isNull())
        return false;

    const QJsonObject & documentObject = document.object();

    movieData.setID(documentObject[Key::id].toInt());
    movieData.setTitle(documentObject[Key::title].toString());

    const String & releaseDate = documentObject[Key::releaseDate].toString();
    UInt year = releaseDate.mid(0, releaseDate.indexOf('-')).toUInt();

    movieData.setYear(year);
    movieData.setOverview(documentObject[Key::overview].toString());

    const QJsonObject & creditObject = documentObject[Key::credits].toObject();

    const QJsonArray & crew = creditObject[Key::crew].toArray();
    StringList directors, writers;

    for (const QJsonValue & employee : crew) {
        const QJsonObject & employeeObject = employee.toObject();

        const String & job = employeeObject[Key::job].toString();
        const String & department = employeeObject[Key::department].toString();

        if (job == Value::director)
            directors.append(employeeObject[Key::name].toString());
        else if (department == Value::writing)
            writers.append(employeeObject[Key::name].toString());
    }

    movieData.setDirectors(directors);
    movieData.setWriters(writers);

    const QJsonArray & cast = creditObject[Key::cast].toArray();
    UInt maximumCastSize = min(cast.size(), 5);

    StringList actors;

    for (UInt i = 0; i < maximumCastSize; i++) {
        const QJsonObject & actorObject = cast.at(i).toObject();
        actors.append(actorObject[Key::name].toString());
    }

    movieData.setActors(actors);

    const QJsonArray & genreList = documentObject[Key::genres].toArray();
    StringList genres;

    for (const QJsonValue & genre : genreList) {
        const QJsonObject & genreObject = genre.toObject();
        genres.append(genreObject[Key::name].toString());
    }

    movieData.setGenres(genres);
    movieData.setRuntime(documentObject[Key::runtime].toInt());

    const String & posterFilename = documentObject[Key::poster].toString();
    movieData.setPoster(retrievePoster(posterFilename));

    return true;
}

MOVIES_NAMESPACE_END