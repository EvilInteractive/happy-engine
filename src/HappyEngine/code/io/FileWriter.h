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

#ifndef _HE_FILEWRITER_H_
#define _HE_FILEWRITER_H_
#pragma once

#include <fstream>

namespace he {
namespace io {

class FileWriter
{
public:
    FileWriter();
    virtual ~FileWriter();

    bool open(const Path& path, const bool overrideWarning);
    void close();

    FileWriter& operator<<(const char* const str);
    FileWriter& operator<<(const he::String& str);

    template<typename T>
    void operator<<(const T value)
    {
        HE_ASSERT(m_Stream.is_open(), "File is not open!\nWrite will fail...");
        m_Stream << value;
    }

private:
    std::ofstream m_Stream;

    //Disable default copy constructor and default assignment operator
    FileWriter(const FileWriter&);
    FileWriter& operator=(const FileWriter&);
};

} } //end namespace

#endif
