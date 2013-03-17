//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//
//Author:  Bastian Damman
//Created: 26/01/2013

#ifndef _HE_StructuredFileVisitor_H_
#define _HE_StructuredFileVisitor_H_
#pragma once

#include "StructuredVisitor.h"

namespace he {
namespace io {

class StructuredFileVisitor : public StructuredVisitor
{
public:
    StructuredFileVisitor();
    virtual ~StructuredFileVisitor();

    bool openWrite(const Path& file);
    bool openRead(const Path& file);
    virtual void close();

protected:
    virtual	size_t readBuffer(void* buffer, const size_t byteCount);
    virtual size_t writeBuffer(const void* buffer, const size_t byteCount);
    
private:
    bool open(const Path& file, const char* type);
    FILE* m_File;

    //Disable default copy constructor and default assignment operator
    StructuredFileVisitor(const StructuredFileVisitor&);
    StructuredFileVisitor& operator=(const StructuredFileVisitor&);
};

} } //end namespace

#endif
