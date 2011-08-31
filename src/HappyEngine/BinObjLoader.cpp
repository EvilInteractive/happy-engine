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
//Created: 28/08/2011

#include "BinObjLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "BinaryStream.h"
#include "FileNotFoundException.h"

#include "Assert.h"

namespace happyengine {
namespace content {
namespace models {

BinObjLoader::BinObjLoader(): m_NumIndices(0), m_NumVertices(0), m_Vertices(nullptr), m_Indices(nullptr)
{
}


BinObjLoader::~BinObjLoader()
{
    free(m_Vertices);
    free(m_Indices);
}
void BinObjLoader::load(const std::string& path, const graphics::VertexLayout& vertLayout, bool allowByteIndices)
{
    read(path, allowByteIndices);

    free(m_Vertices);
    m_Vertices = malloc(vertLayout.getVertexSize() * m_NumVertices);
    std::cout << "malloc " << vertLayout.getVertexSize() * m_NumVertices << " bytes\n";
    ASSERT(m_Vertices != nullptr, "not enough memory!");
        
    fill(m_Vertices, vertLayout);
}

void BinObjLoader::read(const std::string& path, bool allowByteIndices)
{
    //Clean
    m_VertexData.clear();

    using namespace std;

    io::BinaryStream stream(path, io::BinaryStream::Read); //throws error::FileNotFoundException
    
    m_NumVertices = stream.readDword();
    m_VertexData.resize(m_NumVertices);
    stream.readBuffer(&m_VertexData[0], m_NumVertices * sizeof(InternalVertex));

    m_NumIndices = stream.readDword();
    m_IndexStride = static_cast<graphics::IndexStride>(stream.readByte());
    if (m_IndexStride == graphics::IndexStride_Byte && allowByteIndices == false)
        m_IndexStride = graphics::IndexStride_UShort;
    free(m_Indices);
    m_Indices = malloc(m_IndexStride * m_NumIndices);
    ASSERT(m_Indices != nullptr, "not enough memory!");
    stream.readBuffer(m_Indices, m_IndexStride * m_NumIndices);
}

void BinObjLoader::fill(void* pVertexData, const graphics::VertexLayout& vertLayout) const
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;
    int tanOff = -1;

    std::for_each(vertLayout.getElements().cbegin(), vertLayout.getElements().cend(), [&](const graphics::VertexElement& element)
    {
        if (element.getUsage() == graphics::VertexElement::Usage_Position)
            pOff = element.getByteOffset();
        else if (element.getUsage() == graphics::VertexElement::Usage_TextureCoordinate)
            tOff = element.getByteOffset(); 
        else if (element.getUsage() == graphics::VertexElement::Usage_Normal)
            nOff = element.getByteOffset();
        else if (element.getUsage() == graphics::VertexElement::Usage_Tangent)
            tanOff = element.getByteOffset();
    });
    
    //optimazation for struct == internal struct
    if (sizeof(InternalVertex) == vertLayout.getVertexSize())
    {
        if (pOff == 0 && tOff == 12 && nOff == 20 && tanOff == 32)
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
        if (tanOff != -1)
        {
            *reinterpret_cast<math::Vector3*>(&pCharData[count * vertLayout.getVertexSize() + tanOff]) = vert.tan;
            bytecount += 12;
        }
        ++count;
    });
    std::cout << "wrote " << bytecount << " bytes\n";
}

const void* BinObjLoader::getVertices() const
{
    return m_Vertices;
}
const void* BinObjLoader::getIndices() const 
{
    return m_Indices;
}
graphics::IndexStride BinObjLoader::getIndexStride() const
{
    return m_IndexStride;
}
uint BinObjLoader::getNumVertices() const
{
    return m_NumVertices;
}
uint BinObjLoader::getNumIndices() const
{
    return m_NumIndices;
}

} } } //end namespace