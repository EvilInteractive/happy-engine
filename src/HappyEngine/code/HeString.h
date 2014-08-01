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
//Created: 2013/03/09

#ifndef _HE_String_H_
#define _HE_String_H_
#pragma once

#include <string>

namespace he {
// template <typename T>
// class HeStringAllocator
// {
// public:
//     typedef T value_type;
//     typedef T* pointer;
//     typedef const T* const_pointer;
//     typedef T& reference;
//     typedef const T& const_reference;
//     typedef size_t size_type;
//     typedef ptrdiff_t difference_type;
// 
//     template <typename U>
//     struct rebind
//     {
//         typedef HeStringAllocator<U> other;
//     };
// 
//     HeStringAllocator() {}
//     HeStringAllocator(const HeStringAllocator&) {}
//     template<typename U>
//     HeStringAllocator(const HeStringAllocator<U>&) {}
//     ~HeStringAllocator() {}
// 
//     bool operator==(const HeStringAllocator& /*other*/) const
//     {
//         return true; // Always in global heap, so does not matter
//     }
//     bool operator!=(const HeStringAllocator& /*other*/) const
//     {
//         return false;// Always in global heap, so does not matter
//     }
// 
//     pointer allocate(size_type size, const_pointer /*hint*/ = nullptr)
//     {
//         return static_cast<pointer>(he_malloc(sizeof(T) * size));
//     }
//     void deallocate(pointer memory, size_type /*ignored*/)
//     {
//         he_free(memory);
//     }
// 
//     typename pointer address ( reference x ) const /*noexcept*/
//     {
//         return static_cast<pointer>(&val); 
//     }
//     typename const_pointer address ( typename const_reference x ) const /*noexcept*/
//     {
//         return static_cast<const_pointer>(&val); 
//     }
// 
// #if defined(_MSC_VER) && _MSC_VER < 1800
//     #define _ALLOC_MEMBER_CONSTRUCT( \
//         TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, CALL_OPT, X2, X3, X4) \
//         template<class _Objty COMMA LIST(_CLASS_TYPE)> \
//         void construct(_Objty *_Ptr COMMA LIST(_TYPE_REFREF_ARG)) \
//         {	/* construct _Objty(_Types...) at _Ptr */ \
//             PNEW(static_cast<void*>(_Ptr)) _Objty(LIST(_FORWARD_ARG)); \
//         }
// 
//         _VARIADIC_EXPAND_0X(_ALLOC_MEMBER_CONSTRUCT, , , , )
//     #undef _ALLOC_MEMBER_CONSTRUCT
// #else
//     template <typename U, typename... Args>
//     void construct (U* p, Args&&... args);
//     {
//         PNEW(p)U(std::forward<Args>(args)...);
//     }
// #endif
//     
//     size_type max_size() const /*noexcept*/ { return SIZE_T_MAX; }
// 
//     template <typename U>
//     void destroy(U* ptr)	
//     {	
//         ptr->~U();
//         ptr;
//     }
// };
typedef std::allocator<char> THeStringAllocator;
typedef std::basic_string<char, std::char_traits<char>, THeStringAllocator > StdString;
typedef std::basic_stringstream<char, std::char_traits<char>, THeStringAllocator > StringStream;
typedef StdString String;

/*class HAPPY_ENTRY String : public StdString
{
public:
    String();
    ~String();

    String(const char* charBuffer, const int len = -1);
    String(const wchar_t* charBuffer);
    String(he::String&& other);
    String(StdString&& other);
    String(const he::String& other);
    String& operator=(he::String&& other);
    String& operator=(const he::String& other);
    
    he::String clone() const;
    uint32 hash() const;

    // Operators
    bool operator==(const he::String& other) const;
    bool operator!=(const he::String& other) const;
    bool operator<(const he::String& other) const;
    bool operator==(const char* const other) const;
    bool operator!=(const char* const other) const;
    bool operator<(const char* const other) const;

    String& operator+=(const he::String& other);
    String& operator+=(he::String&& other);
    String& operator+=(const char* const other);
    String operator+(const he::String& other) const;
    String operator+(he::String&& other) const;
    String operator+(const char* const other) const;
};*/

void toLower(char* const buff);
uint32 hash(const uint32 oldHash, const he::String& str);

} //end namespace

#endif
