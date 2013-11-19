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
typedef std::map<std::wstring, std::map<std::wstring, std::wstring>> IniReadData;
public:
    IniReader();
    virtual ~IniReader();

    //throws FileNotFoundException
    bool open(const he::String& path);

    bool isOpen() const;

    //when the asked node is not found, defaultReturn is returned
    bool readBool(const std::wstring& root, const std::wstring& node, bool defaultReturn = false) const;
    int readInt(const std::wstring& root, const std::wstring& node, int defaultReturn = 0) const;
    float readFloat(const std::wstring& root, const std::wstring& node, float defaultReturn = 0.0f) const;

    vec2 readVector2(const std::wstring& root, const std::wstring& node, const vec2& defaultReturn = vec2(0.0f, 0.0f)) const;
    vec3 readVector3(const std::wstring& root, const std::wstring& node, const vec3& defaultReturn = vec3(0.0f, 0.0f, 0.0f)) const;
    vec4 readVector4(const std::wstring& root, const std::wstring& node, const vec4& defaultReturn = vec4(0.0f, 0.0f, 0.0f, 0.0f)) const;

    he::String readString(const std::wstring& root, const std::wstring& node, const he::String& defaultReturn = "") const;
    std::wstring readWString(const std::wstring& root, const std::wstring& node, const std::wstring& defaultReturn = L"") const;

    const std::map<std::wstring, std::wstring>& getNodes(const std::wstring& root) const;

    bool containsRoot(const std::wstring& root) const;

private:
    bool readRaw(const std::wstring& root, const std::wstring& node, std::wstring& returnValue) const;

    IniReadData m_Data;
    bool m_IsOpen;

    //Disable default copy constructor and default assignment operator
    IniReader(const IniReader&);
    IniReader& operator=(const IniReader&);
};

} } //end namespace

#endif
