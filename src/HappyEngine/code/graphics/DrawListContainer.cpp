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
#ifdef USE_OCTREE
#include "CullOctree.h"
#endif
#include "ICamera.h"

namespace he {
namespace gfx {

DrawListContainer::DrawListContainer()
{
#ifdef USE_OCTREE
    for (uint32 blend(0); blend < BlendFilter_MAX; ++blend)
        m_DrawList[blend] = NEW CullOctree();
#endif
}


DrawListContainer::~DrawListContainer()
{
#ifdef USE_OCTREE
    for (uint32 blend(0); blend < BlendFilter_MAX; ++blend)
        delete m_DrawList[blend];
#else
    for (uint32 blend(0); blend < BlendFilter_MAX; ++blend)
        HE_ASSERT(m_DrawList[blend].empty() == true, "Drawlist not empty @shutdown");
#endif
}


void DrawListContainer::getContainerIndex(const IDrawable* drawable, BlendFilter& blend)
{
    const gfx::Material* material(drawable->getMaterial());
    HE_IF_ASSERT(material != nullptr, "Material is nullptr!")
    {
        if (material->isBlended() || material->noPost())
            blend = BlendFilter_Blend;
        else
            blend = BlendFilter_Opac;
    }
}
void DrawListContainer::insert( IDrawable* drawable )
{
    BlendFilter blend;
    getContainerIndex(drawable, blend);
#ifdef USE_OCTREE
    m_DrawList[blend]->insert(drawable);
#else
    HE_IF_ASSERT(std::find(m_DrawList[blend].begin(), m_DrawList[blend].end(), drawable) == m_DrawList[blend].end(), "Drawable already attached")
    m_DrawList[blend].push_back(drawable);
#endif
}

void DrawListContainer::remove( IDrawable* drawable )
{
    BlendFilter blend;
    getContainerIndex(drawable, blend);  
#ifdef USE_OCTREE
    m_DrawList[blend]->remove(drawable);
#else
    std::vector<IDrawable*>::iterator it(std::find(m_DrawList[blend].begin(), m_DrawList[blend].end(), drawable));
    if (it != m_DrawList[blend].cend())
    {
        (*it) = m_DrawList[blend].back();
        m_DrawList[blend].pop_back();
    }
#endif
    std::vector<IDrawable*>::iterator dynIt(std::find(m_Dynamics.begin(), m_Dynamics.end(), drawable));
    if (dynIt != m_Dynamics.cend())
    {
        (*dynIt) = m_Dynamics.back();
        m_Dynamics.pop_back();
    }
}

void DrawListContainer::draw( BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
#ifdef USE_OCTREE
    m_DrawList[blend]->draw(camera, drawFunc);
#else
    std::for_each(m_DrawList[blend].cbegin(), m_DrawList[blend].cend(), [camera, drawFunc](IDrawable* drawable)
    {
        if (camera->intersect(drawable->getBound()) != IntersectResult_Outside)
            drawFunc(drawable);
    });
#endif
}
void DrawListContainer::drawAndCreateDebugMesh( BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc, std::vector<vec3>& vertices, std::vector<uint32>& indices ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
#ifdef USE_OCTREE
    m_DrawList[blend]->drawAndCreateDebugMesh(camera, drawFunc, vertices, indices);
#else
    vertices; indices; camera;
    draw(blend, camera, drawFunc);
#endif
}


void DrawListContainer::prepareForRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_Dynamics.empty() == false)
    {
        std::for_each(m_Dynamics.cbegin(), m_Dynamics.cend(), [&](IDrawable* drawable)
        {
            BlendFilter blend;
            getContainerIndex(drawable, blend);
            drawable->calculateBound();
#ifdef USE_OCTREE
            m_DrawList[blend]->reevaluate(drawable);
#endif
            drawable->nodeReevaluated();
        });
        m_Dynamics.clear();
    }
}

void DrawListContainer::forceReevalute( IDrawable* drawable )
{
#ifdef USE_OCTREE
    BlendFilter blend;
    getContainerIndex(drawable, blend);  
    m_DrawList[blend]->reevaluate(drawable);
#else
    drawable;
#endif
}

void DrawListContainer::doReevalute( IDrawable* drawable )
{
    m_Dynamics.push_back(drawable);
}

} } //end namespace
