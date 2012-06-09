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

class AutoExposure : public ge::ITickable
{
public:
    AutoExposure();
    virtual ~AutoExposure();

    void init(const RenderSettings& settings);

    void calculate(const Texture2D* pHdrMap );
    virtual void tick(float dTime);

    const Texture2D* getLuminanceMap() const;

private:

    Shader* m_LumShader;
    uint m_HDRmapPos;
    uint m_PrevLumMapPos;
    uint m_DTimePos;
    
    float m_DTime;
    float m_ExposureSpeed;

    Texture2D* m_pLumTexture[2]; //double buffered
    bool m_FirstBuffer;

    uint m_FboID;

    ModelMesh* m_pQuad;

    bool m_bOnce;

    //Disable default copy constructor and default assignment operator
    AutoExposure(const AutoExposure&);
    AutoExposure& operator=(const AutoExposure&);
};

} } //end namespace

#endif
