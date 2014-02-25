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

class HAPPY_ENTRY Mesh2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    explicit Mesh2D(bool staticDraw = false);
    ~Mesh2D();

    /* GENERAL */
    void addVertex(const vec2& point);
    void clear();

    bool triangulate();

    void createBuffer(bool outline = false);

    /* GETTERS */
    inline uint32 getVBOID() const { return m_VertexVboID; }
    inline uint32 getVBOIndexID() const { return m_IndexVboID; }

    const he::PrimitiveList<vec2>& getVertices() const;
    const he::PrimitiveList<uint32>& getIndices() const;
    bool hasBuffer() const { return m_HasBuffer; }

    void draw();

private:
    /* DATAMEMBERS */
    Polygon* m_Polygon;
    uint32 m_VertexVboID;
    uint32 m_IndexVboID;

    bool m_StaticDraw : 4;
    bool m_HasBuffer : 4;

    /* DEFAULT COPY & ASSIGNMENT */
    Mesh2D(const Mesh2D&);
    Mesh2D& operator=(const Mesh2D&);
};

} } //end namespace

#endif
