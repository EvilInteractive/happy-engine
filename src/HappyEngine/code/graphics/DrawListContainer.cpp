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
//Created: 17/12/2011
#include "HappyPCH.h"

#include "DrawListContainer.h"
#include "IDrawable.h"

namespace he {
namespace gfx {

DrawListContainer::DrawListContainer()
{
}


DrawListContainer::~DrawListContainer()
{
}


void gfx::DrawListContainer::insert( IDrawable* pDrawable )
{
    #if !GCC
    uint i0(pDrawable->getMaterial().noPost()?(pDrawable->getMaterial().isBackground()?BACKGROUND_INDEX:AFTERPOST_INDEX):BEFOREPOST_INDEX);
    uint i1(pDrawable->getMaterial().isBlended()?BLENDING_INDEX:OPAC_INDEX);
    uint i2(pDrawable->isInstanced()?INSTANCE_INDEX:(pDrawable->isSkinned()?SKINNED_INDEX:SINGLE_INDEX));
    #else
    uint i0(0);
    uint i1(0);
    uint i2(0);

    if (pDrawable->getMaterial().noPost())
        if (pDrawable->getMaterial().isBackground())
            i0 = BACKGROUND_INDEX;
        else
            i0 = AFTERPOST_INDEX;
    else
        i0 = BEFOREPOST_INDEX;

    if (pDrawable->getMaterial().isBlended())
        i1 = BLENDING_INDEX;
    else
        i1 = OPAC_INDEX;

    if (pDrawable->isInstanced())
        i2 = INSTANCE_INDEX;
    else if (pDrawable->isSkinned())
        i2 = SKINNED_INDEX;
    else
        i2 = SINGLE_INDEX;
    #endif
    m_DrawList[i0][i1][i2].push_back(pDrawable);
}
void removeFromVector( DrawListContainer::Container& vec, const IDrawable* pDrawable )
{
    for (uint i(0); i < vec.size(); ++i)
    {
        if (vec[i] == pDrawable)
        {
            std::swap(vec[i], vec[vec.size()-1]);
            break;
        }
    }
    vec.pop_back();
}

void gfx::DrawListContainer::remove( const IDrawable* pDrawable )
{
    #if !GCC
    uint i0(pDrawable->getMaterial().noPost()?(pDrawable->getMaterial().isBackground()?BACKGROUND_INDEX:AFTERPOST_INDEX):BEFOREPOST_INDEX);
    uint i1(pDrawable->getMaterial().isBlended()?BLENDING_INDEX:OPAC_INDEX);
    uint i2(pDrawable->isInstanced()?INSTANCE_INDEX:(pDrawable->isSkinned()?SKINNED_INDEX:SINGLE_INDEX));
    #else
    uint i0(0);
    uint i1(0);
    uint i2(0);

    if (pDrawable->getMaterial().noPost())
        if (pDrawable->getMaterial().isBackground())
            i0 = BACKGROUND_INDEX;
        else
            i0 = AFTERPOST_INDEX;
    else
        i0 = BEFOREPOST_INDEX;

    if (pDrawable->getMaterial().isBlended())
        i1 = BLENDING_INDEX;
    else
        i1 = OPAC_INDEX;

    if (pDrawable->isInstanced())
        i2 = INSTANCE_INDEX;
    else if (pDrawable->isSkinned())
        i2 = SKINNED_INDEX;
    else
        i2 = SINGLE_INDEX;
    #endif
    removeFromVector(m_DrawList[i0][i1][i2], pDrawable);
}

void gfx::DrawListContainer::for_each( uint filter, const boost::function<void(IDrawable*)>& f ) const
{
    #pragma region ASSERTS
    HE_ASSERT(filter & F_Loc_BeforePost || filter & F_Loc_AfterPost || filter & F_Loc_Background,
        "flag at least one Location filter: F_Loc_BeforePost or F_Loc_AfterPost");
    HE_ASSERT(filter & F_Sub_Single     || filter & F_Sub_Skinned     || filter & F_Sub_Instanced,
        "flag at least one sub filter: F_Sub_Single, F_Sub_Skinned or F_Sub_Instanced");
    #pragma endregion

    for (int i0(0); i0 < MAX_I0; ++i0)
    {
        if ((filter & F_Loc_BeforePost && BEFOREPOST_INDEX == i0 ||
             filter & F_Loc_AfterPost  && AFTERPOST_INDEX  == i0 || 
             filter & F_Loc_Background && BACKGROUND_INDEX == i0) == false)
            continue;

        for (int i1(0); i1 < MAX_I1; ++i1)
        {
            if ((filter & F_Main_Opac        && OPAC_INDEX      == i1 ||
                 filter & F_Main_Blended     && BLENDING_INDEX  == i1) == false)
                continue;

            for (int i2(0); i2 < MAX_I2; ++i2)
            {
                if ((filter & F_Sub_Single    && SINGLE_INDEX   == i2 ||
                     filter & F_Sub_Skinned   && SKINNED_INDEX  == i2 ||
                     filter & F_Sub_Instanced && INSTANCE_INDEX == i2) == false)
                    continue;

                std::for_each(m_DrawList[i0][i1][i2].cbegin(), m_DrawList[i0][i1][i2].cend(), f);
            }
        }
    }
}

} } //end namespace
