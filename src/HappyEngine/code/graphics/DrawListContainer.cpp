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
#include "Material.h"
#include "CullOctree.h"
#include "ICamera.h"

namespace he {
namespace gfx {

DrawListContainer::DrawListContainer()
{
    for (uint blend(0); blend < BlendFilter_MAX; ++blend)
        m_DrawList[blend] = NEW CullOctree();
}


DrawListContainer::~DrawListContainer()
{
    for (uint blend(0); blend < BlendFilter_MAX; ++blend)
        delete m_DrawList[blend];
}


void DrawListContainer::getContainerIndex(const IDrawable* drawable, BlendFilter& blend)
{
    const gfx::Material* material(drawable->getMaterial());
    HE_IF_ASSERT(material != nullptr, "Material is nullptr!")
    {
        if (material->isBlended())
            blend = BlendFilter_Blend;
        else
            blend = BlendFilter_Opac;
    }
}
void DrawListContainer::insert( IDrawable* drawable, bool autoReevalute )
{
    BlendFilter blend;
    getContainerIndex(drawable, blend);
    m_DrawList[blend]->insert(drawable);
    if (autoReevalute)
        m_Dynamics.push_back(drawable);
}

void DrawListContainer::remove( IDrawable* drawable )
{
    BlendFilter blend;
    getContainerIndex(drawable, blend);  
    m_DrawList[blend]->remove(drawable);
    std::vector<IDrawable*>::iterator it(std::find(m_Dynamics.begin(), m_Dynamics.end(), drawable));
    if (it != m_Dynamics.cend())
    {
        (*it) = m_Dynamics.back();
        m_Dynamics.pop_back();
    }
}

void DrawListContainer::draw( BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_DrawList[blend]->draw(camera, drawFunc);
}
void DrawListContainer::drawAndCreateDebugMesh( BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc, std::vector<vec3>& vertices, std::vector<uint>& indices ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_DrawList[blend]->drawAndCreateDebugMesh(camera, drawFunc, vertices, indices);
}


void DrawListContainer::prepareForRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    std::for_each(m_Dynamics.cbegin(), m_Dynamics.cend(), [&](IDrawable* drawable)
    {
        if (drawable->isSleeping() == false)
        {
            BlendFilter blend;
            getContainerIndex(drawable, blend);
            drawable->calculateBound();
            m_DrawList[blend]->reevaluate(drawable);
        }
    });
}

void DrawListContainer::forceReevalute( IDrawable* drawable )
{
    BlendFilter blend;
    getContainerIndex(drawable, blend);  
    m_DrawList[blend]->reevaluate(drawable);
}

} } //end namespace
