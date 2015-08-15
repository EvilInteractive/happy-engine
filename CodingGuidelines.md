These are the coding guidelines/rules you need to keep while coding.

# Guidelines #
### General ###
  * Prefer a white space after the // of a comment
  * Curly Brackets { } are put on a newline, not on the same one (exception: namespaces)
```
void test() {
} // WRONG

void test() 
{
} // Correct :)
```
  * Use clear naming. Prefer long names over bad short ones
```
ObjectHandle m_ObjHndl;       // WRONG
ObjectHandle m_ObjectHandle;  // Correct :)
```
  * After a comma or operator follows a white space
```
int a(add(1,2+4)); // WRONG
int a(add(1, 2 + 4)); // Correct :)
```
### Variables ###
  * Local variables have a lowerCamelCase name
```
float myLocalVariable;
```
  * Members variables are prefixed with `m_` followed by an UpperCamelCase name
```
float m_MyMemberVariable;
```
  * Static variables are prefixed with `s_` followed by an UpperCamelCase name
```
static int s_MyStatic;
```
  * Globals are prefixed with `g_` followed by an UpperCamelCase name. Avoid globals if possible or at least make them const
```
const short g_MyGlobal;
```
  * Do **not** use [Hungarian notation](http://en.wikipedia.org/wiki/Hungarian_notation)
```
float m_fMyFloat; // WRONG!
float m_MyFloat;  // Correct :)

void* m_pMyPointer; // WRONG!
void* m_MyPointer;  // Correct :)
```
  * Const as much as you can! If you only assign a variable once, make it const. Parameters in functions should also be const
```
void test(const float f1, const int* const constpointer, int* const pointer)
{
    const size_t size(m_MyArray.size());
    for (size_t i(0); i < size; ++i) {}
}
```
  * Prefer constructor initialization over assignment operator initialization
```
int a = 5; // WRONG
int a(5); // Good :)

for (uint i(0); i < 5; ++i) // Good :)
{
}

for (uint i = 0; i < 5; ++i) // WRONG
{
}
```
  * Prefer c++ style casting over c-style
```
int a(0);
a = (int)(5.2f); // WRONG!
a = static_cast<int>(5.2f); // Correct :)
```
  * You cannot dynamic\_cast because RTTI is off
  * Try to never use reinterpret\_cast and const\_cast
  * Initialize pointers with nullptr, not 0 or NULL (c++0x way)
  * Use NEW instead of new, the NEW macro tracks memleaks
```
A* myAClass(nullptr);
myAClass = new A(); // WRONG!
delete A;
myAClass = NEW A(); // Correct :)
```

### Namespace ###
  * As small as possible
  * All lowercase
  * ex:
```
namespace he {

} 
```
where 'he' stands for HappyEngine
  * he namespace is always the base namespace
  * nested namespaces are possible but keep to a minimum (max 3)
  * namespace are none indented and the first bracket is next to the namespace name. This is to keep the code clean.
  * ex:
```
namespace he {
namespace gfx {

} }
```
  * the only third nested namespace you should need is 'details'. Details is a namespace reserved for the parent namespace itself. Nobody should use anything directly out of a details namespace. It has only internal purposes and should not be used outside of its parent namespace.
```
namespace he {
namespace gfx {
namespace details {

} } }
```
  * DO NOT EVER EVER WRITE `using namespace x; ` in a header!
  * You can use `using namespace`, but keep the scope as small as possible: inside a cpp is good, inside a method is better, don't use it at all is best

### Classes ###
  * Use the X.h and X.cpp when you create a new class
X.h
```
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
//Author:  
//Created: //

#ifndef _HE_X_H_
#define _HE_X_H_
#pragma once

namespace he {

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

```
X.cpp
```
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
//Author:  
//Created: //
#include "HappyPCH.h" 

#include "X.h"

namespace he {

X::X()
{
}


X::~X()
{
}

} //end namespace

```
  * The X class contains everything you need for a new class: license, include guard, namespace, class, constructor and virtual destructor, private copy constructor and private default assignment operator
  * Find and replace 'X' to your class name
  * Class names are in UpperCamelCase
  * Fill in the author and create date. We keep track of the author to know who we need to blame or ask if something goes wrong ;)
  * When you edit a class you may - if you wish or make a big change - add your name under the creators name with a proper tag
```
//Author: Bastian Damman
//Edited by: Sebastiaan Sprengers - 10/05/2012 - reason: Added feature X
```
  * ALL member variables are private, write getters/setters
  * Forward declare everything, write as few #include's in a header file

### Structs ###
  * Structs are only used as data containers
  * All public members
  * Mostly no member functions
  * Name in UpperCamelCase
```
struct MyData
{
    float m_Data1;
    int   m_Data2;
    float m_Data3;
};
```

### Enums ###
  * Enums have UpperCamelCase names and values
  * Prefix enum values with EnumName_```
enum MyEnum
{
    MyEnum_Value1,
    MyEnum_Value2,
    MyEnum_Value3,
    MyEnum_MAX
};
```
  * Trick: add as last value `MyEnum_MAX` (MAX in UPPERCASE to show it is not really a value), the corresponding value of this will be the size you need for an array.
```
enum MyEnum 
{
    MyEnum_Value1,
    MyEnum_Value2,
    MyEnum_Value3,
    MyEnum_MAX
};

uint testArray[MyEnum_MAX];
testArray[MyEnum_Value1] = 5;
testArray[MyEnum_Value2] = 1;
testArray[MyEnum_Value3] = 4;

```
  * Enums are also handy for bit patterns
```
enum MyEnum
{
    MyEnum_Value1 = 1 << 0,
    MyEnum_Value2 = 1 << 1,
    MyEnum_Value3 = 1 << 2,
    MyEnum_Value4 = 1 << 3
};
```_

### Functions ###
  * All (member)functions are in lowerCamerCase
```
int testFunction()
{
    return 0;
}
```