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

#include "ObjLineLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "FileReader.h"
#include "FileNotFoundException.h"

#include "Assert.h"

namespace happyengine {
namespace content {
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

    using namespace std;

    io::FileReader reader;
    vector<string> objData;
    try 
    { 
        reader.open(path, io::FileReader::OpenType_ASCII); 
        objData = reader.readToEndSplit();
    }
    catch (error::FileNotFoundException&)
    {
        reader.close();
        throw;
    }
    
    for_each(objData.cbegin(), objData.cend(), [&](const string& line)
    {
        using namespace math;
        if (line[0] == 'v' && line[1] == ' ')
        {
            Vector3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
            m_PointData.push_back(v);
        }
    });
}
const std::vector<math::Vector3>& ObjLineLoader::getPoints() const
{
    return m_PointData;
}

} } } //end namespace