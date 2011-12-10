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
//Created: 04/10/2011
#include "HappyPCH.h" 

#include "DrawManager.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "GraphicsEngine.h"
#include "ShadowCaster.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "InstancingManager.h"

namespace he {
namespace gfx {

DrawManager::DrawManager(): m_pShadowCaster(NEW ShadowCaster())
{
}


DrawManager::~DrawManager()
{
    delete m_pShadowCaster;
}

void DrawManager::draw()
{

    std::vector<DrawElement> culledDrawList;
    culledDrawList.reserve(m_DrawList.size());
    std::for_each(m_DrawList.cbegin(), m_DrawList.cend(), [&](const IDrawable* pDrawable)
    {
        if (pDrawable->isVisible() && pDrawable->getModel()->isLoaded())
        {
            shapes::Sphere bS(pDrawable->getModel()->getBoundingSphere().getPosition() + pDrawable->getWorldMatrix().getTranslation(), 
                              pDrawable->getModel()->getBoundingSphere().getRadius() * pDrawable->getWorldMatrix()(0, 0)); // HACK: only uniform scales

            if (viewClip(CAMERAMANAGER->getActiveCamera(), bS) == false)
            {
                DrawElement e;
                e.pDrawable = pDrawable;
                e.sorter = lengthSqr(CAMERAMANAGER->getActiveCamera()->getPosition() - e.pDrawable->getWorldMatrix().getTranslation());
                culledDrawList.push_back(e);
            }
        }
    });

    std::sort(culledDrawList.begin(), culledDrawList.end());
    std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](const DrawElement& e)
    {
        e.pDrawable->getMaterial().apply(e.pDrawable, CAMERAMANAGER->getActiveCamera());        
        GRAPHICS->draw(e.pDrawable->getModel());

    });
    GRAPHICS->getInstancingManager()->draw();
    
    renderShadow();
}
bool DrawManager::viewClip(const Camera* pCamera, const shapes::Sphere& boundingSphere)
{
    return viewClip(CAMERAMANAGER->getActiveCamera()->getPosition(), 
                    CAMERAMANAGER->getActiveCamera()->getLook(), 
                    CAMERAMANAGER->getActiveCamera()->getFarClip(), boundingSphere);
}
bool DrawManager::viewClip(const vec3& camPos, const vec3& camLook, float camFar, const shapes::Sphere& boundingSphere)
{
    vec3 vec(boundingSphere.getPosition() - camPos);
    float len(length(vec));
    if (len < boundingSphere.getRadius() == false) //if not in bounding sphere
    {
        if (len - boundingSphere.getRadius() > camFar) //if bounding outside clip
        {
            return true;
        }
        else //check behind camera
        {
            vec /= len;
            return dot(camLook, vec) < 0;
        }
    }
    else
        return false;
}

void DrawManager::init(const DrawSettings& settings)
{
    m_pShadowCaster->init(settings);
}

void DrawManager::renderShadow()
{
    m_pShadowCaster->render(m_DrawList, CAMERAMANAGER->getActiveCamera(), GRAPHICS->getLightManager()->getDirectionalLight());
}

void DrawManager::addDrawable( const IDrawable* pDrawable )
{
    ASSERT(pDrawable != nullptr, "adding a nullptr drawable");
    m_DrawList.push_back(pDrawable);
}

const std::vector<const IDrawable*>& DrawManager::getDrawList() const
{
    return m_DrawList;
}

} } //end namespace