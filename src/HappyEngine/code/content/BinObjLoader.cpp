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
//Author: Bastian Damman
//Created: 28/08/2011
#include "HappyPCH.h" 

#include "BinObjLoader.h"

#include "BinaryFileVisitor.h"
#include "VertexLayout.h"
#include "Model.h"
#include "Bone.h"
#include "ModelMesh.h"

namespace he {
namespace ct {
namespace models {

BinObjLoader::BinObjLoader()
{
}


BinObjLoader::~BinObjLoader()
{
    m_BoneData.forEach([&](he::ObjectList<gfx::Bone>* bones)
    {
        delete bones;
    });
    m_VertexData.forEach([&](he::ObjectList<InternalVertex>* vertexData)
    {
        delete vertexData;
    });
    m_Vertices.forEach([&](void* pVert)
    {
        he_free(pVert);
    });
    m_Indices.forEach([&](void* pInd)
    {
        he_free(pInd);
    });
}
bool BinObjLoader::load(const he::String& path, bool allowByteIndices)
{
    if (read(path, allowByteIndices) == false)
        return false;

    m_Vertices.forEach([&](void* pVert)
    {
        he_free(pVert);
    });
    m_Vertices.clear();

    for (uint32 i = 0; i < m_VertexData.size(); ++i)
    {
        void* vert(he_malloc(m_VertexLayout.getSize() * m_VertexData[i]->size()));
        HE_ASSERT(vert != nullptr, "not enough memory!");
        m_Vertices.add(vert);
    }
    
    fill();

    return true;
}

bool BinObjLoader::read(const he::String& path, bool allowByteIndices)
{
    //Clean
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        he_free(pInd);
    });
    m_Indices.clear();
    m_BoneData.forEach([&](he::ObjectList<gfx::Bone>* bones)
    {
        delete bones;
    });
    m_BoneData.clear();
    m_VertexData.forEach([&](he::ObjectList<InternalVertex>* vertexData)
    {
        delete vertexData;
    });
    m_VertexData.clear();

    using namespace std;

    io::BinaryFileVisitor stream;
    if (stream.openRead(path) == false)
        return false;
    
    uint32 meshes(0);
    stream.visit(meshes);

    he::String name;
    for(uint32 i = 0; i < meshes; ++i)
    {
        stream.visit(name);
        m_MeshName.add(name);

        //////////////////////////////////////////////////////////////////////////
        ///                             Bones                                  ///
        //////////////////////////////////////////////////////////////////////////
        uint8 numBones(0);
        stream.visit(numBones);
        m_BoneData.add(NEW he::ObjectList<gfx::Bone>());
        m_BoneData.back()->reserve(numBones);
        for (uint8 iBone = 0; iBone < numBones; ++iBone)
        {
            gfx::Bone bone;
            stream.visit(name);
            bone.m_Name = name.clone();
            mat44 matrix;
            stream.visit(matrix);
            bone.m_BaseTransform = matrix;
            m_BoneData.back()->add(bone);
        }

        //////////////////////////////////////////////////////////////////////////
        ///                             Vertices                               ///
        //////////////////////////////////////////////////////////////////////////
        uint32 numVertices(0);
        stream.visit(numVertices);
        m_VertexData.add(NEW he::ObjectList<InternalVertex>((size_t)numVertices));
        m_VertexData.back()->resize(numVertices);
        stream.visitBlob(&(m_VertexData.back()->front()), numVertices * sizeof(InternalVertex));

        //////////////////////////////////////////////////////////////////////////
        ///                             Indices                                ///
        //////////////////////////////////////////////////////////////////////////
        uint32 numIndices(0);
        stream.visit(numIndices);
        m_NumIndices.add(numIndices);
        gfx::IndexStride stride(gfx::IndexStride_Byte);
        stream.visitEnum<gfx::IndexStride, uint8>(stride);
        if (stride == gfx::IndexStride_Byte && allowByteIndices == false)
            m_IndexStride.add(gfx::IndexStride_UShort);
        else
            m_IndexStride.add(stride);
        
        void* pInd = he_malloc(stride * m_NumIndices.back());
        HE_ASSERT(pInd != nullptr, "not enough memory!");
        stream.visitBlob(pInd, stride * m_NumIndices.back());
        m_Indices.add(pInd);
    }
    stream.close();

    HE_ASSERT(m_VertexLayout.getSize() == 0, "vertexlayout not empty!");
    m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_Position, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_3, 0));
    m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_TextureCoordiante, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_2, 12));
    m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_Normal, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_3, 20));
    m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_Tangent, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_3, 32));
    if (m_BoneData.size() > 0)
    {
        m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_BoneIndices, gfx::eShaderAttributeType_Int32, gfx::eShaderAttributeTypeComponents_4, 44));
        m_VertexLayout.addElement(gfx::VertexElement(gfx::eShaderAttribute_BoneWeights, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_4, 44 + 4 * gfx::Bone::MAX_BONEWEIGHTS));
    }

    return true;
}

void BinObjLoader::fill()
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;
    int tanOff = -1;
    int boneOff = -1;
    int weightOff = -1;

    std::for_each(m_VertexLayout.getElements().cbegin(), m_VertexLayout.getElements().cend(), [&](const gfx::VertexElement& element)
    {
        if (element.getAttribute() == gfx::eShaderAttribute_Position)
            pOff = element.getByteOffset();
        else if (element.getAttribute() == gfx::eShaderAttribute_TextureCoordiante)
            tOff = element.getByteOffset(); 
        else if (element.getAttribute() == gfx::eShaderAttribute_Normal)
            nOff = element.getByteOffset();
        else if (element.getAttribute() == gfx::eShaderAttribute_Tangent)
            tanOff = element.getByteOffset();
        else if (element.getAttribute() == gfx::eShaderAttribute_BoneIndices)
            boneOff = element.getByteOffset();
        else if (element.getAttribute() == gfx::eShaderAttribute_BoneWeights)
            weightOff = element.getByteOffset();
    });
    
    for (uint32 i = 0; i < m_VertexData.size(); ++i)
    {
        //optimazation for struct == internal struct
        if (sizeof(InternalVertex) == m_VertexLayout.getSize())
        {
            if (pOff == 0 && tOff == 12 && nOff == 20 && tanOff == 32 && boneOff == 44 && weightOff == 44 + gfx::Bone::MAX_BONES * 1)
            {
                he_memcpy(m_Vertices[i], &m_VertexData[i]->front(), m_VertexData[i]->size() * m_VertexLayout.getSize());
                break;
            }
        } 

        char* charData = static_cast<char*>(m_Vertices[i]);
        uint32 count(0);
        m_VertexData[i]->forEach([&](const InternalVertex& vert)
        {
            if (pOff != -1)
            {
                he_memcpy(&charData[count * m_VertexLayout.getSize() + pOff], &vert.pos, sizeof(vec3));
            }
            if (tOff != -1)
            {
                he_memcpy(&charData[count * m_VertexLayout.getSize() + tOff], &vert.tex, sizeof(vec2));
            }
            if (nOff != -1)
            {
                he_memcpy(&charData[count * m_VertexLayout.getSize() + nOff], &vert.norm, sizeof(vec3));
            }
            if (tanOff != -1)
            {
                he_memcpy(&charData[count * m_VertexLayout.getSize() + tanOff], &vert.tan, sizeof(vec3));
            }
            if (boneOff != -1)
            {
                HE_COMPILE_ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "Unsupported max boneWeight value only 4 is supported");
                vec4 boneIDs(vert.boneID[0], vert.boneID[1], vert.boneID[2], vert.boneID[3]);
                he_memcpy(&charData[count * m_VertexLayout.getSize() + boneOff], &boneIDs, sizeof(vec4));
            }
            if (weightOff != -1)
            {
                he_memcpy(&charData[count * m_VertexLayout.getSize() + weightOff], vert.boneWeight, sizeof(float) * gfx::Bone::MAX_BONEWEIGHTS);
            }
            ++count;
        });
    }
}

const void* BinObjLoader::getVertices(uint32 mesh) const
{
    return m_Vertices[mesh];
}
const void* BinObjLoader::getIndices(uint32 mesh) const 
{
    return m_Indices[mesh];
}
gfx::IndexStride BinObjLoader::getIndexStride(uint32 mesh) const
{
    return m_IndexStride[mesh];
}
size_t BinObjLoader::getNumVertices(uint32 mesh) const
{
    return m_VertexData[mesh]->size();
}
size_t BinObjLoader::getNumIndices(uint32 mesh) const
{
    return m_NumIndices[mesh];
}

size_t BinObjLoader::getNumMeshes() const
{
    return m_MeshName.size();
}
const he::String& BinObjLoader::getMeshName(uint32 mesh) const
{
    return m_MeshName[mesh];
}

const he::ObjectList<gfx::Bone>& BinObjLoader::getBones( uint32 mesh ) const
{
    HE_ASSERT(mesh < m_BoneData.size(), "mesh outside array");
    return *m_BoneData[mesh];
}

} } } //end namespace
