//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 18/07/2012
#include "HappyPCH.h" 

#include "CullOctree.h"
#include "IDrawable.h"
#include "Frustum.h"
#include "ICamera.h"
#include "Cone.h"

namespace he {
namespace gfx {

CullOctree::CullOctree( uint32 rootSize ):
    m_Root(NEW CullOctreeNode(nullptr, Bound(AABB(vec3(-(float)rootSize, -(float)rootSize, -(float)rootSize), 
                               vec3((float)rootSize, (float)rootSize, (float)rootSize)))))
{

}

CullOctree::~CullOctree()
{
    delete m_Root;
}

void CullOctree::insert( IDrawable* obj )
{
    HE_IF_ASSERT(obj->getNode() == nullptr, "Node already attached to tree")
    {
        HE_ASSERT(obj->getBound().getSphere().intersect(m_Root->getBound().getSphere()) == IntersectResult_Inside, "Obj not completely in root!");
        m_Root->insert(obj);
    }
}

void CullOctree::reevaluate( IDrawable* obj )
{
    HE_IF_ASSERT(obj->getNode() != nullptr, "Node is not attached to the tree")
    {
        obj->getNode()->reevaluate(obj);
    }
}

void CullOctree::remove( IDrawable* obj )
{
    HE_IF_ASSERT(obj->getNode() != nullptr, "Obj is not attached to a tree")
    {
        obj->getNode()->remove(obj);
    }
}

void CullOctree::draw( const ICamera* camera, boost::function1<void, IDrawable*> drawFunction ) const
{
    m_Root->draw(camera, drawFunction, true);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CullOctreeNode::CullOctreeNode(CullOctreeNode* parent, const Bound& bound): m_Bound(bound), m_Parent(parent) 
{
    he_memset(m_ChildNodes, 0, 8 * sizeof(CullOctreeNode*));
}
CullOctreeNode::~CullOctreeNode()
{
    for (uint i(0); i < 8; ++i)
    {
        delete m_ChildNodes[i];
    }
}
void CullOctreeNode::insert( IDrawable* drawable )
{
    if (m_ChildNodes[0] == nullptr)
        createChilds();
    if (m_ChildNodes[0] != nullptr)
    {
        for (uint i(0); i < 8; ++i)
        {
            IntersectResult result(m_ChildNodes[i]->getBound().intersect(drawable->getBound()));
            if (result == IntersectResult_Inside)
            {
                m_ChildNodes[i]->insert(drawable);
                return;
            }
            else if (result == IntersectResult_Intersecting)
            {
                break;
            }
            else if (result == IntersectResult_Outside)
            {
                continue;
            }
        }
    }
    drawable->setNode(this);
    m_ObjectChilds.push_back(drawable);
}
void CullOctreeNode::rinsert( IDrawable* drawable )
{
    IntersectResult result(m_Bound.intersect(drawable->getBound()));
    if (result == IntersectResult_Inside)
    {
        insert(drawable);
        return;
    }
    else
    {
        HE_IF_ASSERT(m_Parent != nullptr, "This is a big problem - node is not inside root")
            m_Parent->rinsert(drawable);
    }
}
void CullOctreeNode::reevaluate( IDrawable* drawable )
{
    IntersectResult result(m_Bound.intersect(drawable->getBound()));
    if (result == IntersectResult_Inside)
    {
        return; // OK - (we do not handle shrinking)
    }
    else
    {
        remove(drawable);
        HE_IF_ASSERT(m_Parent != nullptr, "This is a big problem - node is not inside root")
            m_Parent->rinsert(drawable);
    }

}
void CullOctreeNode::remove( IDrawable* obj )
{
    HE_IF_ASSERT(std::find(m_ObjectChilds.begin(), m_ObjectChilds.end(), obj) != m_ObjectChilds.cend(), "Obj is not attached to this node")
    {
        *std::find(m_ObjectChilds.begin(), m_ObjectChilds.end(), obj) = m_ObjectChilds.back();
        m_ObjectChilds.pop_back();
        obj->setNode(nullptr);
    }
}

void CullOctreeNode::createChilds()
{
    const vec3& tfl(m_Bound.getAABB().getTopFrontLeft());
    float currentSize(m_Bound.getAABB().getBottomBackRight().x - tfl.x);
    if (currentSize <= CullOctree::s_MinLeaveSize)
        return;

    vec3 newSize(currentSize / 2.0f, currentSize / 2.0f, currentSize / 2.0f);
    byte index(0);
    for (byte x(0); x < 2; ++x)
    for (byte y(0); y < 2; ++y)
    for (byte z(0); z < 2; ++z)
    {
        vec3 newTfl(tfl + vec3(x * newSize.x, y * newSize.y, z * newSize.z));
        m_ChildNodes[index++] = NEW CullOctreeNode(this,
            Bound(AABB(newTfl, newTfl + newSize)));
    }
}

void CullOctreeNode::draw( const ICamera* camera, boost::function1<void, IDrawable*> drawFunction, bool checkChilderen ) const
{
    const vec3& cameraPosition(camera->getPosition());
    
    if (checkChilderen == true)
    {
        // if camera is inside bound
        if ( m_Bound.getAABB().isOtherInside(cameraPosition) == false )
        {
            // sphere frustum test
            switch(camera->intersect(m_Bound))
            {
            case IntersectResult_Outside:
                return;
            case IntersectResult_Inside:
                checkChilderen = false; // completely in -> all childeren are in as well
            case IntersectResult_Intersecting:
                break;
            }
        }
    }

    if (m_ChildNodes[0] != nullptr) // if first child is not nullptr then all other are not nullptr as well
    {
        for (uint i(0); i < 8; ++i)
        {
            m_ChildNodes[i]->draw(camera, drawFunction, checkChilderen);
        }
    }
    std::for_each(m_ObjectChilds.cbegin(), m_ObjectChilds.cend(), drawFunction);
}

} } //end namespace
