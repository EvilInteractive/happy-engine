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
#include "StdAfx.h"

#include "ObjLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "FileReader.h"
#include "FileNotFoundException.h"

#include "HeAssert.h"
#include "ModelLoaderFunctions.h"

namespace he {
namespace ct {
namespace models {

ObjLoader::ObjLoader(): m_NumIndices(0), m_NumVertices(0), m_Vertices(nullptr)
{
}


ObjLoader::~ObjLoader()
{
    free(m_Vertices);
}
void ObjLoader::load(const std::string& path, const gfx::VertexLayout& vertLayout, bool allowByteIndices)
{
    std::cout << "reading...\n";
    m_VertexLayout = vertLayout;

    read(path);

    std::cout << "creating...\n";
    create(allowByteIndices);

    free(m_Vertices);
    m_Vertices = malloc(vertLayout.getVertexSize() * m_NumVertices);
    std::cout << "malloc " << vertLayout.getVertexSize() * m_NumVertices << " bytes\n";
    ASSERT(m_Vertices != nullptr, "not enough memory!");

    std::cout << "filling...\n";
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
            sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
            m_PositionData.push_back(v);
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            vec2 t;
            sscanf(line.c_str(), "vt %f %f", &t.x, &t.y);
            m_TextureData.push_back(t);
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            vec3 n;
            sscanf(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
            m_NormalData.push_back(n);
        }
        else if (line[0] == 'g')
        {
            std::string name(line.substr(2));
            m_GroupData.push_back(name);
            if (m_GroupData.size() > 1)
            {
                Range r;
                if (m_FaceDataMeshRange.size() != 0)
                    r.begin = m_FaceDataMeshRange[m_FaceDataMeshRange.size() - 1].end;
                else
                    r.begin = 0;
                r.end = m_FaceData.size();
                m_FaceDataMeshRange.push_back(r);
            }
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

            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &data[0][0], &data[0][1], &data[0][2],
                                                                   &data[1][0], &data[1][1], &data[1][2],
                                                                   &data[2][0], &data[2][1], &data[2][2]);
            m_FaceData.push_back(data);
        }
    });
    Range r;
    if (m_FaceDataMeshRange.size() != 0)
        r.begin = m_FaceDataMeshRange[m_FaceDataMeshRange.size() - 1].end;
    else
        r.begin = 0;
    r.end = m_FaceData.size();
    m_FaceDataMeshRange.push_back(r);
}

void ObjLoader::flushCreateGroup(uint group)
{
    Range r;

    //Index range
    switch (m_IndexStride[group])
    {
        case gfx::IndexStride_Byte:   r.end = m_IndicesByte.size(); break;
        case gfx::IndexStride_UShort: r.end = m_IndicesUShort.size(); break;
        case gfx::IndexStride_UInt:   r.end = m_IndicesUInt.size(); break;
        default: ASSERT("unkown type"); r.end = 0; break;
    }
    r.begin = r.end - m_NumIndices[group];
    //std::cout << "begin: " << r.begin << ", end: " << r.end << ", num: " << m_NumIndices[group] << ", group: " << group << ", stride: " << m_IndexStride[group] << "\n";
    m_IndexMeshRange.push_back(r);

    //Vertex Range
    if (m_VertexMeshRange.size() != 0)
        r.begin = m_VertexMeshRange.back().end;
    else
        r.begin = 0;
    r.end = m_VertexData.size();
    m_VertexMeshRange.push_back(r);
}
void ObjLoader::create(bool allowByteIndices)
{
    m_IndexMap.clear();
    m_VertexData.clear();
    m_IndicesUInt.clear();
    m_IndicesUShort.clear();
    m_IndicesByte.clear();
    m_NumIndices.clear();
    m_NumVertices = 0;
    m_IndexStride.clear();

    std::for_each(m_FaceDataMeshRange.cbegin(), m_FaceDataMeshRange.cend(), [&](const Range& range)
    {
        uint numIndices((range.end - range.begin) * 3);
        m_NumIndices.push_back(numIndices);
        if (numIndices < UCHAR_MAX && allowByteIndices)
        {
            m_IndexStride.push_back(gfx::IndexStride_Byte);
        }
        else if (numIndices < USHRT_MAX)
        {
            m_IndexStride.push_back(gfx::IndexStride_UShort);
        }
        else if (numIndices < UINT_MAX)
        {
            m_IndexStride.push_back(gfx::IndexStride_UInt);
        }
        else
        {
            ASSERT(false, "too many indices");
        }
    });

    uint faceCount(0);
    uint group(0);
    std::for_each(m_FaceData.cbegin(), m_FaceData.cend(), [&](const std::vector<std::vector<uint>>& face)
    {
        if (m_FaceDataMeshRange[group].end == faceCount)
        {
            flushCreateGroup(group);
            ++group;
        }
        ++faceCount;

        for (int i = 0; i < 3; ++i)
        {
            std::stringstream stream;
            stream << face[i][0] << " " << face[i][1] << " " << face[i][2];
            std::map<std::string, uint>::const_iterator index(m_IndexMap.find(stream.str()));
            if (index == m_IndexMap.cend())
            {
                addIndex(m_VertexData.size(), group);
                m_IndexMap[stream.str()] = m_VertexData.size();
                m_VertexData.push_back(
                    ObjLoader::InternalVertex(
                        vec3(m_PositionData[face[i][0] - 1]),
                        vec2(m_TextureData[face[i][1] - 1]),
                        vec3(m_NormalData[face[i][2] - 1])));
                ++m_NumVertices;
            }
            else
            {
                addIndex(index->second, group);
            }
        }
    });
    flushCreateGroup(group);

    //flip indices for CW culling
    //for (uint i = 0; i < m_IndicesByte.size(); i += 3)
    //{
    //    byte bTemp(0);
    //    bTemp = m_IndicesByte[i + 1];
    //    m_IndicesByte[i + 1] = m_IndicesByte[i + 2];
    //    m_IndicesByte[i + 2] = bTemp;
    //}
    //for (uint i = 0; i < m_IndicesUShort.size(); i += 3)
    //{
    //    ushort usTemp(0);
    //    usTemp = m_IndicesUShort[i + 1];
    //    m_IndicesUShort[i + 1] = m_IndicesUShort[i + 2];
    //    m_IndicesUShort[i + 2] = usTemp;
    //}
    //for (uint i = 0; i < m_IndicesUInt.size(); i += 3)
    //{
    //    uint uiTemp(0);
    //    uiTemp = m_IndicesUInt[i + 1];
    //    m_IndicesUInt[i + 1] = m_IndicesUInt[i + 2];
    //    m_IndicesUInt[i + 2] = uiTemp;
    //}
}
void ObjLoader::addIndex(uint index, uint group)
{
    if (m_VertexMeshRange.size() != 0)
        index -= m_VertexMeshRange.back().end;
    switch (m_IndexStride[group])
    {
        case gfx::IndexStride_Byte:   m_IndicesByte.push_back(static_cast<byte>(index)); break;
        case gfx::IndexStride_UShort: m_IndicesUShort.push_back(static_cast<ushort>(index)); break;
        case gfx::IndexStride_UInt:   m_IndicesUInt.push_back(index); break;
        default: ASSERT("unkown type"); break;
    }
}
void ObjLoader::fill(void* pVertexData, const gfx::VertexLayout& vertLayout) const
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;
    int tanOff = -1;

    std::for_each(vertLayout.getElements().cbegin(), vertLayout.getElements().cend(), [&](const gfx::VertexElement& element)
    {
        if (element.getUsage() == gfx::VertexElement::Usage_Position)
            pOff = element.getByteOffset();
        else if (element.getUsage() == gfx::VertexElement::Usage_TextureCoordinate)
            tOff = element.getByteOffset();
        else if (element.getUsage() == gfx::VertexElement::Usage_Normal)
            nOff = element.getByteOffset();
        else if (element.getUsage() == gfx::VertexElement::Usage_Tangent)
            tanOff = element.getByteOffset();
    });

    //optimazation for struct == internal struct
    if (sizeof(InternalVertex) == vertLayout.getVertexSize())
    {
        if (pOff == 0 && tOff == 12 && nOff == 20)
        {
            memcpy(pVertexData, &m_VertexData[0], m_NumVertices * vertLayout.getVertexSize());
            return;
        }
    }

    char* pCharData = static_cast<char*>(pVertexData);
    uint count = 0;
    uint bytecount(0);
    std::for_each(m_VertexData.cbegin(), m_VertexData.cend(), [&](const InternalVertex& vert)
    {
        if (pOff != -1)
        {
            *reinterpret_cast<vec3*>(&pCharData[count * vertLayout.getVertexSize() + pOff]) = vert.pos;
            bytecount += 12;
        }
        if (tOff != -1)
        {
            *reinterpret_cast<vec2*>(&pCharData[count * vertLayout.getVertexSize() + tOff]) = vert.tex;
            bytecount += 8;
        }
        if (nOff != -1)
        {
            *reinterpret_cast<vec3*>(&pCharData[count * vertLayout.getVertexSize() + nOff]) = vert.norm;
            bytecount += 12;
        }
        ++count;
    });

    if (tanOff != -1)
    {
        std::cout << "starting calculation tangents...    \n";

        for (uint i = 0; i < m_GroupData.size(); ++i)
        {
            std::cout << "    calculating tan's of " << m_GroupData[i] << " start: " << m_VertexMeshRange[i].begin << " num: " << m_VertexMeshRange[i].end - m_VertexMeshRange[i].begin <<
                "vdSize: " << m_VertexData.size() << ", vmrSize" << m_VertexMeshRange.size() << "\n";
            std::vector<vec3> tangents(calculateTangents(&m_VertexData[m_VertexMeshRange[i].begin],
                                                                   m_VertexMeshRange[i].end - m_VertexMeshRange[i].begin,
                                                                   0, 12, 20, sizeof(InternalVertex),
                                                                   getIndices(i), getNumIndices(i), getIndexStride(i)));
            std::cout << "    FILL";
            count = 0;
            std::for_each(tangents.cbegin(), tangents.cend(), [&](const vec3& tan)
            {
                *reinterpret_cast<vec3*>(&pCharData[(m_VertexMeshRange[i].begin + count++) * vertLayout.getVertexSize() + tanOff]) = tan;
                bytecount += 12;
            });
            std::cout << "    DONE\n";
        }

        std::cout << "DONE\n";

    }

    std::cout << "wrote " << bytecount << " bytes\n";
}

const void* ObjLoader::getVertices(uint mesh) const
{
    char* pCharData = static_cast<char*>(m_Vertices);
    return &pCharData[m_VertexMeshRange[mesh].begin * m_VertexLayout.getVertexSize()];
}
const void* ObjLoader::getIndices(uint mesh) const
{
    switch (m_IndexStride[mesh])
    {
        case gfx::IndexStride_Byte:   return &m_IndicesByte[m_IndexMeshRange[mesh].begin];
        case gfx::IndexStride_UShort: return &m_IndicesUShort[m_IndexMeshRange[mesh].begin];
        case gfx::IndexStride_UInt:   return &m_IndicesUInt[m_IndexMeshRange[mesh].begin];
        default: ASSERT("unkown type");  return 0;
    }
}
gfx::IndexStride ObjLoader::getIndexStride(uint mesh) const
{
    return m_IndexStride[mesh];
}
uint ObjLoader::getNumVertices(uint mesh) const
{
    return m_VertexMeshRange[mesh].end - m_VertexMeshRange[mesh].begin;
}
uint ObjLoader::getNumIndices(uint mesh) const
{
    return m_NumIndices[mesh];
}

uint ObjLoader::getNumMeshes() const
{
    return m_GroupData.size();
}
const std::string& ObjLoader::getMeshName(uint mesh) const
{
    return m_GroupData[mesh];
}

} } } //end namespace
