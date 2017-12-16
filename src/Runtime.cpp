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

#include <Runtime.h>

MOVIES_NAMESPACE_BEGIN

Runtime::Runtime() : hours(0), minutes(0) {}
Runtime::Runtime(const Runtime & runtime)
    : hours(runtime.hours), minutes(runtime.minutes) {}
Runtime::Runtime(UInt time) {
    setTime(time);
}
Runtime::Runtime(UInt hours, UInt minutes) {
    setHours(hours);
    setMinutes(minutes);
}
Runtime::~Runtime() {}

Runtime & Runtime::operator =(const Runtime & rhs) {
    hours = rhs.hours;
    minutes = rhs.minutes;

    return *this;
}
Bool Runtime::operator ==(const Runtime & rhs) const {
    return hours == rhs.hours && minutes == rhs.minutes;
}
Bool Runtime::operator !=(const Runtime & rhs) const {
    return !(*this == rhs);
}

QDataStream & operator >>(QDataStream & lhs, Runtime & rhs) {
    return lhs >> rhs.hours >> rhs.minutes;
}
QDataStream & operator <<(QDataStream & lhs, const Runtime & rhs) {
    return lhs << rhs.hours << rhs.minutes;
}

Runtime & Runtime::setTime(UInt time) {
    hours = time / 60;
    minutes = time % 60;

    return *this;
}
Runtime & Runtime::setHours(UInt hours) {
    this->hours = hours;
    return *this;
}
Runtime & Runtime::setMinutes(UInt minutes) {
    this->minutes = minutes < 60 ? minutes : 59;
    return *this;
}
UInt Runtime::getTime() const {
    return hours * 60 + minutes;
}
UInt Runtime::getHours() const {
    return hours;
}
UInt Runtime::getMinutes() const {
    return minutes;
}

String Runtime::toString() const {
    StringList output;

    if (hours != 0)
        output << String("%1h").arg(hours);

    if (minutes != 0)
        output << String("%1m").arg(minutes);

    return output.join(' ');
}

MOVIES_NAMESPACE_END