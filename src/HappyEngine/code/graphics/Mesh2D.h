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

#ifndef _HE_MESH2D_H_
#define _HE_MESH2D_H_
#pragma once

namespace he {
class Polygon;
namespace gfx {

class Mesh2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Mesh2D();
    virtual ~Mesh2D();

    /* GENERAL */
    void addVertex(const vec2& point);
    void clear();

    bool triangulate();

    void createBuffer(bool outline = false);

    /* GETTERS */
    uint getBufferID() const;
    const mat44& getWorldMatrix() const;
    const std::vector<vec2>& getVertices() const;
    const std::vector<uint>& getIndices() const;

    /* SETTERS */
    void setWorldMatrix(const mat44& mat);

private:
    void initVao(GLContext* context);
    void destroyVao(GLContext* context);

    /* DATAMEMBERS */
    Polygon* m_pPolygon;
    mat44 m_WorldMatrix;
    uint m_VBOID;
    uint m_IBOID;
    VaoID m_VAOID[MAX_VERTEX_ARRAY_OBJECTS];

    eventCallback1<void, GLContext*> m_ContextCreatedHandler;
    eventCallback1<void, GLContext*> m_ContextRemovedHandler;

    /* DEFAULT COPY & ASSIGNMENT */
    Mesh2D(const Mesh2D&);
    Mesh2D& operator=(const Mesh2D&);
};

} } //end namespace

#endif
