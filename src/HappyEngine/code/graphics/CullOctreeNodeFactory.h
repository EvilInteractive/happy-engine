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
//Created: 07/10/2012

#ifndef _HE_CullOctreeNodeFactory_H_
#define _HE_CullOctreeNodeFactory_H_
#pragma once

#include "Singleton.h"

namespace he {
namespace gfx {
class CullOctreeNode;

class CullOctreeNodeFactory : public Singleton<CullOctreeNodeFactory>
{
friend class Singleton<CullOctreeNodeFactory>;
public:
    virtual ~CullOctreeNodeFactory();

    CullOctreeNode* getNode();
    void releaseNode(CullOctreeNode* node);

private:
    CullOctreeNodeFactory();

    void addBuffer();

    std::queue<uint32> m_FreeNodes;
    std::vector<CullOctreeNode*> m_Buffers;
};

} } //end namespace

#endif
