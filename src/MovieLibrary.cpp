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

#include <MovieLibrary.h>
#include <MovieData.h>

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMimeType>

MOVIES_NAMESPACE_BEGIN

const String MovieLibrary::videoContentType("video");
const QRegularExpression MovieLibrary::sanitizerPattern("([^\\d\\w]|_)+");
const QRegularExpression MovieLibrary::yearPattern("^\\d{4}$");

const String MovieLibrary::sanitize(const String & string, Bool preserveSeparators) {
    String output = string.normalized(String::NormalizationForm_KD);

    if (preserveSeparators)
        output = output.replace(sanitizerPattern, " ").trimmed();
    else
        output.replace(sanitizerPattern, "");

    return output.toLower();
}

MovieLibrary & MovieLibrary::updateMovieCount() {
    emit movieCountChanged(movieCollection.size());
    return *this;
}
MovieLibrary & MovieLibrary::updateStatus(Status status) {
    this->status = status;
    emit statusChanged(status);

    return *this;
}
MovieLibrary & MovieLibrary::updateConditionalStatus(Bool state, Status a, Status b) {
    return updateStatus(state ? a : b);
}
MovieLibrary & MovieLibrary::updateProgress(Float progress) {
    this->progress = progress;
    emit progressChanged(progress);

    return *this;
}

MovieLibrary & MovieLibrary::updateData(const FilteredResults * filteredResults) {
    if (filteredResults == MOVIES_NULL) {
        if (movieCollection.isEmpty())
            updateStatus(Unvailable);
        else {
            QAbstractListModel::beginInsertRows(QModelIndex(), 0, movieCollection.size() - 1);

            for (MovieData & movieData : movieCollection)
                filteredData.append(&movieData);

            updateStatus(Available);
            QAbstractListModel::endInsertRows();
        }
    }
    else {
        if (filteredResults->isEmpty())
            updateStatus(EmptyFilteredResults);
        else {
            QAbstractListModel::beginInsertRows(QModelIndex(), 0, filteredResults->size() - 1);

            for (MovieData * movieData : *filteredResults)
                filteredData.append(movieData);

            updateStatus(Available);
            QAbstractListModel::endInsertRows();
        }
    }

    return *this;
}
MovieLibrary & MovieLibrary::clear() {
    if (!filteredData.isEmpty()) {
        QAbstractListModel::beginRemoveRows(QModelIndex(), 0, filteredData.size() - 1);
        filteredData.clear();
        QAbstractListModel::endRemoveRows();
    }

    return *this;
}

QHash<int, QByteArray> MovieLibrary::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[TitleProperty] = "titleProperty";
    roles[YearProperty] = "yearProperty";
    roles[OverviewProperty] = "overviewProperty";
    roles[DirectorProperty] = "directorProperty";
    roles[WritersProperty] = "writersProperty";
    roles[ActorsProperty] = "actorsProperty";
    roles[GenreProperty] = "genreProperty";
    roles[RuntimeProperty] = "runtimeProperty";
    roles[PosterProperty] = "posterProperty";
    roles[FilenameProperty] = "filenameProperty";

    return roles;
}

MovieLibrary::MovieLibrary() {}
MovieLibrary::MovieLibrary(const String & databaseKey, QWidget * parent)
    : QAbstractListModel(parent), databaseKey(databaseKey),
      status(Unvailable), progress(0) {}
MovieLibrary::~MovieLibrary() {}

QDataStream & operator >>(QDataStream & lhs, MovieLibrary & rhs) {
    rhs.clear();

    lhs >> rhs.contentDirectory >> rhs.movieCollection;
    rhs.updateMovieCount();
    rhs.updateData();

    return lhs;
}
QDataStream & operator <<(QDataStream & lhs, const MovieLibrary & rhs) {
    return lhs << rhs.contentDirectory << rhs.movieCollection;
}

const String & MovieLibrary::getDatabaseKey() const {
    return databaseKey;
}
const String & MovieLibrary::getContentDirectory() const {
    return contentDirectory;
}
const MovieCollection & MovieLibrary::getMovieCollection() const {
    return movieCollection;
}
MovieDataList MovieLibrary::getFilteredData() const {
    MovieDataList movieDataList;

    for (const MovieData * movieData : filteredData)
        movieDataList.append(*movieData);

    return movieDataList;
}
int MovieLibrary::getMovieCount() const {
    return movieCollection.size();
}
MovieLibrary::Status MovieLibrary::getStatus() const {
    return status;
}
double MovieLibrary::getProgress() const {
    return progress;
}

int MovieLibrary::filterCount() const {
    return 9;
}
QString MovieLibrary::filterName(Filter filterMode) const {
    switch (filterMode) {
    case Year:
        return "year";
    case Overview:
        return "overview";
    case Director:
        return "director";
    case Writer:
        return "writer";
    case Actor:
        return "actor";
    case Genre:
        return "genre";
    case Runtime:
        return "runtime";
    case All:
        return "all";
    }

    return "title";
}

int MovieLibrary::rowCount(const QModelIndex & index) const {
    Q_UNUSED(index)
    return filteredData.size();
}
QVariant MovieLibrary::data(const QModelIndex & index, int role) const {
    UInt listIndex = index.row();

    if (listIndex < 0 || listIndex >= filteredData.size())
        return QVariant();

    const MovieData * movieData = filteredData[listIndex];

    switch (role) {
    case TitleProperty:
        return movieData->getTitle();
    case YearProperty:
        return String("(%1)").arg(movieData->getYear());
    case OverviewProperty:
        return movieData->getOverview();
    case DirectorProperty:
        return movieData->getDirectors().join(", ");
    case WritersProperty:
        return movieData->getWriters().join(", ");
    case ActorsProperty:
        return movieData->getActors().join(", ");
    case GenreProperty:
        return movieData->getGenres().join(", ");
    case RuntimeProperty:
        return movieData->getRuntime().toString();
    case PosterProperty:
        return movieData->getPoster();
    case FilenameProperty:
        return movieData->getURL();
    }

    return QVariant();
}

Bool MovieLibrary::import(QString contentDirectory) {
    updateProgress(0);
    updateStatus(Syncronizing);

    abortedSync = false;

    if (!movieDatabase.open(databaseKey)) {
        updateConditionalStatus(movieCollection.isEmpty(), Unvailable, Available);
        return false;
    }

    if (!contentDirectory.isNull())
        this->contentDirectory = contentDirectory;

    if (this->contentDirectory.isNull()) {
        updateConditionalStatus(movieCollection.isEmpty(), Unvailable, Available);
        return false;
    }

    QDir directory(this->contentDirectory);

    if (!directory.exists()) {
        updateConditionalStatus(movieCollection.isEmpty(), Unvailable, Available);
        return false;
    }

    const QFileInfoList & fileInfoList = directory.entryInfoList(QDir::Files | QDir::Readable);
    UInt fileInfoCount = fileInfoList.size();

    clear();

    if (fileInfoCount == 0) {
        movieCollection.clear();

        updateMovieCount();
        updateProgress(1.0);
        updateStatus(Unvailable);

        return true;
    }

    MovieCollection temporaryMovieCollection;
    Float step = 1.0 / fileInfoCount;

    for (UInt i = 1; i <= fileInfoCount; i++) {
        if (abortedSync) {
            abortedSync = false;
            break;
        }

        const QFileInfo & fileInfo = fileInfoList[i - 1];
        const QMimeType & mimeType = mimeDatabase.mimeTypeForFile(fileInfo);

        const String & mimeTypeName = mimeType.name();
        const String & contentType = mimeTypeName.mid(0, mimeTypeName.indexOf('/'));

        if (contentType == videoContentType) {
            const String & filename = sanitize(fileInfo.baseName(), true);

            if (movieCollection.contains(filename))
                temporaryMovieCollection[filename] = movieCollection[filename];
            else {
                UInt separatorIndex = filename.lastIndexOf(' ');

                const String & title = filename.mid(0, separatorIndex);
                const String & year = filename.mid(separatorIndex + 1);

                Bool containsYear = filename.size() > year.size() && yearPattern.match(year).hasMatch();
                SearchResults searchResults;

                if (movieDatabase.search(title, containsYear ? year.toUInt() : 0, searchResults)) {
                    const SearchResult & mostPopularResult = searchResults.first();
                    MovieData movieData;

                    if (movieDatabase.getDetails(mostPopularResult.id, movieData)) {
                        movieData.setURL(fileInfo.absoluteFilePath());
                        temporaryMovieCollection.insert(filename, movieData);
                    }
                }
            }
        }

        updateProgress(i * step);
    }

    movieCollection = temporaryMovieCollection;
    updateMovieCount();
    updateData();

    movieDatabase.close();

    return true;
}
Bool MovieLibrary::update() {
    return import(QString());
}

MovieLibrary & MovieLibrary::abortSync() {
    abortedSync = true;
    return *this;
}

MovieLibrary & MovieLibrary::filter(QString data, int filterMode) {
    clear();

    if (data.isNull() || data.isEmpty())
        updateData();
    else {
        FilteredResults filteredResults;
        const String & query = sanitize(data);

        for (MovieData & movieData : movieCollection) {
            const String & title = movieData.getTitle();
            String content;

            switch (filterMode) {
            case Year:
                content = String("%1").arg(movieData.getYear());
                break;
            case Overview:
                content = movieData.getOverview();
                break;
            case Director:
                content = movieData.getDirectors().join(", ");
                break;
            case Writer:
                content = movieData.getWriters().join(", ");
                break;
            case Actor:
                content = movieData.getActors().join(", ");
                break;
            case Genre:
                content = movieData.getGenres().join(", ");
                break;
            case Runtime:
                content = movieData.getRuntime().toString();
                break;
            case All:
                content = movieData.toString();
                break;
            default:
                content = title;
            }

            const String & sanitizedContent = sanitize(content);

            if (sanitizedContent.contains(query))
                filteredResults.insertMulti(title, &movieData);
        }

        updateData(&filteredResults);
    }

    return *this;
}

MOVIES_NAMESPACE_END