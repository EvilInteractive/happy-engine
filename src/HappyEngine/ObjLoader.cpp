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
//Created: 04/08/2011

#include "ObjLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "FileReader.h"
#include "FileNotFoundException.h"

#include "Assert.h"

namespace happyengine {
namespace content {
namespace models {

ObjLoader::ObjLoader(): m_NumIndices(0), m_NumVertices(0), m_Vertices(nullptr)
{
}


ObjLoader::~ObjLoader()
{
    free(m_Vertices);
}
void ObjLoader::load(const std::string& path, const graphics::VertexLayout& vertLayout, bool allowByteIndices)
{
    read(path);
    create(allowByteIndices);

    free(m_Vertices);
    m_Vertices = malloc(vertLayout.getVertexSize() * m_NumVertices);
    std::cout << "malloc " << vertLayout.getVertexSize() * m_NumVertices << " bytes\n";
    ASSERT(m_Vertices != nullptr, "not enough memory!");
        
    fill(m_Vertices, vertLayout);
}

void ObjLoader::read(const std::string& path)
{
    //Clean
    m_PositionData.clear();
    m_TextureData.clear();
    m_NormalData.clear();
    m_FaceData.clear();

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
            m_PositionData.push_back(v);
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            Vector2 t;
            sscanf_s(line.c_str(), "vt %f %f", &t.x, &t.y);
            t = -t;
            m_TextureData.push_back(t);
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            Vector3 n;
            sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
            m_NormalData.push_back(n);
        }
        else if (line[0] == 'f')
        {
            std::vector<std::vector<uint>> data;
            for (int i = 0; i < 3; ++i)
            {
                std::vector<uint> temp(3);
                temp.resize(3);
                data.push_back(temp);
            }

            sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &data[0][0], &data[0][1], &data[0][2],
                                                                   &data[1][0], &data[1][1], &data[1][2],
                                                                   &data[2][0], &data[2][1], &data[2][2]);
            m_FaceData.push_back(data);
        }
    });
}
void ObjLoader::create(bool allowByteIndices)
{
    m_IndexMap.clear();
    m_VertexData.clear();
    m_IndicesUInt.clear();
    m_IndicesUShort.clear();
    m_IndicesByte.clear();

    m_NumIndices = m_FaceData.size() * 3;
    if (m_NumIndices < UCHAR_MAX && allowByteIndices)
    {
        m_IndexType = graphics::IndexType_Byte;
    }
    else if (m_NumIndices < USHRT_MAX)
    {
        m_IndexType = graphics::IndexType_UShort;
    }
    else if (m_NumIndices < UINT_MAX)
    {
        m_IndexType = graphics::IndexType_UInt;
    }
    else
    {
        ASSERT("too many indices");
    }

    m_NumVertices = 0;
    std::for_each(m_FaceData.cbegin(), m_FaceData.cend(), [&](const std::vector<std::vector<uint>>& face)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::stringstream stream;
            stream << face[i][0] << " " << face[i][1] << " " << face[i][2];
            std::map<std::string, uint>::const_iterator index(m_IndexMap.find(stream.str()));
            if (index == m_IndexMap.cend())
            {
                addIndex(m_VertexData.size());
                m_IndexMap[stream.str()] = m_VertexData.size();
                m_VertexData.push_back(
                    ObjLoader::TempVertex(
                        math::Vector3(m_PositionData[face[i][0] - 1]),
                        math::Vector2(m_TextureData[face[i][1] - 1]),
                        math::Vector3(m_NormalData[face[i][2] - 1])));
                ++m_NumVertices;
            }
            else
            {
                addIndex(index->second);
            }
        }
    });

    //flip indices for CCW culling
    for (uint i = 0; i < m_NumIndices; i += 3)
    {
        byte bTemp(0);
        ushort usTemp(0);
        uint uiTemp(0);
        switch (m_IndexType)
        {
            case graphics::IndexType_Byte:   
                bTemp = m_IndicesByte[i + 1];
                m_IndicesByte[i + 1] = m_IndicesByte[i + 2]; 
                m_IndicesByte[i + 2] = bTemp; break;
            case graphics::IndexType_UShort: 
                usTemp = m_IndicesUShort[i + 1];
                m_IndicesUShort[i + 1] = m_IndicesUShort[i + 2]; 
                m_IndicesUShort[i + 2] = usTemp; break;
            case graphics::IndexType_UInt:   
                uiTemp = m_IndicesUInt[i + 1];
                m_IndicesUInt[i + 1] = m_IndicesUInt[i + 2]; 
                m_IndicesUInt[i + 2] = uiTemp; break;
            default: ASSERT("unkown type"); break;
        }
    }
}
void ObjLoader::addIndex(uint index)
{
    switch (m_IndexType)
    {
        case graphics::IndexType_Byte:   m_IndicesByte.push_back(static_cast<byte>(index)); break;
        case graphics::IndexType_UShort: m_IndicesUShort.push_back(static_cast<ushort>(index)); break;
        case graphics::IndexType_UInt:   m_IndicesUInt.push_back(index); break;
        default: ASSERT("unkown type"); break;
    }
}
void ObjLoader::fill(void* pVertexData, const graphics::VertexLayout& vertLayout) const
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;

    std::for_each(vertLayout.getElements().cbegin(), vertLayout.getElements().cend(), [&](const graphics::VertexElement& element)
    {
        if (element.getUsage() == graphics::VertexElement::Usage_Position)
            pOff = element.getByteOffset();
        else if (element.getUsage() == graphics::VertexElement::Usage_TextureCoordinate)
            tOff = element.getByteOffset();
        else if (element.getUsage() == graphics::VertexElement::Usage_Normal)
            nOff = element.getByteOffset();
    });

    char* pCharData = static_cast<char*>(pVertexData);
    uint count = 0;
    uint bytecount(0);
    std::for_each(m_VertexData.cbegin(), m_VertexData.cend(), [&](const TempVertex& vert)
    {
        if (pOff != -1)
        {
            *reinterpret_cast<math::Vector3*>(&pCharData[count * vertLayout.getVertexSize() + pOff]) = vert.pos;
            bytecount += 12;
        }
        if (tOff != -1)
        {
            *reinterpret_cast<math::Vector2*>(&pCharData[count * vertLayout.getVertexSize() + tOff]) = vert.tex;
            bytecount += 8;
        }
        if (nOff != -1)
        {
            *reinterpret_cast<math::Vector3*>(&pCharData[count * vertLayout.getVertexSize() + nOff]) = vert.norm;
            bytecount += 12;
        }
        ++count;
    });
    std::cout << "wrote " << bytecount << " bytes\n";
}

const void* ObjLoader::getVertices() const
{
    return m_Vertices;
}
const void* ObjLoader::getIndices() const 
{
    switch (m_IndexType)
    {
        case graphics::IndexType_Byte:   return &m_IndicesByte[0];
        case graphics::IndexType_UShort: return &m_IndicesUShort[0];
        case graphics::IndexType_UInt:   return &m_IndicesUInt[0];
        default: ASSERT("unkown type");  return 0;
    }
}
graphics::IndexType ObjLoader::getIndexType() const
{
    return m_IndexType;
}
uint ObjLoader::getNumVertices() const
{
    return m_NumVertices;
}
uint ObjLoader::getNumIndices() const
{
    return m_NumIndices;
}

} } } //end namespace