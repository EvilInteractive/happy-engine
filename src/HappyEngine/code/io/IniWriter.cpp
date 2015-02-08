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
#include "HappyPCH.h" 

#include "IniWriter.h"

#include <sstream>
#include <fstream>
#include <algorithm>

namespace he {
namespace io {

IniWriter::IniWriter(): m_Path("")
{
}


IniWriter::~IniWriter()
{
}

void IniWriter::open(const he::String& path)
{
    m_Path = path;
    m_Data.clear();
}
bool IniWriter::close()
{
    std::ofstream file(m_Path.c_str());
    m_Path = "";

    if (file.is_open())
    {
        m_Data.forEach([&file](const he::String& key, he::ObjectList<std::pair<he::String, he::String>>& root)
        {
            file << "[" << key << "]" << "\n";

            root.forEach([&file](const std::pair<he::String, he::String>& node)
            {
                file << node.first << " = " << node.second << "\n";
            });
            file << "\n";
        });
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void IniWriter::writeBool(const he::String& root, const he::String& node, bool value)
{
    if (value)
        writeRaw(root, node, "true");
    else
        writeRaw(root, node, "false");
}
void IniWriter::writeInt(const he::String& root, const he::String& node, int value)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << value;
    writeRaw(root, node, m_TempStream.str());
}
void IniWriter::writeFloat(const he::String& root, const he::String& node, float value)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << value;
    writeRaw(root, node, m_TempStream.str());
}

void IniWriter::writeVector2(const he::String& root, const he::String& node, const vec2& value)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << value.x << ", " << value.y;
    writeRaw(root, node, m_TempStream.str());
}
void IniWriter::writeVector3(const he::String& root, const he::String& node, const vec3& value)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << value.x << ", " << value.y << ", " << value.z;
    writeRaw(root, node, m_TempStream.str());
}
void IniWriter::writeVector4(const he::String& root, const he::String& node, const vec4& value)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
    writeRaw(root, node, m_TempStream.str());
}

void IniWriter::writeString(const he::String& root, const he::String& node, const he::String& value)
{
    writeRaw(root, node, "\"" + value + "\"");
}

void IniWriter::writeRaw(const he::String& root, const he::String& node, const he::String& value)
{
    m_Data[root].add(std::make_pair(node, value));
}

} } //end namespace