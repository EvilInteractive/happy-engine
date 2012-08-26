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
//Author:  Sebastiaan Sprengers
//Created: 25/04/2012

#include "HappyPCH.h" 

#include "Mesh2D.h"
#include "Polygon.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

#pragma warning(disable:4355) // use of this in initializer list
Mesh2D::Mesh2D() :  
    m_pPolygon(NEW Polygon()),
    m_WorldMatrix(mat44::Identity),
    m_ContextCreatedHandler(boost::bind(&Mesh2D::initVao, this, _1)),
    m_ContextRemovedHandler(boost::bind(&Mesh2D::destroyVao, this, _1))
{
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_IBOID);

    he_memset(m_VAOID, 0xffff, sizeof(VaoID) * MAX_VERTEX_ARRAY_OBJECTS);

    const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
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
    HE_IF_ASSERT(m_VAOID[context->id] == UINT_MAX, "vao already assigned on context")
    {
        glGenVertexArrays(1, m_VAOID + context->id);
        GL::heBindVao(m_VAOID[context->id]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }
}

void Mesh2D::destroyVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    HE_IF_ASSERT(m_VAOID[context->id] != UINT_MAX, "vao not alive on context")
    {
        glDeleteVertexArrays(1, m_VAOID + context->id);
        m_VAOID[context->id] = UINT_MAX;
    }
}

Mesh2D::~Mesh2D()
{
    GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
    const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
    {
        destroyVao(context);
    });

    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_IBOID);

    delete m_pPolygon;
}

/* GENERAL */
void Mesh2D::addVertex(const vec2& point)
{
    m_pPolygon->addPoint(point);
}

void Mesh2D::clear()
{
    m_pPolygon->clear();
}

bool Mesh2D::triangulate()
{
    return m_pPolygon->triangulate();
}

void Mesh2D::createBuffer(bool outline)
{
    if (outline == false)
    {
        if (m_pPolygon->triangulate() == false)
        {
            HE_ERROR("Failed to triangulate Mesh2D!");
        }
    }
    else
    {
        if (m_pPolygon->outline() == false)
        {
            HE_ERROR("Failed to outline Mesh2D!");
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    glBufferData(GL_ARRAY_BUFFER, m_pPolygon->getVertexCount() * 2 * sizeof(vec2), &m_pPolygon->getVertices()[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pPolygon->getIndexCount() * sizeof(uint), &m_pPolygon->getIndices()[0], GL_STREAM_DRAW);
}

/* GETTERS */
uint Mesh2D::getBufferID() const
{
    return m_VAOID[GL::s_CurrentContext->id];
}

const mat44& Mesh2D::getWorldMatrix() const
{
    return m_WorldMatrix;
}

const std::vector<vec2>& Mesh2D::getVertices() const
{
    return m_pPolygon->getVertices();
}

const std::vector<uint>& Mesh2D::getIndices() const
{
    return m_pPolygon->getIndices();
}

/* SETTERS */
void Mesh2D::setWorldMatrix(const mat44& mat)
{
    m_WorldMatrix = mat;
}


} } //end namespace