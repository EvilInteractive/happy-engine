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

#include "Model.h"

#include "GL/glew.h"
#include <algorithm>
#include "Assert.h"

namespace happyengine {
namespace graphics {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(): m_NumVertices(0)
{
}


Model::~Model()
{
    
}

void Model::setVertices(void* pVertices, unsigned int num, const VertexLayout& vertexLayout)
{
    m_NumVertices = num;

    glGenVertexArrays(1, &m_VaoID[0]);
    glBindVertexArray(m_VaoID[0]);
    glEnableVertexAttribArray(m_VaoID[0]);

    VertexLayout::layout elements(vertexLayout.getElements());

    glGenBuffers(1, m_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID[0]);
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

            default: ASSERT(false, "unkown size type"); break;
        }
        glVertexAttribPointer(e.getElementIndex(), components, type, GL_FALSE, vertexLayout.getVertexSize(), 
            BUFFER_OFFSET(e.getByteOffset())); 
    });

    //unbind
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

unsigned int Model::getVertexArraysID() const
{
    return m_VaoID[0];
}

unsigned int Model::getNumVertices() const
{
    return m_NumVertices;
}


} } //end namespace