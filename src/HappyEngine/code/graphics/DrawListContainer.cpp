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
    uint i0(pDrawable->getMaterial().noPost()?AFTERPOST_INDEX:BEFOREPOST_INDEX);
    uint i1(pDrawable->getMaterial().isBlended()?BLENDING_INDEX:OPAC_INDEX);
    uint i2(pDrawable->isInstanced()?INSTANCE_INDEX:(pDrawable->isSkinned()?SKINNED_INDEX:SINGLE_INDEX));
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
    uint i0(pDrawable->getMaterial().noPost()?AFTERPOST_INDEX:BEFOREPOST_INDEX);
    uint i1(pDrawable->getMaterial().isBlended()?BLENDING_INDEX:OPAC_INDEX);
    uint i2(pDrawable->isInstanced()?INSTANCE_INDEX:(pDrawable->isSkinned()?SKINNED_INDEX:SINGLE_INDEX));
    removeFromVector(m_DrawList[i0][i1][i2], pDrawable);
}

void gfx::DrawListContainer::for_each( uint filter, const boost::function<void(IDrawable*)>& f ) const
{
    #pragma region ASSERTS
    ASSERT(filter & F_Loc_BeforePost || filter & F_Loc_AfterPost,                               
        "flag at least one Location filter: F_Loc_BeforePost or F_Loc_AfterPost");
    ASSERT(filter & F_Sub_Single     || filter & F_Sub_Skinned     || filter & F_Sub_Instanced, 
        "flag at least one sub filter: F_Sub_Single, F_Sub_Skinned or F_Sub_Instanced");
    #pragma endregion

    for (int i0(0); i0 < MAX_I0; ++i0)
    {
        if ((filter & F_Loc_BeforePost && BEFOREPOST_INDEX == i0 || 
             filter & F_Loc_AfterPost  && AFTERPOST_INDEX  == i0) == false)
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