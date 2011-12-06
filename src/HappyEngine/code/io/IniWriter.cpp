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
#include "HappyPCH.h" 

#include "IniWriter.h"

#include <sstream>
#include <fstream>
#include <algorithm>

namespace he {
namespace io {

IniWriter::IniWriter()
{
}


IniWriter::~IniWriter()
{
}

void IniWriter::open(const std::string& path)
{
    m_Path = path;
    m_Data.clear();
}
bool IniWriter::close()
{
    using namespace std;
    wofstream file(m_Path.c_str());
    m_Path = "";

    if (file.is_open())
    {
        for_each(m_Data.cbegin(), m_Data.cend(), [&file](const pair<wstring, vector<pair<wstring, wstring>>>& root)
        {
            using namespace std;
            file << L"[" << root.first << L"]" << L"\n";

            wofstream& innerFile(file); //nested lambda loses scope -> workaround
            for_each(root.second.cbegin(), root.second.cend(), [&innerFile](const pair<wstring, wstring>& node)
            {
                innerFile << node.first << L" = " << node.second << L"\n";
            });
            file << L"\n";
        });
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void IniWriter::writeBool(const std::wstring& root, const std::wstring& node, bool value)
{
    if (value)
        writeRaw(root, node, L"true");
    else
        writeRaw(root, node, L"false");
}
void IniWriter::writeInt(const std::wstring& root, const std::wstring& node, int value)
{
    std::wstringstream stream;
    stream << value;
    writeRaw(root, node, stream.str());
}
void IniWriter::writeFloat(const std::wstring& root, const std::wstring& node, float value)
{
    std::wstringstream stream;
    stream << value;
    writeRaw(root, node, stream.str());
}

void IniWriter::writeVector2(const std::wstring& root, const std::wstring& node, const vec2& value)
{
    std::wstringstream stream;
    stream << value.x << L", " << value.y;
    writeRaw(root, node, stream.str());
}
void IniWriter::writeVector3(const std::wstring& root, const std::wstring& node, const vec3& value)
{
    std::wstringstream stream;
    stream << value.x << L", " << value.y << L", " << value.z;
    writeRaw(root, node, stream.str());
}
void IniWriter::writeVector4(const std::wstring& root, const std::wstring& node, const vec4& value)
{
    std::wstringstream stream;
    stream << value.x << L", " << value.y << L", " << value.z << L", " << value.w;
    writeRaw(root, node, stream.str());
}

void IniWriter::writeString(const std::wstring& root, const std::wstring& node, const std::string& value)
{
    std::wstring wvalue(value.cbegin(), value.cend());
    writeRaw(root, node, L"\"" + wvalue + L"\"");
}
void IniWriter::writeWString(const std::wstring& root, const std::wstring& node, const std::wstring& value)
{
    writeRaw(root, node, L"\"" + value + L"\"");
}

void IniWriter::writeRaw(const std::wstring& root, const std::wstring& node, const std::wstring& value)
{
    m_Data[root].push_back(std::make_pair(node, value));
}

} } //end namespace