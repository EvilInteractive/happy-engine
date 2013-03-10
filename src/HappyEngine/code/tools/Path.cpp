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

Path::Path( const he::String& path ): m_Path(path)
{
    convertBackslashesToForward();
    ensureTrailingSlash();
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

const he::String& Path::str() const
{
    return m_Path;
}

Path Path::append( const he::String& relativePath ) const
{
    Path temp(relativePath); // ensures forward slashes and trailing slash
    const he::String path(temp.str());
    he::String::size_type newLength(m_Path.size() - 2); // -2 : skip trailing slash
    he::String::size_type off(0);
    for (;;)
    {
        he::String::size_type tmpOff(path.find("../", off));
        if (tmpOff == he::String::npos)
            break;
        he::String::size_type sPos(m_Path.rfind('/', newLength));
        if (sPos == he::String::npos)
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
    returnPath.m_Path += path.substr(off);
    
    return returnPath;
}

Path Path::getWorkingDir()
{
    boost::filesystem::path workDir(boost::filesystem::current_path());

    return Path(workDir.string());
}

void Path::ensureTrailingSlash()
{
    if (m_Path.size() > 0 && m_Path.back() != '/')
    {
        bool addSlash(true);
        for (int i(m_Path.size() - 1); i >= 0; --i)
        {
            if (m_Path[i] == '/')
            {
                break;
            }
            else if (m_Path[i] == '.')
            {
                addSlash = false;
                break;
            }
        }
        if (addSlash)
            m_Path.push_back('/');
    }
}

void Path::convertBackslashesToForward()
{
    for (uint32 i(0); i < m_Path.size(); ++i)
    {
        if (m_Path[i] == '\\')
            m_Path[i] = '/';
    }
}

he::String Path::getFileName() const
{
    he::String result("");
    size_t index(m_Path.rfind('/', m_Path.size() - 1));
    if (index != he::String::npos)
    {
        result = m_Path.substr(index + 1, m_Path.size() - index - 1);
    }
    return result;
}

bool Path::iterateFiles( const bool recursive, const boost::function1<void, const Path&>& func )
{
    bool result(false);
    boost::filesystem::path boostPath(m_Path);
    boost::system::error_code error;
    if (boost::filesystem::exists(boostPath, error) && boost::filesystem::is_directory(boostPath, error))
    {
        boost::filesystem::directory_iterator endIt;
        for (boost::filesystem::directory_iterator it(boostPath); it != endIt; ++it)
        {
            if (boost::filesystem::is_regular_file(it->status()))
            {
                func(it->path().string());
            }
            else if (boost::filesystem::is_directory(it->status()))
            {
                Path path(it->path().string());
                path.iterateFiles(recursive, func);
            }
        }
        result = true;
    }
    return result;
}

bool Path::isFile() const
{
    return m_Path.back() != '/';
}

bool Path::isDirectory() const
{
    return !isFile();
}

bool Path::exists() const
{
    boost::filesystem::path boostPath(m_Path);
    boost::system::error_code error;
    return boost::filesystem::exists(boostPath, error);
}

} //end namespace