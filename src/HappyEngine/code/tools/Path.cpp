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
//Created: 25/04/2012
#include "HappyPCH.h" 

#include "Path.h"

#include "boost/filesystem.hpp"

namespace he {

Path::Path( const std::string& path ): m_Path(path)
{
    for (uint i(0); i < m_Path.size(); ++i)
    {
        if (m_Path[i] == '\\')
            m_Path[i] = '/';
    }
    if (m_Path.size() > 0 && m_Path.back() != '/')
        m_Path.push_back('/');
}

Path::Path( const Path& other ): m_Path(other.m_Path)
{
}

Path& Path::operator=( const Path& other )
{
    m_Path = other.m_Path;
    return *this;
}



Path::~Path()
{
}

const std::string& Path::str() const
{
    return m_Path;
}

Path Path::getAbsolutePath( const Path& relativePath ) const
{
    std::string path(relativePath.str());
    std::string::size_type newLength(m_Path.size() - 2); // -2 : skip trailing slash
    std::string::size_type off(0);
    for (;;)
    {
        std::string::size_type tmpOff(path.find("../", off));
        if (tmpOff == std::string::npos)
            break;
        std::string::size_type sPos(m_Path.rfind('/', newLength));
        if (sPos == std::string::npos)
        {
            newLength = 0;
            break;
        }
        else
        {
            newLength = sPos - 1;
            off = tmpOff + 3;
        }
    }
    Path returnPath(m_Path.substr(0, newLength + 1));
    returnPath += path.substr(off);
    return returnPath;
}

Path& Path::operator+=( const std::string& str )
{
    m_Path += str;
    return *this;
}

Path Path::getWorkingPath()
{
	boost::filesystem::path workDir(boost::filesystem::current_path());

	return Path(workDir.string());
}

} //end namespace