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
//Author:  Sebastiaan Sprengers
//Created: 18/10/2011

#ifndef _HE_SHAPE_2D_H_
#define _HE_SHAPE_2D_H_
#pragma once

#include "Polygon.h"
#include "mat44.h"
#include "happynew.h"
#include "ModelMesh.h"

namespace he {
namespace gui {

class Shape2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Shape2D() : m_Polygon(Polygon())
    {
    }

    virtual ~Shape2D()
    {
        
    }

    /* GETTERS */
    virtual Polygon& getPolygon()
    {
        return m_Polygon;
    }

    virtual const mat44& getWorldMatrix() const
    {
        return m_World;
    }

    virtual const gfx::ModelMesh::pointer& getMesh() const
    {
        return m_pMesh;
    }

    /* SETTERS */
    virtual void setMesh(const gfx::ModelMesh::pointer& pMesh)
    {
        m_pMesh = pMesh;
    }

    /* DEFAULT COPY & ASSIGNMENT */
    Shape2D(const Shape2D& s)
    {
        m_Polygon = s.m_Polygon;
        m_World = s.m_World;
        m_pMesh = s.m_pMesh;
    }

    Shape2D& operator=(const Shape2D& s)
    {
        m_Polygon = s.m_Polygon;
        m_World = s.m_World;
        m_pMesh = s.m_pMesh;

        return *this;
    }

protected:
    
    /* DATAMEMBERS */
    Polygon m_Polygon;
    mat44 m_World;
    gfx::ModelMesh::pointer m_pMesh;
};

} } //end namespace

#endif
