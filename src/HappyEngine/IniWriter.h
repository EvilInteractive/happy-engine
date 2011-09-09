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
//
//Author:  Bastian Damman
//Created: 11/08/2011

#ifndef _HE_INI_WRITER_H_
#define _HE_INI_WRITER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace happyengine {
namespace io {

class IniWriter
{
private:
    typedef std::map<std::wstring,std::vector<std::pair<std::wstring, std::wstring>>> IniWriteData;

public:
	IniWriter();
    virtual ~IniWriter();

    void open(const std::string& path);
    bool close();

    void writeBool(const std::wstring& root, const std::wstring& node, bool value);
    void writeInt(const std::wstring& root, const std::wstring& node, int value);
    void writeFloat(const std::wstring& root, const std::wstring& node, float value);

    void writeVector2(const std::wstring& root, const std::wstring& node, const math::Vector2& value);
    void writeVector3(const std::wstring& root, const std::wstring& node, const math::Vector3& value);
    void writeVector4(const std::wstring& root, const std::wstring& node, const math::Vector4& value);

    void writeString(const std::wstring& root, const std::wstring& node, const std::string& value);
    void writeWString(const std::wstring& root, const std::wstring& node, const std::wstring& value);

private:
    IniWriteData m_Data;
    std::string m_Path;

    void writeRaw(const std::wstring& root, const std::wstring& node, const std::wstring& value);

    //Disable default copy constructor and default assignment operator
    IniWriter(const IniWriter&);
    IniWriter& operator=(const IniWriter&);
};

} } //end namespace

#endif
