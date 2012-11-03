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

#ifndef _HE_Path_H_
#define _HE_Path_H_
#pragma once

namespace he {

class Path
{
public:
    Path(const std::string& other); // implicit conversion allowed
    Path(const Path& other);
    Path& operator=(const Path& other);
    virtual ~Path();

    const std::string& str() const; // include trailing slash

    //ex:
    //Root = C:/Test/
    //relativePath = ../data/test.png
    //return = C:/data/test.png => absolute
    Path getAbsolutePath(const Path& relativePath) const; 

    static Path getWorkingPath();

private:
    std::string m_Path;
    Path& operator+=(const std::string& str);

};

} //end namespace

#endif
