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
#include "StdAfx.h" 

#include "BinObjLoader.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "BinaryStream.h"
#include "FileNotFoundException.h"

#include "Assert.h"

namespace he {
namespace ct {
namespace models {

BinObjLoader::BinObjLoader(): m_NumIndices(0)
{
}


BinObjLoader::~BinObjLoader()
{
    std::for_each(m_Vertices.begin(), m_Vertices.end(), [&](void* pVert)
    {
        free(pVert);
    });
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        free(pInd);
    });
}
void BinObjLoader::load(const std::string& path, const gfx::VertexLayout& vertLayout, bool allowByteIndices)
{
    read(path, allowByteIndices);
    
    std::for_each(m_Vertices.begin(), m_Vertices.end(), [&](void* pVert)
    {
        free(pVert);
    });
    m_Vertices.clear();

    for (uint i = 0; i < m_VertexData.size(); ++i)
    {
        void* pVert(malloc(vertLayout.getVertexSize() * m_VertexData[i].size()));
        ASSERT(pVert != nullptr, "not enough memory!");
        m_Vertices.push_back(pVert);
    }
    
    fill(vertLayout);
}

void BinObjLoader::read(const std::string& path, bool allowByteIndices)
{
    //Clean
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        free(pInd);
    });
    m_Indices.clear();
    m_VertexData.clear();

    using namespace std;

    io::BinaryStream stream(path, io::BinaryStream::Read); //throws err::FileNotFoundException
    
    uint meshes(stream.readDword());

    for(uint i = 0; i < meshes; ++i)
    {
        m_MeshName.push_back(stream.readString());

        uint numVertices(stream.readDword());
        m_VertexData.push_back(std::vector<InternalVertex>(numVertices));
        stream.readBuffer(&m_VertexData[m_VertexData.size() - 1][0], numVertices * sizeof(InternalVertex));

        m_NumIndices.push_back(stream.readDword());
        gfx::IndexStride stride(static_cast<gfx::IndexStride>(stream.readByte()));
        if (stride == gfx::IndexStride_Byte && allowByteIndices == false)
            m_IndexStride.push_back(gfx::IndexStride_UShort);
        else
            m_IndexStride.push_back(stride);
        
        void* pInd = malloc(stride * m_NumIndices.back());
        ASSERT(pInd != nullptr, "not enough memory!");
        stream.readBuffer(pInd, stride * m_NumIndices.back());
        m_Indices.push_back(pInd);
    }
}

void BinObjLoader::fill(const gfx::VertexLayout& vertLayout) const
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
    
    for (uint i = 0; i < m_VertexData.size(); ++i)
    {
        //optimazation for struct == internal struct
        if (sizeof(InternalVertex) == vertLayout.getVertexSize())
        {
            if (pOff == 0 && tOff == 12 && nOff == 20 && tanOff == 32)
            {
                memcpy(m_Vertices[i], &m_VertexData[i][0], m_VertexData[i].size() * vertLayout.getVertexSize());
                break;
            }
        }

        char* pCharData = static_cast<char*>(m_Vertices[i]);
        uint count = 0;
        std::for_each(m_VertexData[i].cbegin(), m_VertexData[i].cend(), [&](const InternalVertex& vert)
        {
            if (pOff != -1)
            {
                *reinterpret_cast<vec3*>(&pCharData[count * vertLayout.getVertexSize() + pOff]) = vert.pos;
            }
            if (tOff != -1)
            {
                *reinterpret_cast<vec2*>(&pCharData[count * vertLayout.getVertexSize() + tOff]) = vert.tex;
            }
            if (nOff != -1)
            {
                *reinterpret_cast<vec3*>(&pCharData[count * vertLayout.getVertexSize() + nOff]) = vert.norm;
            }
            if (tanOff != -1)
            {
                *reinterpret_cast<vec3*>(&pCharData[count * vertLayout.getVertexSize() + tanOff]) = vert.tan;
            }
            ++count;
        });
    }
}

const void* BinObjLoader::getVertices(uint mesh) const
{
    return m_Vertices[mesh];
}
const void* BinObjLoader::getIndices(uint mesh) const 
{
    return m_Indices[mesh];
}
gfx::IndexStride BinObjLoader::getIndexStride(uint mesh) const
{
    return m_IndexStride[mesh];
}
uint BinObjLoader::getNumVertices(uint mesh) const
{
    return m_VertexData[mesh].size();
}
uint BinObjLoader::getNumIndices(uint mesh) const
{
    return m_NumIndices[mesh];
}

uint BinObjLoader::getNumMeshes() const
{
    return m_MeshName.size();
}
const std::string& BinObjLoader::getMeshName(uint mesh) const
{
    return m_MeshName[mesh];
}

} } } //end namespace