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

#ifndef _HE_CullOctree_H_
#define _HE_CullOctree_H_
#pragma once

#ifdef HE_USE_OCTREE

#include "Bound.h"

namespace he {
namespace gfx {
class Drawable;
class CullOctree;
class ICamera;

class CullOctreeNode
{
public:
    void create(uint32 poolIndex); // pool create
    void reset(); // pool reset
    inline uint32 getPoolIndex() const { return m_PoolIndex; }

    inline const Bound& getStrictBound() const { return m_StrictBound; }
    inline const Bound& getLooseBound() const { return m_LooseBound; }

    CullOctreeNode* rootInsert(Drawable* drawable); // returns current root
    void remove(Drawable* obj);
    void reevaluate(Drawable* obj);
    void draw(const ICamera* camera, boost::function1<void, Drawable*> drawFunction, bool checkChilderen) const;
    void drawAndCreateDebugMesh(const ICamera* camera, boost::function1<void, Drawable*> drawFunction, bool checkChilderen, 
        he::PrimitiveList<vec3>& vertices, he::PrimitiveList<uint32>& indices) const;
    CullOctreeNode* getRoot();

    bool checkRemove() const;
    bool canRemoveChilderen() const;
    void doRemoveChilderen(bool checkParent);
    uint16 getNumObjectChilds() const { return m_NumObjectChilds; }

private:
    void init(CullOctreeNode* parent, uint8 xIndex, uint8 yIndex, uint8 zIndex);
    void init(const vec3& pos, float strictSize, CullOctreeNode* child, uint8 xIndex, uint8 yIndex, uint8 zIndex);

    void createChilds();
    void createChilds(CullOctreeNode* child, uint8 xIndex, uint8 yIndex, uint8 zIndex);
    void rinsert(Drawable* obj);
    void createBounds(const vec3& pos, float strictSize);
    void insert(Drawable* drawable);

    CullOctreeNode* m_ChildNodes[8];
    CullOctreeNode* m_Parent;
    Drawable** m_ObjectChilds;
    uint16 m_NumObjectChilds, m_ObjectChildsCapacity;
    Bound m_StrictBound, m_LooseBound;
    bool m_IsLeafe;
    uint32 m_PoolIndex;
};

class CullOctree
{
public:

    explicit CullOctree(); 
    ~CullOctree();

    void insert(Drawable* obj);
    void reevaluate(Drawable* obj);
    void remove(Drawable* obj);

    void draw(const ICamera* camera, boost::function1<void, Drawable*> drawFunction) const;
    void drawAndCreateDebugMesh(const ICamera* camera, boost::function1<void, Drawable*> drawFunction, 
        he::PrimitiveList<vec3>& vertices, he::PrimitiveList<uint32>& indices) const;

    const static float s_MinLeaveSize;

private:
    CullOctreeNode* m_Root;

    //Disable default copy constructor and default assignment operator
    CullOctree(const CullOctree&);
    CullOctree& operator=(const CullOctree&);
};

} } //end namespace

#endif // HE_USE_OCTREE

#endif
