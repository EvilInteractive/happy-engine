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
//Author: Bastian Damman
//Created: 04/08/2011
#include "HappyPCH.h"

#include "ObjLoader.h"

#include "FileReader.h"

#include "ModelLoaderFunctions.h"
#include "ModelMesh.h"

namespace he {
namespace ct {
namespace models {

ObjLoader::ObjLoader(): m_NumIndices(0), m_NumVertices(0), m_Vertices(nullptr)
{
}


ObjLoader::~ObjLoader()
{
    he_free(m_Vertices);
}
bool ObjLoader::load(const he::String& path, bool allowByteIndices)
{
    std::cout << "reading...\n";

    if (read(path) == false)
    {
        HE_WARNING("Obj read failed: %s", path.c_str());
        return false;
    }

    std::cout << "creating...\n";
    create(allowByteIndices);

    he_free(m_Vertices);
    m_Vertices = he_malloc(m_VertexLayout.getSize() * m_NumVertices);
    std::cout << "malloc " << m_VertexLayout.getSize() * m_NumVertices << " bytes\n";
    HE_ASSERT(m_Vertices != nullptr, "not enough memory!");

    std::cout << "filling...\n";
    fill(m_Vertices);

    return true;
}

bool ObjLoader::read(const he::String& path)
{
    //Clean
    m_PositionData.clear();
    m_TextureData.clear();
    m_NormalData.clear();
    m_FaceData.clear();

    using namespace std;

    io::FileReader reader;
    he::ObjectList<he::String> objData;
    if (reader.open(path, io::FileReader::OpenType_ASCII))
    {
        reader.readToEndSplit(objData);
        reader.close();
    }
    else
    {
        return false;
    }
    
    for_each(objData.cbegin(), objData.cend(), [&](const string& line)
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            vec3 v;
            sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
            m_PositionData.add(v);
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            vec2 t;
            sscanf(line.c_str(), "vt %f %f", &t.x, &t.y);
            m_TextureData.add(t);
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            vec3 n;
            sscanf(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
            m_NormalData.add(n);
        }
        else if (line[0] == 'g')
        {
            he::String name(line.substr(2));
            m_GroupData.add(name);
            if (m_GroupData.size() > 1)
            {
                Range r;
                if (m_FaceDataMeshRange.size() != 0)
                    r.begin = m_FaceDataMeshRange[m_FaceDataMeshRange.size() - 1].end;
                else
                    r.begin = 0;
                r.end = static_cast<uint32>(m_FaceData.size());
                m_FaceDataMeshRange.add(r);
            }
        }
        else if (line[0] == 'f')
        {
            FaceData data;

            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &data.data[0][0], &data.data[0][1], &data.data[0][2],
                                                                 &data.data[1][0], &data.data[1][1], &data.data[1][2],
                                                                 &data.data[2][0], &data.data[2][1], &data.data[2][2]);
            m_FaceData.add(data);
        }
    });
    Range r;
    if (m_FaceDataMeshRange.size() != 0)
        r.begin = m_FaceDataMeshRange[m_FaceDataMeshRange.size() - 1].end;
    else
        r.begin = 0;
    r.end = static_cast<uint32>(m_FaceData.size());
    m_FaceDataMeshRange.add(r);


    HE_ASSERT(m_VertexLayout.getSize() == 0, "vertexlayout not empty!");
    m_VertexLayout.addElement(gfx::BufferElement(gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, 12, 0));
    m_VertexLayout.addElement(gfx::BufferElement(gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_TextureCoordinate, 8, 12));
    m_VertexLayout.addElement(gfx::BufferElement(gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Normal, 12, 20));
    m_VertexLayout.addElement(gfx::BufferElement(gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Tangent, 12, 32));

    return true;
}

void ObjLoader::flushCreateGroup(uint32 group)
{
    Range r;

    //Index range
    switch (m_IndexStride[group])
    {
        case gfx::IndexStride_Byte:   r.end = static_cast<uint32>(m_IndicesByte.size()); break;
        case gfx::IndexStride_UShort: r.end = static_cast<uint32>(m_IndicesUShort.size()); break;
        case gfx::IndexStride_UInt:   r.end = static_cast<uint32>(m_IndicesUInt.size()); break;
        default: LOG(LogType_ProgrammerAssert, "unknown type: %d", m_IndexStride[group]); r.end = 0; break;
    }
    r.begin = r.end - m_NumIndices[group];
    //std::cout << "begin: " << r.begin << ", end: " << r.end << ", num: " << m_NumIndices[group] << ", group: " << group << ", stride: " << m_IndexStride[group] << "\n";
    m_IndexMeshRange.add(r);

    //Vertex Range
    if (m_VertexMeshRange.size() != 0)
        r.begin = m_VertexMeshRange.back().end;
    else
        r.begin = 0;
    r.end = static_cast<uint32>(m_VertexData.size());
    m_VertexMeshRange.add(r);
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

    m_FaceDataMeshRange.forEach([&](const Range& range)
    {
        uint32 numIndices((range.end - range.begin) * 3);
        m_NumIndices.add(numIndices);
        if (numIndices < UCHAR_MAX && allowByteIndices)
        {
            m_IndexStride.add(gfx::IndexStride_Byte);
        }
        else if (numIndices < USHRT_MAX)
        {
            m_IndexStride.add(gfx::IndexStride_UShort);
        }
        else if (numIndices < SIZE_MAX)
        {
            m_IndexStride.add(gfx::IndexStride_UInt);
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "too many indices");
        }
    });

    uint32 faceCount(0);
    uint32 group(0);
    m_FaceData.forEach([&](const FaceData& face)
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
            stream << face.data[i][0] << " " << face.data[i][1] << " " << face.data[i][2];
            std::map<he::String, uint32>::const_iterator index(m_IndexMap.find(stream.str()));
            if (index == m_IndexMap.cend())
            {
                addIndex(static_cast<uint32>(m_VertexData.size()), group);
                m_IndexMap[stream.str()] = static_cast<uint32>(m_VertexData.size());
                m_VertexData.add(
                    ObjLoader::InternalVertex(
                        vec3(m_PositionData[face.data[i][0] - 1]),
                        vec2(m_TextureData[face.data[i][1] - 1]),
                        vec3(m_NormalData[face.data[i][2] - 1])));
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
void ObjLoader::addIndex(uint32 index, uint32 group)
{
    if (m_VertexMeshRange.size() != 0)
        index -= m_VertexMeshRange.back().end;
    switch (m_IndexStride[group])
    {
        case gfx::IndexStride_Byte:   m_IndicesByte.add(static_cast<uint8>(index)); break;
        case gfx::IndexStride_UShort: m_IndicesUShort.add(static_cast<uint16>(index)); break;
        case gfx::IndexStride_UInt:   m_IndicesUInt.add(index); break;
        default: LOG(LogType_ProgrammerAssert, "unknown type: %d", m_IndexStride[group]); break;
    }
}
void ObjLoader::fill(void* pVertexData) const
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;
    int tanOff = -1;

    m_VertexLayout.getElements().forEach([&](const gfx::BufferElement& element)
    {
        if (element.getUsage() == gfx::BufferElement::Usage_Position)
            pOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_TextureCoordinate)
            tOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_Normal)
            nOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_Tangent)
            tanOff = element.getByteOffset();
    });

    //optimazation for struct == internal struct
    if (sizeof(InternalVertex) == m_VertexLayout.getSize())
    {
        if (pOff == 0 && tOff == 12 && nOff == 20)
        {
            he_memcpy(pVertexData, &m_VertexData[0], m_NumVertices * m_VertexLayout.getSize());
            return;
        }
    }

    char* pCharData = static_cast<char*>(pVertexData);
    size_t count = 0;
    size_t bytecount(0);
    m_VertexData.forEach([&](const InternalVertex& vert)
    {
        if (pOff != -1)
        {
            *reinterpret_cast<vec3*>(&pCharData[count * m_VertexLayout.getSize() + pOff]) = vert.pos;
            bytecount += 12;
        }
        if (tOff != -1)
        {
            *reinterpret_cast<vec2*>(&pCharData[count * m_VertexLayout.getSize() + tOff]) = vert.tex;
            bytecount += 8;
        }
        if (nOff != -1)
        {
            *reinterpret_cast<vec3*>(&pCharData[count * m_VertexLayout.getSize() + nOff]) = vert.norm;
            bytecount += 12;
        }
        ++count;
    });

    if (tanOff != -1)
    {
        std::cout << "starting calculation tangents...    \n";

        for (size_t i = 0; i < m_GroupData.size(); ++i)
        {
            std::cout << "    calculating tan's of " << m_GroupData[i] << " start: " << m_VertexMeshRange[i].begin << " num: " << m_VertexMeshRange[i].end - m_VertexMeshRange[i].begin <<
                "vdSize: " << m_VertexData.size() << ", vmrSize" << m_VertexMeshRange.size() << "\n";
            he::PrimitiveList<vec3> tangents(m_NumVertices);
            calculateTangents( &m_VertexData[m_VertexMeshRange[i].begin],
                                m_VertexMeshRange[i].end - m_VertexMeshRange[i].begin,
                                0, 12, 20, sizeof(InternalVertex),
                                getIndices(static_cast<uint32>(i)), 
                                getNumIndices(static_cast<uint32>(i)), 
                                getIndexStride(static_cast<uint32>(i)), tangents);
            std::cout << "    FILL";
            count = 0;
            tangents.forEach([&](const vec3& tan)
            {
                *reinterpret_cast<vec3*>(&pCharData[(m_VertexMeshRange[i].begin + count++) * m_VertexLayout.getSize() + tanOff]) = tan;
                bytecount += 12;
            });
            std::cout << "    DONE\n";
        }

        std::cout << "DONE\n";

    }

    std::cout << "wrote " << bytecount << " bytes\n";
}

const void* ObjLoader::getVertices(uint32 mesh) const
{
    char* pCharData = static_cast<char*>(m_Vertices);
    return &pCharData[m_VertexMeshRange[mesh].begin * m_VertexLayout.getSize()];
}
const void* ObjLoader::getIndices(uint32 mesh) const
{
    switch (m_IndexStride[mesh])
    {
        case gfx::IndexStride_Byte:   return &m_IndicesByte[m_IndexMeshRange[mesh].begin];
        case gfx::IndexStride_UShort: return &m_IndicesUShort[m_IndexMeshRange[mesh].begin];
        case gfx::IndexStride_UInt:   return &m_IndicesUInt[m_IndexMeshRange[mesh].begin];
        default: LOG(LogType_ProgrammerAssert, "unknown type: %d", m_IndexStride[mesh]);  return 0;
    }
}
gfx::IndexStride ObjLoader::getIndexStride(uint32 mesh) const
{
    return m_IndexStride[mesh];
}
size_t ObjLoader::getNumVertices(uint32 mesh) const
{
    return m_VertexMeshRange[mesh].end - m_VertexMeshRange[mesh].begin;
}
size_t ObjLoader::getNumIndices(uint32 mesh) const
{
    return m_NumIndices[mesh];
}

size_t ObjLoader::getNumMeshes() const
{
    return m_GroupData.size();
}
const he::String& ObjLoader::getMeshName(uint32 mesh) const
{
    return m_GroupData[mesh];
}

} } } //end namespace
