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
//Author:  Bastian Damman
//Created: 27/11/2011
#include "HappyPCH.h" 

#include "FxCameraEffect.h"
#include "HappyNew.h"

#include "FxConstant.h"

#include "HappyEngine.h"
#include "CameraManager.h"
#include "Random.h"

#include "Camera.h"

namespace he {
namespace gfx {

FxCameraEffect::FxCameraEffect(): m_ShakeEnabled(false), m_pShakeIntensity(NEW FxConstant<vec3>()), m_Random()
{
}


FxCameraEffect::~FxCameraEffect()
{
}

void FxCameraEffect::start()
{

}
void FxCameraEffect::stop()
{
    //reset shake
    if (CAMERAMANAGER->getActiveCamera() != nullptr) 
        CAMERAMANAGER->getActiveCamera()->setFxPositionOffset(vec3::zero);
}

void FxCameraEffect::tick( float normTime, float /*dTime*/ )
{
    if (m_ShakeEnabled && CAMERAMANAGER->getActiveCamera() != nullptr)
    {
        //random direction
        //mult by intensity
        vec3 shake(normalize(vec3(m_Random.nextFloat(0, 1), m_Random.nextFloat(0, 1), m_Random.nextFloat(0, 1))));
        shake *= m_pShakeIntensity->getValue(normTime);
        CAMERAMANAGER->getActiveCamera()->setFxPositionOffset(shake);
    }
}

void FxCameraEffect::setShakeIntensity( const IFxVariable<vec3>::pointer& pVar )
{
    m_pShakeIntensity = pVar;
}

void FxCameraEffect::toggleShake( bool enable )
{
    m_ShakeEnabled = enable;
}


} } //end namespace