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

    virtual bool visit(const he::FixedString& key, he::String& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, bool& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, int8& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, uint8& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, int16& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, uint16& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, int32& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, uint32& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, int64& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, uint64& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, float& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, double& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, vec2& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, vec3& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, vec4& value, const char* comment = NULL);
    virtual bool visit(const he::FixedString& key, Guid& value, const char* comment = NULL);
  
private:
    Writer* m_Writer;
    Path m_SavePath;

    //Disable default copy constructor and default assignment operator
    JsonFileWriter(const JsonFileWriter&);
    JsonFileWriter& operator=(const JsonFileWriter&);
};

} } //end namespace

#endif
