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

#ifndef MOVIES_MOVIE_DATA_H
#define MOVIES_MOVIE_DATA_H

#include <Global.h>
#include <Runtime.h>

#include <QDataStream>

MOVIES_NAMESPACE_BEGIN

class MovieData {
private:
    String title, overview, poster, url;
    UInt id, year;
    StringList directors, writers, actors, genres;
    Runtime runtime;

public:
    MovieData();
    MovieData(const MovieData &);
    MovieData(UInt, const String &, UInt, const String &, const StringList &,
              const StringList &, const StringList &, const StringList &,
              const Runtime &, const String &, const String &);
    ~MovieData();

    MovieData & operator =(const MovieData &);
    Bool operator ==(const MovieData &) const;
    Bool operator !=(const MovieData &) const;

    friend QDataStream & operator >>(QDataStream &, MovieData &);
    friend QDataStream & operator <<(QDataStream &, const MovieData &);

    MovieData & setID(UInt);
    MovieData & setTitle(const String &);
    MovieData & setYear(UInt);
    MovieData & setOverview(const String &);
    MovieData & setDirectors(const StringList &);
    MovieData & setWriters(const StringList &);
    MovieData & setActors(const StringList &);
    MovieData & setGenres(const StringList &);
    MovieData & setRuntime(const Runtime &);
    MovieData & setPoster(const String &);
    MovieData & setURL(const String &);
    UInt getID() const;
    const String & getTitle() const;
    UInt getYear() const;
    const String & getOverview() const;
    const StringList & getDirectors() const;
    const StringList & getWriters() const;
    const StringList & getActors() const;
    const StringList & getGenres() const;
    const Runtime & getRuntime() const;
    const String & getPoster() const;
    const String & getURL() const;

    String toString() const;
};

MOVIES_NAMESPACE_END

#endif