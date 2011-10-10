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
//
//Author: Bastian Damman
//Created: 01/09/2011
#include "StdAfx.h" 

#include "ObjLineLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "FileReader.h"
#include "FileNotFoundException.h"

#include "Assert.h"

namespace he {
namespace ct {
namespace lines {

ObjLineLoader::ObjLineLoader()
{
}


ObjLineLoader::~ObjLineLoader()
{
}
void ObjLineLoader::load(const std::string& path)
{
    read(path);        
}

void ObjLineLoader::read(const std::string& path)
{
    //Clean
    m_PointData.clear();
    m_Indices.clear();

    using namespace std;

    io::FileReader reader;
    vector<string> objData;
    try 
    { 
        reader.open(path, io::FileReader::OpenType_ASCII); 
        objData = reader.readToEndSplit();
    }
    catch (err::FileNotFoundException&)
    {
        reader.close();
        throw;
    }
    
    for_each(objData.cbegin(), objData.cend(), [&](const string& line)
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            vec3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
            m_PointData.push_back(v);
        }
        else if (line[0] == 'l')
        {
            std::stringstream stream;
            std::vector<ushort>& indices(m_Indices); //fix for_each scope loss
            std::for_each(line.cbegin() + 2, line.cend(), [&](char c)
            {
                if (c == ' ')
                {
                    ushort i;
                    stream >> i;
                    indices.push_back(i);
                    stream.clear();
                    stream.str("");
                }
                else
                {
                    stream << c;
                }
            });
            
            ushort i;
            stream >> i;
            indices.push_back(i);
        }
    });
}
const std::vector<vec3>& ObjLineLoader::getPoints() const
{
    return m_PointData;
}
const std::vector<ushort>& ObjLineLoader::getIndices() const
{
    return m_Indices;
}

} } } //end namespace