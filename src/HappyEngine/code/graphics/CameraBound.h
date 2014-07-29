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
//Created: 22/07/2012

#ifndef _HE_CameraBound_H_
#define _HE_CameraBound_H_
#pragma once

#include "Cone.h"
#include "Sphere.h"
#include "Frustum.h"

namespace he {
namespace gfx {
class CameraPerspective;
class CameraOrtho;
class CameraBoundOrtho
{
public:
    inline const Sphere& getSphere() const { return m_Sphere; }
    inline const Frustum& getFrustum() const { return m_Frustum; }

    void calculate(const CameraOrtho* cam);

private:
    Sphere m_Sphere;
    Frustum m_Frustum;
};

} } //end namespace

#endif
