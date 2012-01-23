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
//Author:  
//Created: //
#include "HappyPCH.h" 

#include "FxLoader.h"

#include "BinaryStream.h"

#include "fx/FxManager.h"
#include "fx/FxTimeLine.h"
#include "fx/FxTimeLineTrack.h"

#include "fx/FxParticleSystem.h"
#include "fx/FxCameraEffect.h"

#include "fx/IFxTimeLineTrackComponent.h"

#include "fx/FxCurve.h"
#include "fx/FxConstant.h"
#include "fx/FxRandom.h"

#include "fx/FxParticleColor.h"
#include "fx/FxParticleForce.h"
#include "fx/FxParticleLocation.h"
#include "fx/FxParticleRotation.h"
#include "fx/FxParticleRotationRate.h"
#include "fx/FxParticleScale.h"
#include "fx/FxParticleSpeed.h"
#include "fx/FxParticleLife.h"

#include "ContentManager.h"


namespace he {
namespace ct {

FxLoader::FxLoader()
{
}


FxLoader::~FxLoader()
{
}


void getVar(float& var, const io::BinaryStream& stream)
{
    var = stream.readFloat();
}
void getVar(vec2& var, const io::BinaryStream& stream)
{
    var = stream.readVector2();
}
void getVar(vec3& var, const io::BinaryStream& stream)
{
    var = stream.readVector3();
}
void getVar(vec4& var, const io::BinaryStream& stream)
{
    var = stream.readVector4();
}

template<typename T>
typename gfx::FxCurve<T>::pointer getCurveVariable(const io::BinaryStream& stream)
{
    gfx::FxCurve<T>::pointer curve(NEW gfx::FxCurve<T>());
    uint points(stream.readDword());
    for (uint point(0); point < points; ++point)
    {
        float time(stream.readFloat());
        T value;
        getVar(value, stream);
        curve->addPoint(time, value);
    }
    curve->finalize();
    return curve;
}

gfx::ParticleModifyComponentType getPsMCT(int type)
{
    switch (type)
    {
        case 0: return gfx::PMCT_Color; break;
        case 1: return gfx::PMCT_Force; break;
        case 2: break;
        case 3: return gfx::PMCT_Rotation; break;
        case 4: return gfx::PMCT_RotationRate; break;
        case 5: return gfx::PMCT_Scale; break;
        case 6: return gfx::PMCT_Speed; break;
    }
    ASSERT(false, "unknown PMCT type");
    return gfx::PMCT_Color;
}
gfx::ParticleInitComponentType getPsICT(int type)
{
    switch (type)
    {
        case 0: return gfx::PICT_Color; break;
        case 1: break;
        case 2: return gfx::PICT_Location; break;
        case 3: return gfx::PICT_Rotation; break;
        case 4: break;
        case 5: return gfx::PICT_Scale; break;
        case 6: return gfx::PICT_Speed; break;
    }
    ASSERT(false, "unknown PICT type");
    return gfx::PICT_Color;
}

void addPSBehaviours(gfx::IFxTimeLineTrackComponent* pComp, const io::BinaryStream& stream, uint count)
{
    using namespace gfx;

    FxParticleSystem* pPS(dynamic_cast<FxParticleSystem*>(pComp));
    pPS->setMaxParticles(500);
    pPS->setSpawnRate(gfx::IFxVariable<float>::pointer(NEW gfx::FxConstant<float>(100)));
    for (uint i(0); i < count; ++i)
    {        
        int compType(stream.readInt32());

        if (compType == 7) //required
        {
            uint varType(stream.readDword()); //texture
            Material material(CONTENT->loadMaterial("particles/particles.material"));
            gfx::ShaderUserVar<gfx::Texture2D::pointer>::pointer texture(boost::dynamic_pointer_cast<gfx::ShaderUserVar<gfx::Texture2D::pointer>>(material.getVar("diffuseMap")));           
            texture->setData(CONTENT->asyncLoadTexture(stream.readString()));
            pPS->setMaterial(material);

            varType = stream.readDword(); //uvtiles
            /*int xTiles(*/stream.readInt32()/*)*/; 
            /*int yTiles(*/stream.readInt32()/*)*/;
            //pPS->setTiles(FxConstant<vec2>::pointer(NEW FxConstant<vec2>(vec2((float)xTiles, (float)yTiles))));

            varType = stream.readDword(); //uvtile
            if (varType == 1) //random
            {
                /*int xMin(*/stream.readInt32()/*)*/;
                /*int yMin(*/stream.readInt32()/*)*/;
                /*int xMax(*/stream.readInt32()/*)*/;
                /*int yMax(*/stream.readInt32()/*)*/;
                //pPS->setEndTime
            }
            else if (varType == 2) //const
            {
                /*int xTile(*/stream.readInt32()/*)*/;
                /*int yTile(*/stream.readInt32()/*)*/;

            }

            varType = stream.readDword(); //lifetime           
            if (varType == 1) //random
            {
                float min(stream.readFloat());
                float max(stream.readFloat());
                pPS->getInitComponent<FxParticleLife>(pPS->addInitComponent(PICT_Life))->setValue(
                    FxRandomFloat::pointer(NEW FxRandomFloat(min, max)));
            }
            else if (varType == 2) //const
            {
                float life(stream.readFloat());
                pPS->getInitComponent<FxParticleLife>(pPS->addInitComponent(PICT_Life))->setValue(
                    FxConstant<float>::pointer(NEW FxConstant<float>(life)));
            }
        }
        else
        {
            uint varType(stream.readDword());
            if (varType == 0) //mod  //HACK
            {
                ParticleModifyComponentType pmct(getPsMCT(compType));
                uint component(pPS->addModifyComponent(pmct));            
                switch (pmct)
                {
                case PMCT_Color: 
                    {
                        FxParticleColor* color(pPS->getModifyComponent<FxParticleColor>(component));
                        color->setValue(getCurveVariable<vec4>(stream));
                        break;
                    }
                case PMCT_Force: 
                    {
                        FxParticleForce* force(pPS->getModifyComponent<FxParticleForce>(component));
                        force->setValue(getCurveVariable<vec3>(stream));
                        break;
                    }
                case PMCT_Rotation: 
                    {
                        FxParticleRotation* rot(pPS->getModifyComponent<FxParticleRotation>(component));
                        rot->setValue(getCurveVariable<float>(stream));
                        break;
                    }
                case PMCT_RotationRate: 
                    {
                        FxParticleRotationRate* rot(pPS->getModifyComponent<FxParticleRotationRate>(component));
                        rot->setValue(getCurveVariable<float>(stream));
                        break;
                    }
                case PMCT_Scale: 
                    {
                        FxParticleScale* scale(pPS->getModifyComponent<FxParticleScale>(component));
                        scale->setValue(getCurveVariable<vec3>(stream));
                        break;
                    }
                case PMCT_Speed: 
                    {
                        FxParticleSpeed* speed(pPS->getModifyComponent<FxParticleSpeed>(component));
                        speed->setValue(getCurveVariable<vec3>(stream));
                        break;
                    }
                }           
            }
            else //init
            {

            }
        }
    }
}

uint FxLoader::load( const std::string& asset )
{
    using namespace gfx;

    uint timeLineId(FX->createTimeline());
    FxTimeLine* pTimeLine(FX->getTimeline(timeLineId));   
    io::BinaryStream stream(asset, io::BinaryStream::Read);
    pTimeLine->setEndTime(stream.readFloat() * 2);
    uint tracks(stream.readDword());

    for (uint track(0); track < tracks; ++track)
    {
        uint trackId(pTimeLine->addTrack());
        FxTimeLineTrack* pTrack(pTimeLine->getTrack(trackId));

        uint componentCount(stream.readDword());
        for (uint component(0); component < componentCount; ++component)
        {
            FxType compType(static_cast<FxType>(stream.readDword()));
            float startTime(stream.readFloat());
            float endTime(stream.readFloat());
            uint behaviours(stream.readDword());

            uint compId(pTrack->addComponent(compType));

            IFxTimeLineTrackComponent* pComp(pTrack->getComponent<IFxTimeLineTrackComponent>(compId));
            pComp->setStartTime(startTime);
            pComp->setEndTime(endTime);

            switch (compType)
            {
                case FxType_ParticleSystem:
                    {
                        addPSBehaviours(pComp, stream, behaviours);
                        break;
                    }
                case FxType_CameraEffect:
                    {
                        break;
                    }
                case FxType_AudioEffect:
                    {
                        break;
                    }
                default:
                    ASSERT(false, "Unknown compType: " + compType);
            }
        }
    }

    return timeLineId;
}

} } //end namespace