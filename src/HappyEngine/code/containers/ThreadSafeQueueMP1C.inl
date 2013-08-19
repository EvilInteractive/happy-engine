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
//Created: 2013/08/19
namespace he {
    
template<typename T>
ThreadSafeQueueMP1C<T>::ThreadSafeQueueMP1C(const size_t initSize, const size_t growBy, const char* debugName)
    : m_Head(nullptr)
    , m_Tail(nullptr)
    , m_NodeStackHead(nullptr)
    , m_GrowBy(growBy)
    , m_Name(debugName)
    , m_PoolSize(0)
{
    enlargePool(initSize);

    // We always have one node in the queue, this is the empty node (no nullptr checks needed)
    m_Head = createNode(T());
    // If m_Tail == m_Head, the queue is empty
    m_Tail  = m_Head;
}

template<typename T>
ThreadSafeQueueMP1C<T>::~ThreadSafeQueueMP1C()
{
    // No access should happen when the destructor is called
    {
        TNode* head(m_Head);
        while (head != nullptr)
        {
            TNode* next(head->m_Next);
            removeNode(head);
            head = next;
        }
    }

    {
        TNode* head(m_NodeStackHead);
        while (head != nullptr)
        {
            TNode* const oldHead(head);
            head = oldHead->m_Next;
            delete oldHead;
        }
    }
}

template<typename T>
void he::ThreadSafeQueueMP1C<T>::enlargePool(const size_t amount)
{
    HE_CONDITIONAL_WARNING(m_PoolSize != 0, "%s is increasing in size! %d -> %d", m_Name, m_PoolSize, m_PoolSize + amount);
    m_PoolSize += amount;
    for (size_t i(0); i < amount; ++i)
    {
        removeNode(NEW TNode()); // "Remove" the node to make the stack bigger
    }
}

template<typename T>
typename ThreadSafeQueueMP1C<T>::TNode* he::ThreadSafeQueueMP1C<T>::createNode( const T& data )
{
    m_Mutex.lock(FILE_AND_LINE);
    TNode* returnValue(m_NodeStackHead);
    if (returnValue == nullptr)
    {
        enlargePool(m_GrowBy);
        returnValue = m_NodeStackHead;
    }
    m_NodeStackHead = returnValue->m_Next;
    m_Mutex.unlock();

    returnValue->m_Data = data;
    returnValue->m_Next = nullptr;

    return returnValue;
}

template<typename T>
void he::ThreadSafeQueueMP1C<T>::removeNode( TNode* const node )
{
    m_Mutex.lock(FILE_AND_LINE);
    node->m_Next = m_NodeStackHead;
    m_NodeStackHead = node;
    m_Mutex.unlock();
}

template<typename T>
void ThreadSafeQueueMP1C<T>::push( const T& obj )
{
    m_Mutex.lock(FILE_AND_LINE);
    TNode* const newNode(createNode(obj));
    m_Tail->m_Next = newNode;
    m_Tail = newNode;
    m_Mutex.unlock();
}

template<typename T>
bool ThreadSafeQueueMP1C<T>::pop(T& outObj)
{
    if (empty() == false) 
    { 
        m_Mutex.lock(FILE_AND_LINE);

        TNode* head(m_Head);
        TNode* next(head->m_Next);
        outObj = next->m_Data;
        m_Head = next;

        m_Mutex.unlock();

        removeNode(head);

        return true;
    }
    return false;
}

template<typename T>
bool ThreadSafeQueueMP1C<T>::empty()
{
    return m_Head == m_Tail;
}
} //end namespace
