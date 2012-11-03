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
//Created: 13/09/2011
#include "HappyPCH.h" 

#include "BinObjLineLoader.h"

#include "BinaryStream.h"

namespace he {
namespace ct {
namespace lines {

BinObjLineLoader::BinObjLineLoader()
{
}


BinObjLineLoader::~BinObjLineLoader()
{
}
bool BinObjLineLoader::load(const std::string& path)
{
    return read(path);
}

bool BinObjLineLoader::read(const std::string& path)
{
    //Clean
    m_PointData.clear();
    m_Indices.clear();

    using namespace std;

    io::BinaryStream stream;
    if (stream.open(path, io::BinaryStream::Read) == false)
        return false;
    
    m_PointData.resize(stream.readDword());
    stream.read(&m_PointData[0], m_PointData.size() * sizeof(vec3));

    m_Indices.resize(stream.readDword());
    stream.read(&m_Indices[0], m_Indices.size() * sizeof(uint16));

    return true;
}


const std::vector<vec3>& BinObjLineLoader::getPoints() const
{
    return m_PointData;
}
const std::vector<uint16>& BinObjLineLoader::getIndices() const
{
    return m_Indices;
}

} } } //end namespace