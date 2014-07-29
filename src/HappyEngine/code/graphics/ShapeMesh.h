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
//Author:  Bastian Damman
//Created: 29/07/2014

#ifndef _HE_SHAPEMESH_H_
#define _HE_SHAPEMESH_H_
#pragma once

#include "VertexLayout.h"
#include "MeshEnums.h"

namespace he {
class Polygon;
namespace gfx {
class ModelMesh;

class HAPPY_ENTRY ShapeMesh
{
    static VertexLayout s_VertexLayout;
public:
    /* CONSTRUCTOR - DESTRUCTOR */
    ShapeMesh();
    ~ShapeMesh();

    /* GENERAL */
    void init(const MeshDrawMode mode);
    void draw() const;

    /* EDIT */

    // \brief  start the editing of the polygon
    void beginEditing();

    // \brief  adds a points the the polygon
    // \param point  the new point in the polygon
    // \param startNewLine  false if the new points connects to the previous pointS
    void addPoint(const vec3& point, const bool startNewLine = false);

    // \brief  call this if you are done editing
    // \param close  should the last point connect to the first point
    // \param keepBuffer  keeps the internal point buffer, else it will be deleted to save memory
    void endEditing(const bool close, const bool keepBuffer); 

    /* GETTERS */
    uint32 getVBO() const;
    uint32 getIBO() const;
    static const VertexLayout& getVertexLayout() { return s_VertexLayout; }
    
    /* SDM */
    void sdmInit();
    void sdmDestroy();

private:
    /* DATAMEMBERS */
    ModelMesh* m_InternalMesh;

    struct InternalData;
    InternalData* m_Points;

    /* DEFAULT COPY & ASSIGNMENT */
    ShapeMesh(const ShapeMesh&);
    ShapeMesh& operator=(const ShapeMesh&);
};

} } //end namespace

#endif
