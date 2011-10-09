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

#include "FileReader.h"
#include "FileNotFoundException.h"
#include <sstream>
#include "Assert.h"

namespace happyengine {
namespace io {

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
    close();
}
void FileReader::open(const std::string& path, OpenType type)
{
    close();
    if (type == OpenType_Unicode)
    {
        m_Wfstream.open(path.c_str(), std::ios_base::in);
        if (m_Wfstream.is_open() == false)
            throw error::FileNotFoundException(path);
    }
    else
    {
        m_fstream.open(path.c_str(), std::ios_base::in);
        if (m_fstream.is_open() == false)
            throw error::FileNotFoundException(path);
    }
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
std::string FileReader::readLine()
{
    ASSERT(m_fstream.is_open(), "ASCII line reader is not open, did you check for open exceptions? or did you open it for unicode?");
    std::string str;
    std::getline(m_fstream, str);
    return str;
}
std::string FileReader::readToEnd()
{
    ASSERT(m_fstream.is_open(), "ASCII file reader is not open, did you check for open exceptions? or did you open it for unicode?");

    std::stringstream file;

    while (m_fstream.eof() == false)
    {
        std::string line;
        std::getline(m_fstream, line);
        file << line << "\n";
    }

    return file.str();
}
std::vector<std::string> FileReader::readToEndSplit()
{
    ASSERT(m_fstream.is_open(), "ASCII file reader is not open, did you check for open exceptions? or did you open it for unicode?");

    std::vector<std::string> file;

    while (m_fstream.eof() == false)
    {
        std::string line;
        std::getline(m_fstream, line);
        file.push_back(line);
    }

    return file;
}

std::wstring FileReader::readLineW()
{
    ASSERT(m_Wfstream.is_open(), "Unicode line reader is not open, did you check for open exceptions? or did you open it for ASCII?");
    std::string str;
    std::getline(m_fstream, str);
    return L"";
}
std::wstring FileReader::readToEndW()
{    
    ASSERT(m_Wfstream.is_open(), "unicode file reader is not open, did you check for open exceptions? or did you open it for ASCII?");
    std::wstringstream file;

    while (m_Wfstream.eof() == false)
    {
        std::wstring line;
        std::getline(m_Wfstream, line);
        file << line << L"\n";
    }

    return file.str();
}
std::vector<std::wstring> FileReader::readToEndSplitW()
{    
    ASSERT(m_Wfstream.is_open(), "unicode file reader is not open, did you check for open exceptions? or did you open it for ASCII?");
    std::vector<std::wstring> file;

    while (m_Wfstream.eof() == false)
    {
        std::wstring line;
        std::getline(m_Wfstream, line);
        file.push_back(line);
    }

    return file;
}

} } //end namespace