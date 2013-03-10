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
#include "HappyPCH.h" 

#include "FileReader.h"
#include <sstream>

namespace he {
namespace io {

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
    close();
}
bool FileReader::open( const he::Path& path, OpenType type )
{
    close();
    if (type == OpenType_Unicode)
    {
        m_Wfstream.open(path.str().c_str(), std::ios_base::in);
        if (m_Wfstream.is_open() == false)
            return false;
    }
    else
    {
        m_fstream.open(path.str().c_str(), std::ios_base::in);
        if (m_fstream.is_open() == false)
            return false;
    }
    return true;
}
void FileReader::close()
{
    if (m_fstream.is_open())
        m_fstream.close();
    if (m_Wfstream.is_open())
        m_Wfstream.close();
}

bool FileReader::eof() const
{
    if (m_fstream.is_open())
        return m_fstream.eof();
    else
        return m_Wfstream.eof();
}
he::String FileReader::readLine()
{
    HE_ASSERT(m_fstream.is_open(), "ASCII line reader is not open, did you check for open errors? or did you open it for unicode?");
    he::String str;
    std::getline(m_fstream, str);
    return str;
}
he::String FileReader::readToEnd()
{
    HE_ASSERT(m_fstream.is_open(), "ASCII file reader is not open, did you check for open errors? or did you open it for unicode?");

    std::stringstream file;

    while (m_fstream.eof() == false)
    {
        he::String line;
        std::getline(m_fstream, line);
        file << line << "\n";
    }

    return file.str();
}
void FileReader::readToEndSplit(he::ObjectList<he::String>& outList)
{
    HE_ASSERT(m_fstream.is_open(), "ASCII file reader is not open, did you check for open errors? or did you open it for unicode?");
    
    while (m_fstream.eof() == false)
    {
        he::String line;
        std::getline(m_fstream, line);
        outList.add(line);
    }
}

std::wstring FileReader::readLineW()
{
    HE_ASSERT(m_Wfstream.is_open(), "Unicode line reader is not open, did you check for open errors? or did you open it for ASCII?");
    he::String str;
    std::getline(m_fstream, str);
    return L"";
}
std::wstring FileReader::readToEndW()
{    
    HE_ASSERT(m_Wfstream.is_open(), "unicode file reader is not open, did you check for open errors? or did you open it for ASCII?");
    std::wstringstream file;

    while (m_Wfstream.eof() == false)
    {
        std::wstring line;
        std::getline(m_Wfstream, line);
        file << line << L"\n";
    }

    return file.str();
}
void FileReader::readToEndSplitW(he::ObjectList<std::wstring>& outList)
{    
    HE_ASSERT(m_Wfstream.is_open(), "unicode file reader is not open, did you check for open errors? or did you open it for ASCII?");

    while (m_Wfstream.eof() == false)
    {
        std::wstring line;
        std::getline(m_Wfstream, line);
        outList.add(line);
    }
}

} } //end namespace