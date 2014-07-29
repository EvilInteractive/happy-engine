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
//Created: 10/08/2011

#ifndef _HE_INI_READER_H_
#define _HE_INI_READER_H_
#pragma once

#undef string
#undef wstring
#include <string>
#include <map>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace he {
namespace io {

class IniReader
{
private:
typedef std::map<he::String, std::map<he::String, he::String>> IniReadData;
public:
    IniReader();
    virtual ~IniReader();

    //throws FileNotFoundException
    bool open(const he::String& path);

    bool isOpen() const;

    //when the asked node is not found, defaultReturn is returned
    bool readBool(const he::String& root, const he::String& node, bool defaultReturn = false) const;
    int readInt(const he::String& root, const he::String& node, int defaultReturn = 0) const;
    float readFloat(const he::String& root, const he::String& node, float defaultReturn = 0.0f) const;

    vec2 readVector2(const he::String& root, const he::String& node, const vec2& defaultReturn = vec2(0.0f, 0.0f)) const;
    vec3 readVector3(const he::String& root, const he::String& node, const vec3& defaultReturn = vec3(0.0f, 0.0f, 0.0f)) const;
    vec4 readVector4(const he::String& root, const he::String& node, const vec4& defaultReturn = vec4(0.0f, 0.0f, 0.0f, 0.0f)) const;

    he::String readString(const he::String& root, const he::String& node, const he::String& defaultReturn = "") const;

    const std::map<he::String, he::String>& getNodes(const he::String& root) const;

    bool containsRoot(const he::String& root) const;

private:
    bool readRaw(const he::String& root, const he::String& node, he::String& returnValue) const;

    IniReadData m_Data;
    bool m_IsOpen;

    //Disable default copy constructor and default assignment operator
    IniReader(const IniReader&);
    IniReader& operator=(const IniReader&);
};

} } //end namespace

#endif
