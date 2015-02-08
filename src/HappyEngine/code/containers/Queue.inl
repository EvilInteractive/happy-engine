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

template<typename T>
he::Queue<T>::Queue( Queue&& other ) 
    : TParent(std::forward<TParent>(other))
{

}

template<typename T>
he::Queue<T>& he::Queue<T>::operator=( Queue&& other )
{
    TParent::operator=(std::forward<TParent>(other));
    return *this;
}

template<typename T>
void he::Queue<T>::clone( const Queue& other )
{
    TParent::operator=(other);
}
