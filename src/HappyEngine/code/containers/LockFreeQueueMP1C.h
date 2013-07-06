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
//Created: 2013/06/22

#ifndef _HE_LockFreeQueueMP1C_H_
#define _HE_LockFreeQueueMP1C_H_
#pragma once

namespace he {

// Lock free queue: multiple producers, 1 consumer
template<typename T>
class LockFreeQueueMP1C
{
    template<typename R>
    struct Node
    {
        Node(const R& data): m_Next(nullptr), m_Data(data) {}
        Node(): m_Next(nullptr) {}
        Node<R>* m_Next;
        R m_Data;
    };
    typedef Node<T> TNode;
public:
    LockFreeQueueMP1C();
    ~LockFreeQueueMP1C();

    void push(const T& obj);
    bool pop(T& outObj);
    bool empty();

private:
    std::atomic<TNode*> m_Tail;
    TNode* m_Head;

    //Disable default copy constructor and default assignment operator
    LockFreeQueueMP1C(const LockFreeQueueMP1C&);
    LockFreeQueueMP1C& operator=(const LockFreeQueueMP1C&);
};

} //end namespace

#include "LockFreeQueueMP1C.inl"

#endif
