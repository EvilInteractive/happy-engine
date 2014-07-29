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
//Created: 11/11/2012

namespace he {

template<typename TInput, typename TOutput>
void he::NodeGraph<TInput, TOutput>::addRootNode( NodeGraphNode<TInput, TOutput>* node )
{
    m_RootNodes.add(node);
}

template<typename TInput, typename TOutput>
void he::NodeGraph<TInput, TOutput>::removeRootNode( NodeGraphNode<TInput, TOutput>* node )
{
    m_RootNodes.remove(node);
}

template<typename TInput, typename TOutput>
void he::NodeGraph<TInput, TOutput>::clear()
{
    m_RootNodes.clear();
}

template<typename TInput, typename TOutput>
void he::NodeGraph<TInput, TOutput>::evalute( he::ObjectList<NodeGraphError<TInput, TOutput>>& errorList )
{
    uint8 marchId(m_CurrentMarchId);
    m_RootNodes.forEach([marchId, &errorList](NodeGraphNode<TInput, TOutput>* node)
    {
        node->evaluteMarch(marchId, errorList);
    });
    if (marchId < UINT8_MAX)
        ++m_CurrentMarchId;
    else
        m_CurrentMarchId = 0;
}

template<typename TInput, typename TOutput>
void he::NodeGraph<TInput, TOutput>::evaluteFrom(NodeGraphNode<TInput, TOutput>* node,  he::ObjectList<NodeGraphError<TInput, TOutput>>& errorList )
{
    uint8 marchId(m_CurrentMarchId);
    node->evaluteMarch(marchId, errorList);

    if (marchId < UINT8_MAX)
        ++m_CurrentMarchId;
    else
        m_CurrentMarchId = 0;
}

} //end namespace
