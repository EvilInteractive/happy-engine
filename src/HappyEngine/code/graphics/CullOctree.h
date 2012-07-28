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

#include "Bound.h"

namespace he {
namespace gfx {
class IDrawable;
class CullOctree;
class ICamera;

class CullOctreeNode
{
public:
    explicit CullOctreeNode(CullOctreeNode* parent, const Bound& bound);
    ~CullOctreeNode();

    const Bound& getBound() const { return m_Bound; }

    void insert(IDrawable* drawable);
    void rinsert(IDrawable* obj);
    void remove(IDrawable* obj);
    void reevaluate(IDrawable* obj);
    void draw(const ICamera* camera, boost::function1<void, IDrawable*> drawFunction, bool checkChilderen) const;

private:
    void createChilds();

    CullOctreeNode* m_ChildNodes[8];
    CullOctreeNode* m_Parent;
    std::vector<IDrawable*> m_ObjectChilds;
    Bound m_Bound;
};

class CullOctree
{
public:

    explicit CullOctree(uint32 rootSize);
    ~CullOctree();

    void insert(IDrawable* obj);
    void reevaluate(IDrawable* obj);
    void remove(IDrawable* obj);

    void draw(const ICamera* camera, boost::function1<void, IDrawable*> drawFunction) const;

    const static int s_MinLeaveSize = 2;

private:
    CullOctreeNode* m_Root;

    //Disable default copy constructor and default assignment operator
    CullOctree(const CullOctree&);
    CullOctree& operator=(const CullOctree&);
};

} } //end namespace

#endif
