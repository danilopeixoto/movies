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

#include <MovieData.h>

MOVIES_NAMESPACE_BEGIN

MovieData::MovieData() {}
MovieData::MovieData(const MovieData & movieData)
    : id(movieData.id), title(movieData.title), year(movieData.year),
      overview(movieData.overview), directors(movieData.directors),
      writers(movieData.writers), actors(movieData.actors),
      genres(movieData.genres), runtime(movieData.runtime),
      poster(movieData.poster), url(movieData.url) {}
MovieData::MovieData(UInt id, const String & title, UInt year, const String & overview,
                     const StringList & directors, const StringList & writers,
                     const StringList & actors, const StringList & genres,
                     const Runtime & runtime, const String & poster, const String & url)
    : id(id), title(title), year(year), overview(overview),
      directors(directors), writers(writers), actors(actors),
      genres(genres), runtime(runtime), poster(poster), url(url) {}
MovieData::~MovieData() {}

MovieData & MovieData::operator =(const MovieData & rhs) {
    id = rhs.id;
    title = rhs.title;
    year = rhs.year;
    overview = rhs.overview;
    directors = rhs.directors;
    writers = rhs.writers;
    actors = rhs.actors;
    genres = rhs.genres;
    runtime = rhs.runtime;
    poster = rhs.poster;
    url = rhs.url;

    return *this;
}
Bool MovieData::operator ==(const MovieData & rhs) const {
    return id == rhs.id && title == rhs.title && year == rhs.year
            && overview == rhs.overview && directors == rhs.directors
            && writers == rhs.writers && actors == rhs.writers
            && genres == rhs.genres && runtime == rhs.runtime
            && poster == rhs.poster && url == rhs.url;
}
Bool MovieData::operator !=(const MovieData & rhs) const {
    return !(*this == rhs);
}

QDataStream & operator >>(QDataStream & lhs, MovieData & rhs) {
    return lhs >> rhs.id >> rhs.title >> rhs.year >> rhs.overview
               >> rhs.directors >> rhs.writers >> rhs.actors
               >> rhs.genres >> rhs.runtime >> rhs.poster >> rhs.url;
}
QDataStream & operator <<(QDataStream & lhs, const MovieData & rhs) {
    return lhs << rhs.id << rhs.title << rhs.year << rhs.overview
               << rhs.directors << rhs.writers << rhs.actors
               << rhs.genres << rhs.runtime << rhs.poster << rhs.url;
}

MovieData & MovieData::setID(UInt id) {
    this->id = id;
    return *this;
}
MovieData & MovieData::setTitle(const String & title) {
    this->title = title;
    return *this;
}
MovieData & MovieData::setYear(UInt year) {
    this->year = year;
    return *this;
}
MovieData & MovieData::setOverview(const String & overview) {
    this->overview = overview;
    return *this;
}
MovieData & MovieData::setDirectors(const StringList & directors) {
    this->directors = directors;
    return *this;
}
MovieData & MovieData::setWriters(const StringList & writers) {
    this->writers = writers;
    return *this;
}
MovieData & MovieData::setActors(const StringList & actors) {
    this->actors = actors;
    return *this;
}
MovieData & MovieData::setGenres(const StringList & genres) {
    this->genres = genres;
    return *this;
}
MovieData & MovieData::setRuntime(const Runtime & runtime) {
    this->runtime = runtime;
    return *this;
}
MovieData & MovieData::setPoster(const String & poster) {
    this->poster = poster;
    return *this;
}
MovieData & MovieData::setURL(const String & url) {
    this->url = url;
    return *this;
}

UInt MovieData::getID() const {
    return id;
}
const String & MovieData::getTitle() const {
    return title;
}
UInt MovieData::getYear() const {
    return year;
}
const String & MovieData::getOverview() const {
    return overview;
}
const StringList & MovieData::getDirectors() const {
    return directors;
}
const StringList & MovieData::getWriters() const {
    return writers;
}
const StringList & MovieData::getActors() const {
    return actors;
}
const StringList & MovieData::getGenres() const {
    return genres;
}
const Runtime & MovieData::getRuntime() const {
    return runtime;
}
const String & MovieData::getPoster() const {
    return poster;
}
const String & MovieData::getURL() const {
    return url;
}

String MovieData::toString() const {
    StringList output;

    output << String("%1 (%2)").arg(title).arg(year)
           << overview
           << "Directors: " + directors.join(", ")
           << "Writers: " + writers.join(", ")
           << "Actors: " + actors.join(", ")
           << "Genres: " + genres.join(", ")
           << "Runtime: " + runtime.toString();

    return output.join('\n');
}

MOVIES_NAMESPACE_END