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

#ifndef MOVIES_GLOBAL_H
#define MOVIES_GLOBAL_H

#define MOVIES_NAME "Movies"
#define MOVIES_VERSION "1.0.0"
#define MOVIES_AUTHOR "Danilo Ferreira"
#define MOVIES_COPYRIGHT "Copyright (c) 2017, Danilo Ferreira. All rights reserved."
#define MOVIES_LICENSE "BSD-3-Clause License"

#define MOVIES_NAMESPACE_BEGIN namespace movies {
#define MOVIES_NAMESPACE_END };
#define MOVIES_NAMESPACE_USING using namespace movies;

#define MOVIES_NULL nullptr

#include <QtGlobal>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QFile>
#include <QVector>
#include <QMap>

MOVIES_NAMESPACE_BEGIN

typedef QChar Character;
typedef bool Bool;
typedef quint64 UInt;
typedef qint64 Int;
typedef double Float;

typedef QString String;
typedef QStringList StringList;
typedef QImage Poster;
typedef QFile File;

template<typename T> using List = QVector<T>;
template<typename K, typename V> using Map = QMap<K, V>;

typedef List<UInt> SearchResults;

MOVIES_NAMESPACE_END

#endif