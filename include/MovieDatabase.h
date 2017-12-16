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

#ifndef MOVIES_MOVIE_DATABASE_H
#define MOVIES_MOVIE_DATABASE_H

#include <Global.h>

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

MOVIES_NAMESPACE_BEGIN

class MovieData;

class MovieDatabase {
private:
    struct Key {
        static const String status;
        static const String results;
        static const String id;
        static const String title;
        static const String releaseDate;
        static const String overview;
        static const String credits;
        static const String cast;
        static const String name;
        static const String order;
        static const String crew;
        static const String job;
        static const String department;
        static const String genres;
        static const String runtime;
        static const String poster;
    };

    struct Value {
        static const String director;
        static const String writing;
    };

    static const QNetworkRequest authenticationRequest(const String &);
    static const QNetworkRequest searchRequest(const String &, const String &);
    static const QNetworkRequest detailRequest(const String &, UInt);
    static const QNetworkRequest posterRequest(const String &);

    String key;

    QEventLoop eventLoop;
    QNetworkAccessManager networkManager;

    MovieDatabase & wait();
    Bool authenticate(const String &);
    Bool retrievePoster(const String &, Poster &);

public:
    MovieDatabase();
    MovieDatabase(const String &);
    ~MovieDatabase();

    const String & getKey() const;
    Bool isOpen() const;

    MovieDatabase & open(const String &);
    MovieDatabase & close();

    Bool search(const String &, SearchResults &);
    Bool getDetails(UInt, MovieData &);
};

MOVIES_NAMESPACE_END

#endif