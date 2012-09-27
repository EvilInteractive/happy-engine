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

const float CullOctree::s_MinLeaveSize = 1.0f;

CullOctree::CullOctree( ):
    m_Root(NEW CullOctreeNode())
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
        m_Root = m_Root->rootInsert(obj);
    }
}

void CullOctree::reevaluate( IDrawable* obj )
{
    HE_IF_ASSERT(obj->getNode() != nullptr, "Node is not attached to the tree")
    {
        obj->getNode()->reevaluate(obj);
        m_Root = m_Root->getRoot();
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

void CullOctree::drawAndCreateDebugMesh( const ICamera* camera, boost::function1<void, IDrawable*> drawFunction, std::vector<vec3>& vertices, std::vector<uint>& indices ) const
{
    m_Root->drawAndCreateDebugMesh(camera, drawFunction, true, vertices, indices);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CullOctreeNode::CullOctreeNode()
    : m_Parent(nullptr), m_IsLeafe(false)
{
    float strictSize(CullOctree::s_MinLeaveSize * 128); // needs to be power of two
    vec3 pos(strictSize / -2.0f);

    createBounds(pos, strictSize);

    he_memset(m_ChildNodes, 0, 8 * sizeof(CullOctreeNode*));
}
CullOctreeNode::CullOctreeNode(CullOctreeNode* parent, byte xIndex, byte yIndex, byte zIndex)
    : m_Parent(parent), m_IsLeafe(false)
{
    const AABB& parentAABB(m_Parent->m_StrictBound.getAABB());
    const vec3& tfl(parentAABB.getTopFrontLeft());

    float looseSize(parentAABB.getBottomBackRight().x - tfl.x);
    float strictSize = looseSize / 2.0f;
    if (strictSize <= CullOctree::s_MinLeaveSize)
        m_IsLeafe = true;

    vec3 pos(tfl);
    pos.x += strictSize * xIndex;
    pos.y += strictSize * yIndex;
    pos.z += strictSize * zIndex;

    createBounds(pos, strictSize);

    he_memset(m_ChildNodes, 0, 8 * sizeof(CullOctreeNode*));
}

CullOctreeNode::CullOctreeNode( const vec3& pos, float strictSize, CullOctreeNode* child, byte xIndex, byte yIndex, byte zIndex )
    : m_Parent(nullptr), m_IsLeafe(false)
{
    createBounds(pos, strictSize);
    createChilds(child, xIndex, yIndex, zIndex);
}

void CullOctreeNode::createBounds( const vec3& pos, float strictSize )
{
    AABB aabb(pos, pos + vec3(strictSize));
    m_StrictBound.fromAABB(aabb);

    aabb.setTopFrontLeft(aabb.getTopFrontLeft() - vec3(strictSize / 2.0f));
    aabb.setBottomBackRight(aabb.getBottomBackRight() + vec3(strictSize / 2.0f));
    m_LooseBound.fromAABB(aabb);
}

CullOctreeNode::~CullOctreeNode()
{
    for (uint i(0); i < 8; ++i)
    {
        delete m_ChildNodes[i];
    }
}
CullOctreeNode* CullOctreeNode::rootInsert( IDrawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
        insert(drawable);
        return this;
    }
    else
    {
        const AABB& strictAABB(m_StrictBound.getAABB());
        float parentSize((strictAABB.getBottomBackRight().x - strictAABB.getTopFrontLeft().x) * 2.0f);
        vec3 directiontHint(drawable->getBound().getSphere().getPosition() - m_StrictBound.getSphere().getPosition());
        vec3 parentPosition(strictAABB.getTopFrontLeft());
        parentPosition.x -= directiontHint.x < 0? parentSize / 2.0f : 0;
        parentPosition.y -= directiontHint.y < 0? parentSize / 2.0f : 0;
        parentPosition.z -= directiontHint.z < 0? parentSize / 2.0f : 0;
        CullOctreeNode* newRoot(NEW CullOctreeNode(parentPosition, parentSize, this, 
            directiontHint.x < 0? 1 : 0, directiontHint.y < 0? 1 : 0, directiontHint.z < 0? 1 : 0));
        m_Parent = newRoot;
        return newRoot->rootInsert(drawable);
    }
}
void CullOctreeNode::insert( IDrawable* drawable )
{
    if (m_ChildNodes[0] == nullptr)
        createChilds();
    if (m_ChildNodes[0] != nullptr)
    {
        const Bound& drawableBound(drawable->getBound());

        // An object is inside the loosebound when the center point is in the strictbound 
        // and the size of the object's bound is smaller or equal to the strictbound's size
        // since all child's have same size, checking the first one is enough
        if (drawableBound.getSphere().getRadius() <= m_ChildNodes[0]->getStrictBound().getSphere().getRadius())
        {
            for (uint i(0); i < 8; ++i)
            {
                const Bound& strictBound(m_ChildNodes[i]->getStrictBound());
                const Bound& looseBound(m_ChildNodes[i]->getLooseBound());
                if (strictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()) == false)
                    continue; // maybe it is in an other child
                HE_ASSERT(looseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
                m_ChildNodes[i]->insert(drawable);
                return;
            }
            HE_ASSERT(false, "Object fits loosebound but is not in one of the childs!");
        }
    }
    if (m_Parent == nullptr)
    {

    }
    drawable->setNode(this);
    m_ObjectChilds.push_back(drawable);
}
void CullOctreeNode::rinsert( IDrawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
        insert(drawable);
        return;
    }
    else
    {
        if (m_Parent != nullptr)
            m_Parent->rinsert(drawable);
        else
            rootInsert(drawable);
    }
}
void CullOctreeNode::reevaluate( IDrawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
        return; // OK - (we do not handle shrinking)
    }
    else
    {
        remove(drawable);
        if (m_Parent != nullptr)
            m_Parent->rinsert(drawable);
        else
            rootInsert(drawable);
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
    if (m_IsLeafe)
        return;
    byte index(0);
    for (byte x(0); x < 2; ++x)
    for (byte y(0); y < 2; ++y)
    for (byte z(0); z < 2; ++z)
    {
        m_ChildNodes[index++] = NEW CullOctreeNode(this, x, y, z);
    }
}

void CullOctreeNode::createChilds( CullOctreeNode* child, byte xIndex, byte yIndex, byte zIndex )
{
    byte index(0);
    for (byte x(0); x < 2; ++x)
    for (byte y(0); y < 2; ++y)
    for (byte z(0); z < 2; ++z)
    {
        if (x != xIndex || y != yIndex || z != zIndex)
            m_ChildNodes[index++] = NEW CullOctreeNode(this, x, y, z);
        else
            m_ChildNodes[index++] = child;
    }
}

void CullOctreeNode::draw( const ICamera* camera, boost::function1<void, IDrawable*> drawFunction, bool checkChilderen ) const
{
    const vec3& cameraPosition(camera->getPosition());
    
    if (checkChilderen == true)
    {
        // if camera is inside bound
        if ( m_LooseBound.getAABB().isOtherInside(cameraPosition) == false )
        {
            // sphere frustum test
            switch(camera->intersect(m_LooseBound))
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

bool CullOctreeNode::drawAndCreateDebugMesh( const ICamera* camera, boost::function1<void, IDrawable*> drawFunction, bool checkChilderen, 
    std::vector<vec3>& vertices, std::vector<uint>& indices ) const
{
    const vec3& cameraPosition(camera->getPosition());

    if (checkChilderen == true)
    {
        // if camera is inside bound
        if ( m_LooseBound.getAABB().isOtherInside(cameraPosition) == false )
        {
            // sphere frustum test
            switch(camera->intersect(m_LooseBound))
            {
            case IntersectResult_Outside:
                return false;
            case IntersectResult_Inside:
                checkChilderen = false; // completely in -> all childeren are in as well
            case IntersectResult_Intersecting:
                break;
            }
        }
    }

    bool hasChilds(m_ObjectChilds.size() > 0);
    if (m_ChildNodes[0] != nullptr) // if first child is not nullptr then all other are not nullptr as well
    {
        for (uint i(0); i < 8; ++i)
        {
            hasChilds |= m_ChildNodes[i]->drawAndCreateDebugMesh(camera, drawFunction, checkChilderen, vertices, indices);
        }
    }
    if (hasChilds)
    {
        m_StrictBound.getAABB().generateIndices(indices, vertices.size());
        m_StrictBound.getAABB().generateVertices(vertices);
        std::for_each(m_ObjectChilds.cbegin(), m_ObjectChilds.cend(), drawFunction);
    }
    return hasChilds;
}

CullOctreeNode* CullOctreeNode::getRoot()
{
    if (m_Parent == nullptr)
        return this;
    else 
        return m_Parent->getRoot();
}

} } //end namespace
