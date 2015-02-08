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

#include <SDL2/SDL_filesystem.h>

#include <sys/stat.h>
#include <direct.h>

#ifdef WIN32
#include <FileAPI.h>
#endif

namespace he {

Path* Path::s_BinPath(nullptr);
Path* Path::s_DataPath(nullptr);
Path* Path::s_UserDataFolder(nullptr);

void Path::init( const int argc, const char* const * const argv )
{
    char* sdlBasePath(SDL_GetBasePath());
    s_BinPath = HENew(Path)(sdlBasePath);
    SDL_free(sdlBasePath);

    const char* const dataPath(getProgramArgumentValue(argc, argv, "dataPath"));
    if (dataPath == nullptr)
    {
        s_DataPath = HENew(Path)(*s_BinPath);
        size_t index(s_DataPath->m_Path.rfind("/bin/"));
        if (index != he::String::npos)
        {
            s_DataPath->m_Path = s_DataPath->m_Path.substr(0, index);
            s_DataPath->ensureTrailingSlash();
        }
        *s_DataPath = s_DataPath->append("data");
    }
    else
    {
        s_DataPath = HENew(Path)(dataPath);
    }

    char* sdlUserPath(SDL_GetPrefPath("EvilInteractive", "Game"));
    s_UserDataFolder = HENew(Path)(sdlUserPath);
    SDL_free(sdlUserPath);
}

void Path::destroy()
{
    HEDelete(s_BinPath);
    s_BinPath = nullptr;
    HEDelete(s_DataPath);
    s_DataPath = nullptr;
    HEDelete(s_UserDataFolder);
    s_UserDataFolder = nullptr;
}

Path::Path()
{

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
    
    return std::move(returnPath);
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

bool Path::iterateFiles( const bool recursive, const std::function<void(const Path&)>& func, const char* filter /*= "*"*/ )
{
    HE_ASSERT(isDirectory(), "Path is not a directory! cannot perform iterateFiles!");
    if (!isDirectory())
        return false;

    bool result(false);
#ifdef WIN32
    Path searchPath = append(filter);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind(FindFirstFileEx(searchPath.str().c_str(), FindExInfoBasic, &findFileData, FindExSearchNameMatch, NULL, FIND_FIRST_EX_CASE_SENSITIVE));
    if (hFind != INVALID_HANDLE_VALUE) 
    {
        result = true;
        Path resultPath;
        do  
        {
            resultPath.m_Path = m_Path;
            resultPath.m_Path += findFileData.cFileName;
            if ((findFileData.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                if (recursive)
                {
                    resultPath.ensureTrailingSlash();
                    resultPath.iterateFiles(recursive, func, filter);
                }
            }
            else
            {
                func(resultPath);
            }
        } while (FindNextFile(hFind, &findFileData));
        FindClose(hFind);
    }
#else
    DIR* dirp = opendir(m_Path.c_str());
    if (dirp)
    {
        result = true;

        struct dirent* dp;
        Path resultPath;
        while ((dp = readdir(dirp)) != NULL)
        {
            resultPath.m_Path = m_Path;
            resultPath.m_Path += dp->d_name;
            if (dp->d_type == DT_DIR)
            {
                if (recursive)
                {
                    resultPath.ensureTrailingSlash();
                    resultPath.iterateFiles(recursive, func, filter);
                }
            }
            else
            {
                func(resultPath);
            }
        }
        closedir(dirp);
    }
#endif
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
    struct stat buffer;
    return stat(m_Path.c_str(), &buffer) == 0;
}

bool Path::createFolderStructure() const
{
    if (isDirectory() && exists())
        return true;
    he::String path(m_Path);
    const char* dir(path.c_str());
    char* next(&path[0]);
    struct stat buffer;
    while ((next = strstr(next+1, "/")) != nullptr)
    {
        *next = '\0';
        if (stat(dir, &buffer) != 0)
        {
            if (mkdir(dir) != 0)
            {
                LOG(LogType_ProgrammerAssert, "Could not create directory %s", dir);
                return false;
            }
        }
        *next = '/';
    }    
    return true;
}

bool Path::operator==(const Path& other) const
{
    return m_Path == other.m_Path;
}
    
bool Path::operator!=(const Path& other) const
{
    return m_Path != other.m_Path;
}
    
int Path::operator<(const Path& other) const
{
    return m_Path < other.m_Path;
}


} //end namespace
