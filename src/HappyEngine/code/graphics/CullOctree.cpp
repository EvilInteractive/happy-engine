//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifdef HE_USE_OCTREE
#include "CullOctree.h"
#include "Drawable.h"
#include "Frustum.h"
#include "ICamera.h"
#include "Cone.h"

#include "CullOctreeNodeFactory.h"

namespace he {
namespace gfx {

//#define OCTREE_PARANOID

const float CullOctree::s_MinLeaveSize = 8.0f;

CullOctree::CullOctree( ):
    m_Root(CullOctreeNodeFactory::getInstance()->getNode())
{

}

CullOctree::~CullOctree()
{
    m_Root->doRemoveChilderen(false);
    CullOctreeNodeFactory::getInstance()->releaseNode(m_Root);
}

void CullOctree::insert( Drawable* obj )
{
    HE_IF_ASSERT(obj->getNode() == nullptr, "Node already attached to tree")
    {
        m_Root = m_Root->rootInsert(obj);
    }
}

void CullOctree::reevaluate( Drawable* obj )
{
    HE_IF_ASSERT(obj->getNode() != nullptr, "Node is not attached to the tree")
    {
        obj->getNode()->reevaluate(obj);
        m_Root = m_Root->getRoot();
    }
}

void CullOctree::remove( Drawable* obj )
{
    CullOctreeNode* node(obj->getNode());
    HE_IF_ASSERT(node != nullptr, "Obj is not attached to a tree")
    {
        obj->getNode()->remove(obj);
        if (node->checkRemove())
        {
            node->doRemoveChilderen(true);
        }
    }
}

void CullOctree::draw( const ICamera* camera, std::function<void(Drawable*)> drawFunction ) const
{
    m_Root->draw(camera, drawFunction, true);
}

void CullOctree::drawAndCreateDebugMesh( const ICamera* camera, std::function<void(Drawable*)> drawFunction, he::PrimitiveList<vec3>& vertices, he::PrimitiveList<uint32>& indices ) const
{
    m_Root->drawAndCreateDebugMesh(camera, drawFunction, true, vertices, indices);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CullOctreeNode::create(uint32 poolIndex)
{
    m_PoolIndex = poolIndex;
    reset();

    float strictSize(CullOctree::s_MinLeaveSize * 128); // needs to be power of two
    vec3 pos(strictSize / -2.0f);

    createBounds(pos, strictSize);

    m_ObjectChildsCapacity = 0;
    m_NumObjectChilds = 0;
    m_ObjectChilds = nullptr;

    he_memset(m_ChildNodes, 0, 8 * sizeof(CullOctreeNode*));
}

void CullOctreeNode::reset()
{
    HE_ASSERT(m_NumObjectChilds == 0, "Object leak in octree!");
    m_Parent = nullptr;
    m_IsLeafe = false;
    m_NumObjectChilds = 0;
}

void CullOctreeNode::init(CullOctreeNode* parent, uint8 xIndex, uint8 yIndex, uint8 zIndex)
{
    m_Parent = parent;
    m_IsLeafe = false;

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

void CullOctreeNode::init( const vec3& pos, float strictSize, CullOctreeNode* child, uint8 xIndex, uint8 yIndex, uint8 zIndex )
{
    m_Parent = nullptr;
    m_IsLeafe = false;
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

CullOctreeNode* CullOctreeNode::rootInsert( Drawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
#ifdef OCTREE_PARANOID
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
#endif
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
        CullOctreeNode* newRoot(CullOctreeNodeFactory::getInstance()->getNode());
        newRoot->init(parentPosition, parentSize, this, 
            directiontHint.x < 0? 1 : 0, directiontHint.y < 0? 1 : 0, directiontHint.z < 0? 1 : 0);
        m_Parent = newRoot;
        return newRoot->rootInsert(drawable);
    }
}
void CullOctreeNode::insert( Drawable* drawable )
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
            for (uint32 i(0); i < 8; ++i)
            {
                const Bound& strictBound(m_ChildNodes[i]->getStrictBound());
                if (strictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()) == false)
                    continue; // maybe it is in an other child
#ifdef OCTREE_PARANOID
                const Bound& looseBound(m_ChildNodes[i]->getLooseBound());
                HE_ASSERT(looseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
#endif
                m_ChildNodes[i]->insert(drawable);
                return;
            }
#ifdef OCTREE_PARANOID
            LOG(LogType_ProgrammerAssert, "Object fits loosebound but is not in one of the childs!");
#endif
        }
    }
    if (m_Parent == nullptr)
    {

    }
    drawable->setNode(this);

    if (m_NumObjectChilds == m_ObjectChildsCapacity)
    {
        m_ObjectChildsCapacity += 5;
        m_ObjectChilds = static_cast<Drawable**>(he_realloc(m_ObjectChilds, m_ObjectChildsCapacity));
    }
    m_ObjectChilds[m_NumObjectChilds++] = drawable;
}
void CullOctreeNode::rinsert( Drawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
#ifdef OCTREE_PARANOID
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
#endif
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
void CullOctreeNode::reevaluate( Drawable* drawable )
{
    const Bound& drawableBound(drawable->getBound());
    if (drawableBound.getSphere().getRadius() <= m_StrictBound.getSphere().getRadius() &&
        m_StrictBound.getAABB().isOtherInside(drawableBound.getSphere().getPosition()))
    {
#ifdef OCTREE_PARANOID
        HE_ASSERT(m_LooseBound.isOtherInside(drawableBound), "Object not completely in loose bound!");
#endif
        return; // OK - (we do not handle shrinking)
    }
    else
    {
        remove(drawable);
        if (m_Parent != nullptr)
            m_Parent->rinsert(drawable);
        else
            rootInsert(drawable);
        if (checkRemove())
        {
            doRemoveChilderen(true);
        }
    }

}
void CullOctreeNode::remove( Drawable* obj )
{
    Drawable** begin = m_ObjectChilds;
    Drawable** end = m_ObjectChilds + m_NumObjectChilds;
    Drawable** it = std::find(begin, end, obj);
    HE_IF_ASSERT(it != end, "Obj is not attached to this node")
    {
        *it = m_ObjectChilds[--m_NumObjectChilds];
        obj->setNode(nullptr);
    }
}

void CullOctreeNode::createChilds()
{
    if (m_IsLeafe)
        return;
    uint8 index(0);
    for (uint8 x(0); x < 2; ++x)
    for (uint8 y(0); y < 2; ++y)
    for (uint8 z(0); z < 2; ++z)
    {
        CullOctreeNode* node(CullOctreeNodeFactory::getInstance()->getNode());
        m_ChildNodes[index++] = node;
        node->init(this, x, y, z);
    }
}

void CullOctreeNode::createChilds( CullOctreeNode* child, uint8 xIndex, uint8 yIndex, uint8 zIndex )
{
    uint8 index(0);
    for (uint8 x(0); x < 2; ++x)
    for (uint8 y(0); y < 2; ++y)
    for (uint8 z(0); z < 2; ++z)
    {
        if (x != xIndex || y != yIndex || z != zIndex)
        {
            CullOctreeNode* node(CullOctreeNodeFactory::getInstance()->getNode());
            m_ChildNodes[index++] = node;
            node->init(this, x, y, z);
        }
        else
            m_ChildNodes[index++] = child;
    }
}

void CullOctreeNode::draw( const ICamera* camera, std::function<void(Drawable*)> drawFunction, bool checkChilderen ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    const vec3& cameraPosition(camera->getPosition());
    
    if (checkChilderen == true)
    {
        HIERARCHICAL_PROFILE("checkChilderen");
        // if camera is inside bound
        if ( m_LooseBound.getAABB().isOtherInside(cameraPosition) == false )
        {
            // sphere frustum test
            switch(camera->intersect(m_LooseBound))
            {
            case IntersectResult_Outside:
                {
                    HIERARCHICAL_PROFILE("Outside");
                    return;
                }
            case IntersectResult_Inside:
                {
                    HIERARCHICAL_PROFILE("Inside");
                    checkChilderen = false; // completely in -> all childeren are in as well
                }
                break;
            case IntersectResult_Intersecting:
                {
                    HIERARCHICAL_PROFILE("Intersecting");
                    break;
                }
            }
        }
    }

    if (m_ChildNodes[0] != nullptr) // if first child is not nullptr then all other are not nullptr as well
    {
        for (uint32 i(0); i < 8; ++i)
        {
            m_ChildNodes[i]->draw(camera, drawFunction, checkChilderen);
        }
    }
    if (m_NumObjectChilds != 0)
    {
        HIERARCHICAL_PROFILE("draw objects");
        Drawable** begin = m_ObjectChilds;
        Drawable** end = m_ObjectChilds + m_NumObjectChilds;
        std::for_each(begin, end, drawFunction);
    }
}

void CullOctreeNode::drawAndCreateDebugMesh( const ICamera* camera, std::function<void(Drawable*)> drawFunction, bool checkChilderen, 
    he::PrimitiveList<vec3>& vertices, he::PrimitiveList<uint32>& indices ) const
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    const vec3& cameraPosition(camera->getPosition());

    if (checkChilderen == true)
    {
        HIERARCHICAL_PROFILE("checkChilderen");
        m_StrictBound.getAABB().generateIndices(indices, static_cast<uint32>(vertices.size()));
        m_StrictBound.getAABB().generateVertices(vertices);
        // if camera is inside bound
        if ( m_LooseBound.getAABB().isOtherInside(cameraPosition) == false )
        {
            // sphere frustum test
            switch(camera->intersect(m_LooseBound))
            {
            case IntersectResult_Outside:
                {
                    HIERARCHICAL_PROFILE("Outside");
                    return;
                }
            case IntersectResult_Inside:
                {
                    HIERARCHICAL_PROFILE("Inside");
                    checkChilderen = false; // completely in -> all childeren are in as well
                }
                break;
            case IntersectResult_Intersecting:
                {
                    HIERARCHICAL_PROFILE("Intersecting");
                    break;
                }
            }
        }
    }

    if (m_ChildNodes[0] != nullptr) // if first child is not nullptr then all other are not nullptr as well
    {
        for (uint32 i(0); i < 8; ++i)
        {
            m_ChildNodes[i]->drawAndCreateDebugMesh(camera, drawFunction, checkChilderen, vertices, indices);
        }
    }
    if (m_NumObjectChilds != 0)
    {
        Drawable** begin = m_ObjectChilds;
        Drawable** end = m_ObjectChilds + m_NumObjectChilds;
        std::for_each(begin, end, drawFunction);
    }
}

CullOctreeNode* CullOctreeNode::getRoot()
{
    if (m_Parent == nullptr)
        return this;
    else 
        return m_Parent->getRoot();
}

bool CullOctreeNode::checkRemove() const
{
    bool remove(m_NumObjectChilds == 0);
    if (remove == true)
    {
        return canRemoveChilderen();
    }
    return remove;
}
bool CullOctreeNode::canRemoveChilderen() const
{
    if (m_ChildNodes[0] != nullptr) // if first child is not nullptr then all other are not nullptr as well
    {
        for (uint32 i(0); i < 8; ++i)
        {
            if (m_ChildNodes[i]->checkRemove() == false)
                return false;
        }
    }
    return true;
}

void CullOctreeNode::doRemoveChilderen(bool checkParent)
{
    if (m_ChildNodes[0] != nullptr)
    {
        HIERARCHICAL_PROFILE(__HE_FUNCTION__);
        CullOctreeNodeFactory* factory(CullOctreeNodeFactory::getInstance());
        for (uint32 i(0); i < 8; ++i)
        {
            m_ChildNodes[i]->doRemoveChilderen(false);
            factory->releaseNode(m_ChildNodes[i]);
            m_ChildNodes[i] = nullptr;
        }
    }
    if (m_Parent != nullptr && checkParent && m_Parent->canRemoveChilderen())
        m_Parent->doRemoveChilderen(true);
}

} } //end namespace

#endif
