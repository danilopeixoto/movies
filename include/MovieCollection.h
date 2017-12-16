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

#ifndef MOVIES_MOVIE_COLLECTION_H
#define MOVIES_MOVIE_COLLECTION_H

#include <Global.h>
#include <MovieData.h>

#include <QDataStream>

MOVIES_NAMESPACE_BEGIN

class MovieCollection : private Map<String, MovieData> {
private:
    typedef Map<String, MovieData> MapContainer;

public:
    typedef MapContainer::iterator Iterator;
    typedef MapContainer::const_iterator ConstIterator;

    MovieCollection() : MapContainer() {}
    MovieCollection(const MovieCollection & movieCollection)
        : MapContainer(movieCollection) {}
    ~MovieCollection() {
        MapContainer::~MapContainer();
    }

    MovieCollection & operator =(const MovieCollection & rhs) {
        return static_cast<MovieCollection &>(MapContainer::operator =(rhs));
    }
    MovieData & operator [](const String & title) {
        return MapContainer::operator [](title);
    }
    const MovieData & operator [](const String & title) const {
        return static_cast<const MovieData &>(MapContainer::operator [](title));
    }
    Bool operator ==(const MovieCollection & rhs) const {
        return MapContainer::operator ==(rhs);
    }
    Bool operator !=(const MovieCollection & rhs) const {
        return MapContainer::operator !=(rhs);
    }

    friend QDataStream & operator >>(QDataStream & lhs, MovieCollection & rhs) {
        return lhs >> static_cast<Map<String, MovieData> &>(rhs);
    }
    friend QDataStream & operator <<(QDataStream & lhs, const MovieCollection & rhs) {
        return lhs << static_cast<const Map<String, MovieData> &>(rhs);
    }

    UInt getSize() const {
        return MapContainer::size();
    }
    Bool isEmpty() const {
        return MapContainer::isEmpty();
    }
    Bool contains(const String & title) const {
        return MapContainer::contains(title);
    }

    Iterator begin() {
        return MapContainer::begin();
    }
    ConstIterator begin() const {
        return MapContainer::constBegin();
    }
    Iterator end() {
        return MapContainer::end();
    }
    ConstIterator end() const {
        return MapContainer::constEnd();
    }

    MovieData & first() {
        return MapContainer::first();
    }
    const MovieData & first() const {
        return static_cast<const MovieData &>(MapContainer::first());
    }
    MovieData & last() {
        return MapContainer::last();
    }
    const MovieData & last() const {
        return static_cast<const MovieData &>(MapContainer::last());
    }

    MovieCollection & insert(const MovieData & movieData) {
        MapContainer::insertMulti(movieData.getTitle(), movieData);
        return *this;
    }
    Iterator remove(const String & title) {
        return MapContainer::erase(find(title));
    }
    Iterator remove(Iterator iterator) {
        return MapContainer::erase(iterator);
    }
    Iterator find(const String & title) {
        return MapContainer::find(title);
    }
    MovieCollection & clear() {
        MapContainer::clear();
        return *this;
    }
};

MOVIES_NAMESPACE_END

#endif