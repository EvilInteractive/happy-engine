//HappySandBox Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappySandBox.
//
//    HappySandBox is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappySandBox is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappySandBox.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _X_H_
#define _X_H_
#pragma once

namespace happysandbox {

class X
{
public:
    X();
    virtual ~X();

private:

    //Disable default copy constructor and default assignment operator
    X(const X&);
    X& operator=(const X&);
};

} //end namespace

#endif
