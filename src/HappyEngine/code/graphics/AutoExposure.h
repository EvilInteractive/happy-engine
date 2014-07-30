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
//Created: 26/10/2011

#ifndef _HE_AUTO_EXPOSURE_H_
#define _HE_AUTO_EXPOSURE_H_
#pragma once

#include "Shader.h"
#include "ITickable.h"
#include "RenderSettings.h"

namespace he {
namespace gfx {
class Texture2D;
class ModelMesh;
class RenderTarget;
class MaterialInstance;

class AutoExposure : public ge::ITickable
{
public:
    AutoExposure();
    virtual ~AutoExposure();

    void init(const PostSettings::HdrSettings& settings);

    void calculate(const Texture2D* hdrMap );
    void tick(float dTime) override;

    const Texture2D* getLuminanceMap() const;

private:

    MaterialInstance* m_LumMaterial;

    int8 m_MaterialparamHDRmapIndex;
    int8 m_MaterialparamPrevLumMapIndex;
    int8 m_MaterialparamDTimeIndex;
    bool m_FirstBuffer : 1;
    bool m_IsInitialized : 1;
    
    float m_ExposureSpeed;

    Texture2D* m_LumTexture[2]; //double buffered

    RenderTarget* m_RenderTarget;
    ModelMesh* m_Quad;


    //Disable default copy constructor and default assignment operator
    AutoExposure(const AutoExposure&);
    AutoExposure& operator=(const AutoExposure&);
};

} } //end namespace

#endif
