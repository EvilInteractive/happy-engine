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
//Created: 10/08/2011

#ifndef _HE_INI_READER_H_
#define _HE_INI_READER_H_
#pragma once

#include <string>
#include <map>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace happyengine {
namespace io {

class IniReader
{
private:
typedef std::map<std::wstring, std::map<std::wstring, std::wstring>> IniReadData;
public:
	IniReader();
    virtual ~IniReader();

    //throws FileNotFoundException
    void open(const std::string& path);

    bool isOpen() const;

    //when the asked node is not found, defaultReturn is returned
    bool readBool(const std::wstring& root, const std::wstring& node, bool defaultReturn = false) const;
    int readInt(const std::wstring& root, const std::wstring& node, int defaultReturn = 0) const;
    float readFloat(const std::wstring& root, const std::wstring& node, float defaultReturn = 0.0f) const;

    math::Vector2 readVector2(const std::wstring& root, const std::wstring& node, const math::Vector2& defaultReturn = math::Vector2(0.0f, 0.0f)) const;
    math::Vector3 readVector3(const std::wstring& root, const std::wstring& node, const math::Vector3& defaultReturn = math::Vector3(0.0f, 0.0f, 0.0f)) const;
    math::Vector4 readVector4(const std::wstring& root, const std::wstring& node, const math::Vector4& defaultReturn = math::Vector4(0.0f, 0.0f, 0.0f, 0.0f)) const;

    std::string readString(const std::wstring& root, const std::wstring& node, const std::string& defaultReturn = "") const;
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
