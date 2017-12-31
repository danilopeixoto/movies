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

#ifndef MOVIES_MOVIE_LIBRARY_H
#define MOVIES_MOVIE_LIBRARY_H

#include <Global.h>
#include <MovieDatabase.h>

#include <QObject>
#include <QVariant>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include <QHash>
#include <QWidget>
#include <QRegularExpression>
#include <QMimeDatabase>
#include <QAbstractListModel>
#include <QAbstractItemModel>

MOVIES_NAMESPACE_BEGIN

class MovieData;

class MovieLibrary : public QAbstractListModel {
    Q_OBJECT

public:
    enum Status {
        Unvailable = 0,
        Available,
        Syncronizing,
        EmptyFilteredResults
    };

    Q_ENUM(Status)

    enum Filter {
        Title = 0,
        Year,
        Overview,
        Director,
        Writer,
        Actor,
        Genre,
        Runtime,
        All
    };

    Q_ENUM(Filter)

    enum Property {
        TitleProperty = Qt::UserRole + 1,
        YearProperty,
        OverviewProperty,
        DirectorProperty,
        WritersProperty,
        ActorsProperty,
        GenreProperty,
        RuntimeProperty,
        PosterProperty,
        FilenameProperty
    };

private:
    typedef List<MovieData *> MovieReferenceList;
    typedef OrderedMap<String, MovieData *> FilteredResults;

    static const String videoContentType;
    static const QRegularExpression sanitizerPattern;
    static const QRegularExpression yearPattern;

    static const String sanitize(const String &, Bool = false);

    String databaseKey;
    String contentDirectory;
    Status status;
    Float progress;

    Bool abortedSync;

    MovieCollection movieCollection;
    MovieReferenceList filteredData;

    MovieDatabase movieDatabase;
    QMimeDatabase mimeDatabase;

    MovieLibrary & updateMovieCount();
    MovieLibrary & updateStatus(Status);
    MovieLibrary & updateConditionalStatus(Bool, Status, Status);
    MovieLibrary & updateProgress(Float);

    MovieLibrary & updateData(const FilteredResults * = MOVIES_NULL);
    MovieLibrary & clear();

protected:
    QHash<int, QByteArray> roleNames() const;

public:
    MovieLibrary();
    MovieLibrary(const String &, QWidget * = MOVIES_NULL);
    ~MovieLibrary();

    friend QDataStream & operator >>(QDataStream &, MovieLibrary &);
    friend QDataStream & operator <<(QDataStream &, const MovieLibrary &);

    const String & getDatabaseKey() const;
    const String & getContentDirectory() const;
    const MovieCollection & getMovieCollection() const;
    MovieDataList getFilteredData() const;
    Q_INVOKABLE int getMovieCount() const;
    Q_INVOKABLE Status getStatus() const;
    Q_INVOKABLE double getProgress() const;

    Q_INVOKABLE int filterCount() const;
    Q_INVOKABLE QString filterName(Filter) const;

    int rowCount(const QModelIndex & = QModelIndex()) const;
    QVariant data(const QModelIndex &, int) const;

public slots:
    Bool import(QString);
    Bool update();

    MovieLibrary & abortSync();

    MovieLibrary & filter(QString = QString(), int = Title);

signals:
    void movieCountChanged(int);
    void statusChanged(Status);
    void progressChanged(double);
};

MOVIES_NAMESPACE_END

#endif