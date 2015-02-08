//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 11/08/2011

#ifndef _HE_INI_WRITER_H_
#define _HE_INI_WRITER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace he {
namespace io {

class IniWriter
{
private:
    typedef he::Map<he::String,he::ObjectList<std::pair<he::String, he::String>>> IniWriteData;

public:
    IniWriter();
    virtual ~IniWriter();

    void open(const he::String& path);
    bool close();

    void writeBool(const he::String& root, const he::String& node, bool value);
    void writeInt(const he::String& root, const he::String& node, int value);
    void writeFloat(const he::String& root, const he::String& node, float value);

    void writeVector2(const he::String& root, const he::String& node, const vec2& value);
    void writeVector3(const he::String& root, const he::String& node, const vec3& value);
    void writeVector4(const he::String& root, const he::String& node, const vec4& value);

    void writeString(const he::String& root, const he::String& node, const he::String& value);

private:
    IniWriteData m_Data;
    he::String m_Path;
    he::StringStream m_TempStream;

    void writeRaw(const he::String& root, const he::String& node, const he::String& value);

    //Disable default copy constructor and default assignment operator
    IniWriter(const IniWriter&);
    IniWriter& operator=(const IniWriter&);
};

} } //end namespace

#endif
