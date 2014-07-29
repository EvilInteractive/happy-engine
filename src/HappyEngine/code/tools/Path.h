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

#ifndef _HE_Path_H_
#define _HE_Path_H_
#pragma once

namespace he {

class HAPPY_ENTRY Path
{
public:
    Path(const he::String& other); // implicit conversion allowed
    Path(const Path& other);
    Path(Path&& other);
    Path& operator=(const Path& other);
    Path& operator=(Path&& other);
    ~Path();

    const he::String& str() const; // includes trailing slash

    //ex:
    //Root = C:/Test/
    //relativePath = ../data/test.png
    //return = C:/data/test.png => absolute
    Path append(const he::String& relativePath) const; 

    // Checks
    bool isFile() const;
    bool isDirectory() const;
    bool exists() const;

    // Getters
    he::String getFileName() const;
    he::Path getDirectory() const;

    // Iterators
    bool iterateFiles(const bool recursive, const boost::function1<void, const Path&>& func);

    // Static
    static const Path& getBinPath() { return s_BinPath; }
    static const Path& getDataPath() { return s_DataPath; }
    static const Path& getUserDir() { return s_UserDataFolder; }

    static void init(const int argc, const char* const * const argv);

    bool operator==(const Path& other) const;
    bool operator!=(const Path& other) const;
    int operator<(const Path& other) const;

private:
    void convertBackslashesToForward();
    void ensureTrailingSlash();

    static Path s_BinPath;
    static Path s_DataPath;
    static Path s_UserDataFolder;

    he::String m_Path;
};

} //end namespace

#endif
