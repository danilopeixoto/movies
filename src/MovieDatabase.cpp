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
#include <Runtime.h>
#include <MovieData.h>

#include <QObject>
#include <QtGlobal>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

MOVIES_NAMESPACE_BEGIN

const String MovieDatabase::Key::status = "success";
const String MovieDatabase::Key::results = "results";
const String MovieDatabase::Key::id = "id";
const String MovieDatabase::Key::title = "original_title";
const String MovieDatabase::Key::releaseDate = "release_date";
const String MovieDatabase::Key::overview = "overview";
const String MovieDatabase::Key::credits = "credits";
const String MovieDatabase::Key::cast = "cast";
const String MovieDatabase::Key::name = "name";
const String MovieDatabase::Key::order = "order";
const String MovieDatabase::Key::crew = "crew";
const String MovieDatabase::Key::job = "job";
const String MovieDatabase::Key::department = "department";
const String MovieDatabase::Key::genres = "genres";
const String MovieDatabase::Key::runtime = "runtime";
const String MovieDatabase::Key::poster = "poster_path";

const String MovieDatabase::Value::director = "Director";
const String MovieDatabase::Value::writing = "Writing";

const QNetworkRequest MovieDatabase::authenticationRequest(const String & key) {
    String requestPattern("https://api.themoviedb.org/3/authentication/token/new?api_key=%1");
    return QNetworkRequest(QUrl(requestPattern.arg(key)));
}
const QNetworkRequest MovieDatabase::searchRequest(const String & key, const String & title) {
    String requestPattern("https://api.themoviedb.org/3/search/movie?api_key=%1&query=%2");
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

MovieDatabase & MovieDatabase::wait() {
    eventLoop.exec();
    return *this;
}
Bool MovieDatabase::authenticate(const String & key) {
    QNetworkReply * response = networkManager.get(authenticationRequest(key));
    wait();

    const QJsonDocument & document = QJsonDocument::fromJson(response->readAll());
    delete response;

    if (document.isNull())
        return false;

    const QJsonObject & documentObject = document.object();
    return documentObject[Key::status].toBool();
}
Bool MovieDatabase::retrievePoster(const String & filename, Poster & poster) {
    QNetworkReply * response = networkManager.get(posterRequest(filename));
    wait();

    poster = Poster::fromData(response->readAll());
    delete response;

    return !poster.isNull();
}

MovieDatabase::MovieDatabase() {
    QObject::connect(&networkManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
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
Bool MovieDatabase::isOpen() const {
    return !key.isEmpty();
}

MovieDatabase & MovieDatabase::open(const String & key) {
    if (authenticate(key))
        this->key = key;

    return *this;
}
MovieDatabase & MovieDatabase::close() {
    key.clear();
    return *this;
}

Bool MovieDatabase::search(const String & title, SearchResults & searchResults) {
    if (!isOpen())
        return false;

    QNetworkReply * response = networkManager.get(searchRequest(key, title));
    wait();

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
        searchResults.append(resultObject[Key::id].toInt());
    }

    return true;
}
Bool MovieDatabase::getDetails(UInt id, MovieData & movieData) {
    if (!isOpen())
        return false;

    QNetworkReply * response = networkManager.get(detailRequest(key, id));
    wait();

    const QJsonDocument & document = QJsonDocument::fromJson(response->readAll());
    delete response;

    if (document.isNull())
        return false;

    const QJsonObject & documentObject = document.object();
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
    UInt maximumCastSize = qMin(cast.size(), 5);

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

    Runtime runtime(documentObject[Key::runtime].toInt());
    movieData.setRuntime(runtime);

    const String & posterFilename = documentObject[Key::poster].toString();
    Poster poster;

    if (retrievePoster(posterFilename, poster))
        movieData.setPoster(poster);

    return true;
}

MOVIES_NAMESPACE_END