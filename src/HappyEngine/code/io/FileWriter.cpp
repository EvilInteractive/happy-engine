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
#include "HappyPCH.h" 

#include "FileWriter.h"

#include <HappyMessageBox.h>

namespace he {
namespace io {

FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
    close();
}

bool FileWriter::open( const Path& path, const bool overrideWarning )
{
    close();

    bool success(true);
    if (path.exists())
    {
        if (overrideWarning)
        {
            he::String message("Are you sure you want to override\n" + path.str() + "?");
            success = HappyMessageBox::showExt("Override file?", message.c_str(), 
                HappyMessageBox::Icon_Info, "Yes", "No") == HappyMessageBox::Button_Button1;
        }
    }
    else
    {
        path.createFolderStructure();
    }
    if (success)
    {
        m_Stream.open(path.str().c_str(), std::ios_base::out);
    }
    return m_Stream.is_open();
}

void FileWriter::close()
{
    m_Stream.close();
}

FileWriter& FileWriter::operator<<( const char* const str )
{
    HE_ASSERT(m_Stream.is_open(), "File is not open!\nWrite will fail...");
    m_Stream << str;
    return *this;
}

FileWriter& FileWriter::operator<<( const he::String& str )
{
    HE_ASSERT(m_Stream.is_open(), "File is not open!\nWrite will fail...");
    m_Stream << str.c_str();
    return *this;
}

} } //end namespace