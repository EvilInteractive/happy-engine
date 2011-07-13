//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _FILE_OPEN_EXCEPTION_H_
#define _FILE_OPEN_EXCEPTION_H_
#pragma once

#include "Exception.h"

namespace happyengine {
namespace error {

class FileOpenException : public Exception
{
public:
	FileOpenException();
	FileOpenException(const std::wstring& file);
    virtual ~FileOpenException();
    //default copy constructor and assignment operator are fine

    virtual std::wstring getType() const;
};

} } //end namespace

#endif
