//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/01/24

#ifndef Queue_h__
#define Queue_h__

#include <queue>

namespace he {

template<typename T>
class Queue : public std::queue<T, std::deque<T, StlAllocater<T>>>
{
    typedef std::queue<T, std::deque<T, StlAllocater<T>>> TParent;
public:
    Queue() : TParent() {}
    void clone(const Queue& other);

    // do nothing special in here!
    Queue(Queue&& other);
    Queue& operator=(Queue&& other);

private:
    Queue(const Queue&);
    Queue& operator=(const Queue&);
};

}

#include "Queue.inl"

#endif // Queue_h__
