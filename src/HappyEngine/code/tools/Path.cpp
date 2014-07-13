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
//Created: 25/04/2012
#include "HappyPCH.h" 

#include "Path.h"

#include <boost/filesystem.hpp>
#include <SDL2/SDL_filesystem.h>

namespace he {

Path Path::s_BinPath("");
Path Path::s_DataPath("");
Path Path::s_UserDataFolder("");

void Path::init( const int argc, const char* const * const argv )
{
    char* sdlBasePath(SDL_GetBasePath());
    s_BinPath = Path(sdlBasePath);
    SDL_free(sdlBasePath);

    const char* const dataPath(getProgramArgumentValue(argc, argv, "dataPath"));
    if (dataPath == nullptr)
    {
        s_DataPath = s_BinPath;
        size_t index(s_DataPath.m_Path.rfind("/bin/"));
        if (index != std::string::npos)
        {
            s_DataPath.m_Path = s_DataPath.m_Path.substr(0, index);
            s_DataPath.ensureTrailingSlash();
        }
        s_DataPath = s_DataPath.append("data");
    }
    else
    {
        s_DataPath = Path(dataPath);
    }

    char* sdlUserPath(SDL_GetPrefPath("EvilInteractive", "Game"));
    s_UserDataFolder = Path(sdlUserPath);
    SDL_free(sdlUserPath);
}

Path::Path( const he::String& path ): m_Path(path)
{
    convertBackslashesToForward();
    ensureTrailingSlash();
}

Path::Path( const Path& other ): m_Path(other.m_Path)
{
}

Path::Path(Path&& other) : m_Path(std::move(other.m_Path))
{

}

Path& Path::operator=( const Path& other )
{
    m_Path = other.m_Path;
    return *this;
}

Path& Path::operator=(Path&& other)
{
    m_Path = std::move(other.m_Path);
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

Path Path::getDirectory() const
{
    if (isFile())
        return (*this).append("../");
    else
        return *this;
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
