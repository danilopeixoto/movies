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

#ifndef MOVIES_RUNTIME_H
#define MOVIES_RUNTIME_H

#include <Global.h>

#include <QDataStream>

MOVIES_NAMESPACE_BEGIN

class Runtime {
private:
    UInt hours, minutes;

public:
    Runtime();
    Runtime(const Runtime &);
    Runtime(UInt);
    Runtime(UInt, UInt);
    ~Runtime();

    Runtime & operator =(const Runtime &);
    Bool operator ==(const Runtime &) const;
    Bool operator !=(const Runtime &) const;

    friend QDataStream & operator >>(QDataStream &, Runtime &);
    friend QDataStream & operator <<(QDataStream &, const Runtime &);

    Runtime & setTime(UInt);
    Runtime & setHours(UInt);
    Runtime & setMinutes(UInt);
    UInt getTime() const;
    UInt getHours() const;
    UInt getMinutes() const;

    String toString() const;
};

MOVIES_NAMESPACE_END

#endif