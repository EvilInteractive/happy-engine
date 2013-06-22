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
namespace he {

template<typename T>
struct LockFreeQueueMP1C::Node
{
    Node(const T& data): m_Next(nullptr), m_Data(data) {}
    volatile Node* m_Next;
    volatile T m_Data;
};

template<typename T>
LockFreeQueueMP1C::LockFreeQueueMP1C()
    : m_Head(NEW TNode())
    , m_Tail(nullptr)
{
    m_Tail.store(m_Head);
}


template<typename T>
LockFreeQueueMP1C::~LockFreeQueueMP1C()
{
    // No access should happen when the destructor is called
    while (m_Head != nullptr)
    {
        Node* oldHead(m_Head);
        m_Head = m_Head->m_Next;
        delete oldHead;
    }
}

template<typename T>
void LockFreeQueueMP1C<T>::push( const T& obj )
{
    TNode* newNode(NEW TNode(obj));
    TNode* oldTail(m_Tail.exchange(newNode));
    oldTail->m_Next = newNode;
}

template<typename T>
bool LockFreeQueueMP1C<T>::pop(T& outObj)
{
    while (empty() == false) 
    { 
        Node* next(m_Head->m_Next);
        if (next != nullptr)
        {
            Node* oldHead(m_Head);
            m_Head = next;
            outObj = m_Head->m_Data;
            delete oldHead;
            return true;
        }
    }
    return false;
}

template<typename T>
bool LockFreeQueueMP1C<T>::empty()
{
    return m_Head->m_Next == nullptr;
}
} //end namespace
