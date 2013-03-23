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

#ifndef _HE_JsonFileWriter_H_
#define _HE_JsonFileWriter_H_
#pragma once

#include "StructuredVisitor.h"

namespace he {
namespace io {

class HAPPY_ENTRY JsonFileWriter : public StructuredVisitor
{
public:
    class Writer;

    JsonFileWriter();
    virtual ~JsonFileWriter();

    void open(const Path& file);
    virtual void close();

    virtual bool enterNode(const he::FixedString& key, const char* comment = NULL);
    virtual void exitNode(const he::FixedString& key);

protected:
    virtual bool enterNode(const size_t index, const char* comment = NULL);
    virtual void exitNode(const size_t index);
    virtual bool enterArray(const he::FixedString& key, const char* comment = NULL);
    virtual void exitArray(const he::FixedString& key);
    virtual size_t getArraySize();
    
    virtual bool visit(he::String& value, const char* comment = NULL);
    virtual bool visit(bool& value, const char* comment = NULL);
    virtual bool visit(int8& value, const char* comment = NULL);
    virtual bool visit(uint8& value, const char* comment = NULL);
    virtual bool visit(int16& value, const char* comment = NULL);
    virtual bool visit(uint16& value, const char* comment = NULL);
    virtual bool visit(int32& value, const char* comment = NULL);
    virtual bool visit(uint32& value, const char* comment = NULL);
    virtual bool visit(int64& value, const char* comment = NULL);
    virtual bool visit(uint64& value, const char* comment = NULL);
    virtual bool visit(float& value, const char* comment = NULL);
    virtual bool visit(double& value, const char* comment = NULL);

private:
    Writer* m_Writer;
    Path m_SavePath;

    //Disable default copy constructor and default assignment operator
    JsonFileWriter(const JsonFileWriter&);
    JsonFileWriter& operator=(const JsonFileWriter&);
};

} } //end namespace

#endif
