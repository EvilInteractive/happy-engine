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
//Created: 15/10/2011

#ifndef _HE_POLYGON_H_
#define _HE_POLYGON_H_
#pragma once

namespace he {

class Polygon
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Polygon();
    virtual ~Polygon();

    /* GENERAL */
    void addPoint(const vec2& p);
    bool outline();
    bool triangulate();
    void clear();

    /* GETTERS */
    const he::PrimitiveList<vec2>& getVertices() const;
    const he::PrimitiveList<uint32>& getIndices() const;

    bool isTriangulated() const;
    bool hitTest(const vec2& hitPoint) const;

    float getArea() const;

    uint32 getVertexCount() const;
    uint32 getIndexCount() const;
    uint32 getTriangleCount() const;

    /* OPERATORS */
    bool operator==(const Polygon& p) const;
    bool operator!=(const Polygon& p) const;

    /* DEFAULT COPY & ASSIGNMENT */
    Polygon(const Polygon& p);
    Polygon& operator=(const Polygon& p);

private:

    /* DATAMEMBERS */
    he::PrimitiveList<vec2> m_Vertices;
    he::PrimitiveList<uint32> m_Indices;
};

} //end namespace

#endif
