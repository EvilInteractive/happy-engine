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

#include "IDrawable.h"
#include "Material.h"
#include "TextureCube.h"

namespace he {
namespace gfx {

class Texture2D;
class ModelMesh;

class SkyBox : public ISingleDrawable
{
public:
    SkyBox();
    virtual ~SkyBox();

    void load(const std::string& asset);

    //////////////////////////////////////////////////////////////////////////
    /// ISingleDrawable
    /////////////////////////////////////////////////////////////////////////
    virtual const Material& getMaterial() const;
    virtual void applyMaterial(const ICamera* pCamera) const;
    virtual void applyMaterial(const Material& customMaterial, const ICamera* pCamera) const;

    virtual const ModelMesh* getModelMesh() const;

    virtual bool getCastsShadow() const { return false; }
    virtual void setCastsShadow(bool /*castShadow*/) { }

    virtual bool isVisible() const;
    virtual void setVisible(bool /*visible*/) { }

    virtual bool isInCamera(const ICamera* pCamera) const;
    virtual float getDrawPriority(const ICamera* /*pCamera*/) const { return 0; } 

    virtual void draw();
    virtual void drawShadow() {}

    virtual mat44 getWorldMatrix() const { return mat44::Identity; }

private:

    enum Face
    {
        Cube_Left,
        Cube_Right,

        Cube_Bottom,
        Cube_Top,

        Cube_Back,
        Cube_Front,
    };

    void faceLoaded();

    void unload();

    ModelMesh* m_pCube;
    TextureCube::pointer m_pCubeMap;
    const Texture2D* m_CubeFaces[6];
    Material m_Material;
    
    bool m_IsVisible;
    byte m_LoadedCount;

    //Disable default copy constructor and default assignment operator
    SkyBox(const SkyBox&);
    SkyBox& operator=(const SkyBox&);
};

} } //end namespace

#endif
