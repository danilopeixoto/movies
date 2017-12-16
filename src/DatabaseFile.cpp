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

#include <DatabaseFile.h>
#include <MovieData.h>
#include <MovieCollection.h>

#include <QIODevice>

MOVIES_NAMESPACE_BEGIN

const UInt DatabaseFile::signature() {
    return 0x4D5644;
}

DatabaseFile::DatabaseFile() {
    stream.setDevice(&file);
    stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
}
DatabaseFile::DatabaseFile(const String & filename, OpenMode openMode)
    : DatabaseFile() {
    open(filename, openMode);
}
DatabaseFile::~DatabaseFile() {
    close();
}

const String & DatabaseFile::getFilename() const {
    return file.fileName();
}
DatabaseFile::OpenMode DatabaseFile::getOpenMode() const {
    return static_cast<OpenMode>(file.openMode() - 1);
}
Bool DatabaseFile::isOpen() const {
    return file.isOpen();
}

DatabaseFile & DatabaseFile::open(const String & filename, OpenMode openMode) {
    file.setFileName(filename);
    file.open(static_cast<QIODevice::OpenModeFlag>(openMode + 1));

    return *this;
}
DatabaseFile & DatabaseFile::close() {
    file.close();
    return *this;
}

Bool DatabaseFile::read(MovieCollection & movieCollection) {
    if (!isOpen() || !file.isReadable())
        return false;

    if (!file.reset())
        return false;

    UInt readSignature;
    stream >> readSignature;

    if (readSignature != signature())
        return false;

    movieCollection.clear();
    stream >> movieCollection;

    return stream.status() == QDataStream::Ok;
}
Bool DatabaseFile::write(const MovieCollection & movieCollection) {
    if (!isOpen() || !file.isWritable())
        return false;

    if (!file.resize(0))
        return false;

    stream << signature() << movieCollection;
    return stream.status() == QDataStream::Ok;
}

MOVIES_NAMESPACE_END