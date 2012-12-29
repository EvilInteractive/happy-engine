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
//Created: 11/11/2012

#ifndef _HE_NodeGraph_H_
#define _HE_NodeGraph_H_
#pragma once

#include "NodeGraphError.h"

namespace he {
template<typename TInput, typename TOutput>
class NodeGraphNode;

template<typename TInput, typename TOutput>
class NodeGraph
{
public:
    NodeGraph(): m_CurrentMarchId(0) {}
    ~NodeGraph() {}

    void addRootNode(NodeGraphNode<TInput, TOutput>* node);
    void removeRootNode(NodeGraphNode<TInput, TOutput>* node);

    void clear();

    void evalute(he::ObjectList<NodeGraphError<TInput, TOutput>>& errorList);
    void evaluteFrom(NodeGraphNode<TInput, TOutput>* node, he::ObjectList<NodeGraphError<TInput, TOutput>>& errorList);

private:

    he::PrimitiveList<NodeGraphNode<TInput, TOutput>*> m_RootNodes;

    uint8 m_CurrentMarchId;

    //Disable default copy constructor and default assignment operator
    NodeGraph(const NodeGraph&);
    NodeGraph& operator=(const NodeGraph&);
};

} //end namespace

#include "NodeGraph.inl"

#endif
