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
//Author:  Bastian Damman
//Created: 14/01/2012

#ifndef _HE_SKYBOX_H_
#define _HE_SKYBOX_H_
#pragma once

#include "DefaultSingleDrawable.h"
#include "TextureCube.h"

namespace he {
namespace gfx {

class Texture2D;
class TextureCube;
class ModelMesh;
class Material;

class SkyBox : public DefaultSingleDrawable
{
public:
    SkyBox();
    virtual ~SkyBox();

    void load(const std::string& asset);
    void unload();

    virtual const Material* getMaterial() const { return m_Material; }
    virtual const ModelMesh* getModelMesh() const { return m_Cube; }

    virtual void calculateBound() {}

private:
    ModelMesh* m_Cube;
    const TextureCube* m_CubeMap;
    Material* m_Material;
    
    //Disable default copy constructor and default assignment operator
    SkyBox(const SkyBox&);
    SkyBox& operator=(const SkyBox&);


};

} } //end namespace

#endif
