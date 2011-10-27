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
//Author:  Bastian Damman
//Created: 15/09/2011 (copy of the previous Model class)
#include "StdAfx.h" 

#include "ModelMesh.h"

#include "OpenGL.h"
#include <algorithm>
#include "Assert.h"
#include "Color.h"
#include "ExternalError.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ModelMesh::ModelMesh(const std::string& name): m_NumVertices(0), m_NumIndices(0), m_Complete(false), m_Name(name)
{
}


ModelMesh::~ModelMesh()
{
    glDeleteVertexArrays(1, m_VaoID);
    glDeleteVertexArrays(1, m_VaoShadowID);
    glDeleteBuffers(1, m_VertexVboID);
    glDeleteBuffers(1, m_VertexVboShadowID);
    glDeleteBuffers(1, m_IndexVboID);
}

void ModelMesh::init()
{
    glGenVertexArrays(1, &m_VaoID[0]);
    glGenVertexArrays(1, &m_VaoShadowID[0]);
}

//Calling glBufferData with a NULL pointer before uploading new data can improve performance (tells the driver you don't care about the old cts)
void ModelMesh::setVertices(const void* pVertices, uint num, const VertexLayout& vertexLayout)
{
    //err::glCheckForErrors(false);

    ASSERT(m_NumVertices == 0, "you can only set the vertices once, use DynamicModelMesh instead");
    m_NumVertices = num;

    uint posOffset = 0;
    std::for_each(vertexLayout.getElements().cbegin(), vertexLayout.getElements().cend(), [&posOffset](const VertexElement& e)
    {
        if (e.getUsage() == gfx::VertexElement::Usage_Position)
        {
            posOffset = e.getByteOffset();
            return;
        }
    });
    m_BoundingSphere = shapes::Sphere::getBoundingSphere(pVertices, num, vertexLayout.getVertexSize(), posOffset);

    GL::heBindVao(m_VaoID[0]);
    //err::glCheckForErrors();

    VertexLayout::layout elements(vertexLayout.getElements());

    glGenBuffers(1, m_VertexVboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexLayout.getVertexSize() * num, pVertices, GL_STATIC_DRAW);

    std::for_each(elements.cbegin(), elements.cend(), [&](const VertexElement& e)
    {
        GLint components = 1;
        GLenum type = 0;
        switch (e.getType())
        {
            case VertexElement::Type_Vector2: type = GL_FLOAT; components = 2; break;
            case VertexElement::Type_Vector3: type = GL_FLOAT; components = 3; break;
            case VertexElement::Type_Vector4: type = GL_FLOAT; components = 4; break;
            case VertexElement::Type_Float: type = GL_FLOAT; break;
            case VertexElement::Type_Double: type = GL_DOUBLE; break;

            case VertexElement::Type_Int: type = GL_INT; break;
            case VertexElement::Type_UInt: type = GL_UNSIGNED_INT; break;
                
            case VertexElement::Type_Short: type = GL_SHORT; break;
            case VertexElement::Type_UShort: type = GL_UNSIGNED_SHORT; break;

            case VertexElement::Type_Byte: type = GL_BYTE; break;
            case VertexElement::Type_UByte: type = GL_UNSIGNED_BYTE; break;
            
            #pragma warning(disable:4127)
            default: ASSERT(false, "unknown type"); break;
            #pragma warning(default:4127)
        }
        glVertexAttribPointer(e.getElementIndex(), components, type, GL_FALSE, vertexLayout.getVertexSize(), 
            BUFFER_OFFSET(e.getByteOffset())); 
        glEnableVertexAttribArray(e.getElementIndex());
        //err::glCheckForErrors();
    });



    //////////////////////////////////////////////////////////////////////////
    ///                             Shadow                                 ///
    //////////////////////////////////////////////////////////////////////////
    std::vector<vec3> shadowVertices(num);
    const char* charPointCloud = static_cast<const char*>(pVertices);
    for(uint i = 0; i < num; ++i)
    {
        const vec3& p(*reinterpret_cast<const vec3*>(charPointCloud + vertexLayout.getVertexSize() * i + posOffset));
        shadowVertices[i] = p;
    }

    GL::heBindVao(m_VaoShadowID[0]);
    glGenBuffers(1, m_VertexVboShadowID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboShadowID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num, &shadowVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), BUFFER_OFFSET(0)); 
    glEnableVertexAttribArray(0);


    //unbind
    GL::heBindVao(0);

    if (m_NumIndices > 0)
        m_Complete = true;
}
void ModelMesh::setIndices(const void* pIndices, uint num, IndexStride type)
{
    ASSERT(m_NumIndices == 0, "you can only set the indices once, use DynamicModelMesh instead");
    m_NumIndices = num;
    
    GL::heBindVao(m_VaoID[0]);
    glGenBuffers(1, m_IndexVboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, GL_STATIC_DRAW);

    GL::heBindVao(m_VaoShadowID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, GL_STATIC_DRAW);

    switch (type)
    {
        case IndexStride_Byte: m_IndexType = GL_UNSIGNED_BYTE; break;
        case IndexStride_UShort: m_IndexType = GL_UNSIGNED_SHORT; break;
        case IndexStride_UInt: m_IndexType = GL_UNSIGNED_INT; break;
        default: ASSERT("unkown type"); break;
    }

    if (m_NumVertices > 0)
        m_Complete = true;
    //unbind
    GL::heBindVao(0);
}

const std::string& ModelMesh::getName() const
{
    return m_Name;
}

uint ModelMesh::getVertexArraysID() const
{
    return m_VaoID[0];
}

uint ModelMesh::getNumVertices() const
{
    return m_NumVertices;
}
uint ModelMesh::getNumIndices() const
{
    return m_NumIndices;
}

uint ModelMesh::getIndexType() const
{
    return m_IndexType;
}

bool ModelMesh::isComplete() const
{
    return m_Complete;
}

const shapes::Sphere& ModelMesh::getBoundingSphere() const
{
    return m_BoundingSphere;
}

he::uint ModelMesh::getVertexShadowArraysID() const
{
    return m_VaoShadowID[0];
}


} } //end namespace