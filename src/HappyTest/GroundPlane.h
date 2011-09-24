//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _GROUND_PLANE_H_
#define _GROUND_PLANE_H_
#pragma once

#include "DeferredPreEffect.h"
#include "I3DRenderer.h"
#include "Camera.h"
#include "ModelMesh.h"
#include "Texture2D.h"

namespace happytest {

class GroundPlane
{
public:
	GroundPlane();
    virtual ~GroundPlane();

    void load();
    void draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, const happyengine::graphics::Camera* pCamera);

private:
    
    happyengine::graphics::ModelMesh::pointer m_pModel;
    happyengine::graphics::Texture2D::pointer m_pDiffuseMap;
    happyengine::graphics::Texture2D::pointer m_pNormalMap;
    happyengine::graphics::Texture2D::pointer m_pSGIMap;

    //Disable default copy constructor and default assignment operator
    GroundPlane(const GroundPlane&);
    GroundPlane& operator=(const GroundPlane&);
};

} //end namespace

#endif
