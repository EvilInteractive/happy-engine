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
//Author:  Sebastiaan Sprengers
//Created: 25/04/2012

#include "HappyPCH.h" 

#include "Mesh2D.h"
#include "Polygon.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

#pragma warning(disable:4355) // use of this in initializer list
Mesh2D::Mesh2D(bool staticDraw) :  
    m_Polygon(NEW Polygon()),
    m_WorldMatrix(mat44::Identity),
    m_ContextCreatedHandler(boost::bind(&Mesh2D::initVao, this, _1)),
    m_ContextRemovedHandler(boost::bind(&Mesh2D::destroyVao, this, _1)),
    m_StaticDraw(staticDraw),
    m_HasBuffer(false)
{
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_IBOID);

    he_memset(m_VAOID, 0xffffffff, sizeof(VaoID) * MAX_VERTEX_ARRAY_OBJECTS);

    const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
    contexts.forEach([&](GLContext* context)
    {
        initVao(context);
    });

    GRAPHICS->ContextCreated += m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved += m_ContextRemovedHandler;
}
#pragma warning(default:4355)

void Mesh2D::initVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_VAOID[contextID] == UINT32_MAX, "vao already assigned on context")
    {
        glGenVertexArrays(1, m_VAOID + contextID);
        GL::heBindVao(m_VAOID[contextID]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }
}

void Mesh2D::destroyVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_VAOID[contextID] != UINT32_MAX, "vao not alive on context")
    {
        glDeleteVertexArrays(1, m_VAOID + contextID);
        m_VAOID[contextID] = UINT32_MAX;
    }
}

Mesh2D::~Mesh2D()
{
    GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
    const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
    contexts.forEach([&](GLContext* context)
    {
        destroyVao(context);
    });

    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_IBOID);

    delete m_Polygon;
}

/* GENERAL */
void Mesh2D::addVertex(const vec2& point)
{
    m_Polygon->addPoint(point);
    m_HasBuffer = false;
}

void Mesh2D::clear()
{
    m_Polygon->clear();
    m_HasBuffer = false;
}

bool Mesh2D::triangulate()
{
    return m_Polygon->triangulate();
}

void Mesh2D::createBuffer(bool outline)
{
    if (outline == false)
    {
        if (m_Polygon->triangulate() == false)
        {
            HE_ERROR("Failed to triangulate Mesh2D!");
        }
    }
    else
    {
        if (m_Polygon->outline() == false)
        {
            HE_ERROR("Failed to outline Mesh2D!");
        }
    }

    GL::heBindVao(getBufferID());

    GLenum drawType(m_StaticDraw == true ? GL_STATIC_DRAW : GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    glBufferData(GL_ARRAY_BUFFER, m_Polygon->getVertexCount() * sizeof(vec2), &m_Polygon->getVertices()[0], drawType);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Polygon->getIndexCount() * sizeof(uint32), &m_Polygon->getIndices()[0], drawType);

    m_HasBuffer = true;
}

/* GETTERS */
uint32 Mesh2D::getBufferID() const
{
    return m_VAOID[GL::s_CurrentContext->getID()];
}

const mat44& Mesh2D::getWorldMatrix() const
{
    return m_WorldMatrix;
}

const he::PrimitiveList<vec2>& Mesh2D::getVertices() const
{
    return m_Polygon->getVertices();
}

const he::PrimitiveList<uint32>& Mesh2D::getIndices() const
{
    return m_Polygon->getIndices();
}

/* SETTERS */
void Mesh2D::setWorldMatrix(const mat44& mat)
{
    m_WorldMatrix = mat;
}

} } //end namespace