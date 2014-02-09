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
#include "HappyPCH.h" 

#ifdef HE_USE_OCTREE
#include "CullOctreeNodeFactory.h"
#include "CullOctree.h"

namespace he {
namespace gfx {

#define BUFFER_SIZE 1024

CullOctreeNodeFactory::CullOctreeNodeFactory()
{
}

CullOctreeNodeFactory::~CullOctreeNodeFactory()
{
    m_Buffers.forEach([](CullOctreeNode* buffer)
    {
        he_free(buffer);
    });
}

void CullOctreeNodeFactory::addBuffer()
{
    CullOctreeNode* buffer = static_cast<CullOctreeNode*>(he_malloc(sizeof(CullOctreeNode) * BUFFER_SIZE));
    he_memset(buffer, 0, sizeof(CullOctreeNode) * BUFFER_SIZE);
    
    uint32 bufferIndex(static_cast<uint32>(m_Buffers.size()));
    m_Buffers.add(buffer);

    uint32 shiftBufferIndex((bufferIndex << 16) & 0xffff0000);
    for (uint32 i(0); i < BUFFER_SIZE; ++i)
    {
        uint32 index = shiftBufferIndex | i;
        buffer[i].create(index);
        m_FreeNodes.push(index);
    }

    HE_INFO("Creating new octree buffer");
}

CullOctreeNode* CullOctreeNodeFactory::getNode()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_FreeNodes.empty())
        addBuffer();

    uint32 index(m_FreeNodes.front());
    m_FreeNodes.pop();

    uint32 bufferId( (index & 0xffff0000) >> 16 );
    uint32 nodeIndex( index & 0x0000ffff );

    CullOctreeNode* node(m_Buffers[bufferId] + nodeIndex);
    HE_ASSERT(node->getPoolIndex() == index, "index does not match");
    node->reset();
    return node;
}

void CullOctreeNodeFactory::releaseNode( CullOctreeNode* node )
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    HE_ASSERT(node->getNumObjectChilds() == 0, "Object leak in octree!");
    m_FreeNodes.push(node->getPoolIndex());
    node->reset();
}

} } //end namespace

#endif HE_USE_OCTREE
