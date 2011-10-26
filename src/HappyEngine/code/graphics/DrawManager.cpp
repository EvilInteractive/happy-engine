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
#include "StdAfx.h" 

#include "DrawManager.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "Assert.h"
#include "GraphicsEngine.h"
#include "ShadowCaster.h"

namespace he {
namespace gfx {

DrawManager::DrawManager(): m_pShadowCaster(NEW ShadowCaster())
{
}


DrawManager::~DrawManager()
{
    delete m_pShadowCaster;
}

void DrawManager::begin(Type type, const Camera* pCamera)
{
    m_Type = type;
    m_pCamera = pCamera;
    m_DrawList.clear();
}
void DrawManager::end()
{
    if (m_Type == Type_Immediate)
        return;

    std::sort(m_DrawList.begin(), m_DrawList.end());
    std::for_each(m_DrawList.cbegin(), m_DrawList.cend(), [&](const DrawElement& e)
    {
        e.pDrawable->getMaterial().begin(e.pDrawable, m_pCamera);
        std::for_each(e.pDrawable->getModel()->cbegin(), e.pDrawable->getModel()->cend(), [&](const ModelMesh::pointer& m)
        {            
            GRAPHICS->draw(m);
        });
        e.pDrawable->getMaterial().end();
    });

}

bool DrawManager::viewClip(const shapes::Sphere& boundingSphere)
{
    ASSERT(m_pCamera != nullptr, "call begin first");
    vec3 vec(boundingSphere.getPosition() - m_pCamera->getPosition());
    float len(length(vec));
    if (len < boundingSphere.getRadius() == false) //if not in bounding sphere
    {
        if (len - boundingSphere.getRadius() > GRAPHICS->getSettings().getFogEnd()) //if bounding outside fog
        {
            return true;
        }
        else //check behind camera
        {
            vec /= len;
            return dot(normalize(m_pCamera->getLook()), vec) < 0;
        }
    }
    else
        return false;
}
void DrawManager::draw(const IDrawable* pDrawable)
{
    ASSERT(pDrawable != nullptr, "");
    if (pDrawable->getModel()->isComplete() == false)
        return;
    shapes::Sphere bS(pDrawable->getModel()->getMesh(0)->getBoundingSphere().getPosition() + pDrawable->getWorldMatrix().getTranslation(), 
                      pDrawable->getModel()->getMesh(0)->getBoundingSphere().getRadius() * pDrawable->getWorldMatrix()(0, 0)); // HACK: only uniform scales
    if (viewClip(bS) == false)
    {
        if (m_Type == Type_Immediate)
        {
            pDrawable->getMaterial().begin(pDrawable, m_pCamera);
            std::for_each(pDrawable->getModel()->cbegin(), pDrawable->getModel()->cend(), [&](const ModelMesh::pointer& m)
            {
                GRAPHICS->draw(m);
            });
            pDrawable->getMaterial().end();
        }
        else
        {
            DrawElement e;
            e.pDrawable = pDrawable;
            e.sorter = lengthSqr(m_pCamera->getPosition() - pDrawable->getWorldMatrix().getTranslation());
            if (m_Type == Type_BackToFront)
                e.sorter = FLT_MAX - e.sorter;
            m_DrawList.push_back(e);
        }
    }
}

void DrawManager::init(const DrawSettings& settings)
{
    m_pShadowCaster->init(settings);
}

void DrawManager::renderShadow()
{
    m_pShadowCaster->render(m_DrawList, m_pCamera, GRAPHICS->getLightManager()->getDirectionalLight());
}

} } //end namespace