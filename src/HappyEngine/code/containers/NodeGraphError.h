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

#ifndef _HE_NodeGraphError_H_
#define _HE_NodeGraphError_H_
#pragma once

#include "NodeGraphNode.h"

namespace he {

template<typename TInput, typename TOutput>
class NodeGraphNode;

template<typename TInput, typename TOutput>
struct NodeGraphError
{
    friend class NodeGraphNode<TInput, TOutput>;
    NodeGraphError()
        : m_Node(nullptr)
        , m_ErrorMessage("")
        , m_Fixed(true)
    {
    }

    void setMessage(const char* message, ...)
    {
        va_list argList;
        va_start(argList, message);
        char buff[1024];
        memset(buff, 0, 1024);
        hevsnprintf(buff, 1023, message, argList);
        va_end(argList);

        m_ErrorMessage = buff;
    }

    const he::String getMessage() const { return m_ErrorMessage; }
    const NodeGraphNode<TInput, TOutput>* getNode() const { return m_Node; }
    const NodeGraphConnection<TInput, TOutput>& getConnection() const { return m_Connecter; }
    bool isFatal() const { return !m_Fixed; }

private:
    const NodeGraphNode<TInput, TOutput>* m_Node;                  // Node that threw the error
    NodeGraphConnection<TInput, TOutput> m_Connecter;              // Connecter from the node that threw the error (can be nullptr)
    he::String m_ErrorMessage;                                    // Error message
    bool m_Fixed;                                                  // If true, error was fixed -> throw tool tip why something is not possible
                                                                   // If false, user needs to solve the error
};

} //end namespace

#endif
