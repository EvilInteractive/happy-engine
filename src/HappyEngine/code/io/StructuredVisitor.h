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
//Created: 26/01/2013

#ifndef _HE_STRUCTURED_VISITOR_H_
#define _HE_STRUCTURED_VISITOR_H_
#pragma once

namespace he {
namespace io {

class HAPPY_ENTRY StructuredVisitor
{
public:
    enum EOpenType
    {
        eOpenType_Read, eOpenType_Write, eOpenType_Closed
    };
    StructuredVisitor(): m_OpenType(eOpenType_Closed) {}
    virtual ~StructuredVisitor() {}

    bool isOpen() const { return m_OpenType != eOpenType_Closed; }
    bool isReading() const { return m_OpenType == eOpenType_Read; }
    bool isWriting() const { return m_OpenType == eOpenType_Write; }

    virtual void close() = 0;

    virtual bool enterNode(const he::FixedString& key, const char* comment = NULL) = 0;
    virtual void exitNode(const he::FixedString& key) = 0;

    bool visit(const he::FixedString& key, he::String& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, bool& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, int8& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, uint8& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, int16& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, uint16& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, int32& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, uint32& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, int64& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, uint64& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, float& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, double& value, const char* comment = NULL)
    {
        return internalVisit(key, value, comment);
    }
    bool visit(const he::FixedString& key, vec2& value, const char* comment = NULL);
    bool visit(const he::FixedString& key, vec3& value, const char* comment = NULL);
    bool visit(const he::FixedString& key, vec4& value, const char* comment = NULL);
    bool visit(const he::FixedString& key, Guid& value, const char* comment = NULL);

    template<typename T>
    bool visitList(const he::FixedString& key, he::ObjectList<T>& list, const char* comment = NULL)
    {
        return internalVisitList(key, list, comment);
    }
    template<typename T>
    bool visitList(const he::FixedString& key, he::PrimitiveList<T>& list, const char* comment = NULL)
    {
        return internalVisitList(key, list, comment);
    }

    template<typename T>
    bool visitObjectList(const he::FixedString& key, he::ObjectList<T>& list, const char* comment = NULL)
    {
        return internalVisitObjectList(key, list, comment);
    }

    template<typename T>
    bool visitCustomList(const he::FixedString& key, he::ObjectList<T>& list, 
        const boost::function3<void, StructuredVisitor* const, const size_t, T&>& callback, const char* comment = NULL)
    {
        return internalVisitCustomList<he::ObjectList<T>, T>(key, list, callback, comment);
    }
    template<typename T>
    bool visitCustomList(const he::FixedString& key, he::PrimitiveList<T>& list,
        const boost::function3<void, StructuredVisitor* const, const size_t, T&>& callback, const char* comment = NULL)
    {
        return internalVisitCustomList<he::PrimitiveList<T>, T>(key, list, callback, comment);
    }
    
    template<typename EnumType, typename CastType>
    bool visitEnum(const he::FixedString& key, EnumType& enumValue, const char* comment = NULL)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        CastType value;
        if (m_OpenType == eOpenType_Write)
        {
            value = checked_numcast<CastType>(enumValue);
        }
        const bool result(visit(key, value, comment));

        enumValue = checked_numcast<EnumType>(value);

        return result;
    }

protected:
    // Array methods
    virtual bool enterNode(const size_t index, const char* comment = NULL) = 0;
    virtual void exitNode(const size_t index) = 0;
    virtual bool enterArray(const he::FixedString& key, const char* comment = NULL) = 0;
    virtual void exitArray(const he::FixedString& key) = 0;
    virtual size_t getArraySize() = 0;
    
    // internal visits
    virtual bool visit(he::String& value, const char* comment = NULL) = 0;
    virtual bool visit(bool& value, const char* comment = NULL) = 0;
    virtual bool visit(int8& value, const char* comment = NULL) = 0;
    virtual bool visit(uint8& value, const char* comment = NULL) = 0;
    virtual bool visit(int16& value, const char* comment = NULL) = 0;
    virtual bool visit(uint16& value, const char* comment = NULL) = 0;
    virtual bool visit(int32& value, const char* comment = NULL) = 0;
    virtual bool visit(uint32& value, const char* comment = NULL) = 0;
    virtual bool visit(int64& value, const char* comment = NULL) = 0;
    virtual bool visit(uint64& value, const char* comment = NULL) = 0;
    virtual bool visit(float& value, const char* comment = NULL) = 0;
    virtual bool visit(double& value, const char* comment = NULL) = 0;

    EOpenType m_OpenType;

private:
    template <typename T>
    bool internalVisit(const he::FixedString& key, T& value, const char* comment)
    {
        bool result(false);
        if (enterNode(key))
        {
            result = visit(value, comment);
            exitNode(key);
        }
        return result;
    }

    template<typename T>
    bool internalVisitList(const he::FixedString& key, T& list, const char* comment = NULL)
    {
        bool result(false);
        if (enterArray(key, comment))
        {
            HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
            const size_t size(m_OpenType == eOpenType_Read? getArraySize() : list.size());
            list.resize(size);
            for (size_t i(0); i < size; ++i)
            {
                if (enterNode(i))
                {
                    visit(list[i], NULL);
                    exitNode(i);
                }
            };
            result = true;
            exitArray(key);
        }
        return result;
    }

    template<typename T, typename U>
    bool internalVisitCustomList(const he::FixedString& key, T& list,
        const boost::function3<void, StructuredVisitor* const, const size_t, U&>& callback, const char* comment = NULL)
    {
        bool result(false);
        if (enterArray(key, comment))
        {
            HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
            HE_ASSERT(m_OpenType != eOpenType_Read || list.empty(), "List is not empty when visiting!");
            const size_t size(m_OpenType == eOpenType_Read? getArraySize() : list.size());
            list.resize(size);
            for (size_t i(0); i < size; ++i)
            {
                U& element(list[i]);
                if (enterNode(i))
                {
                    callback(this, i, element);
                    exitNode(i);
                }
            };
            result = true;
            exitArray(key);
        }
        return result;
    }

    template<typename T>
    bool internalVisitObjectList(const he::FixedString& key, he::ObjectList<T>& list, const char* comment = NULL)
    {
        bool result(false);
        if (enterArray(key, comment))
        {
            HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
            const size_t size(m_OpenType == eOpenType_Read? getArraySize() : list.size());
            list.resize(size);
            for (size_t i(0); i < size; ++i)
            {
                if (enterNode(i))
                {
                    T& element(list[i]);
                    element.visit(this);
                    exitNode(i);
                }
            };
            result = true;
            exitArray(key);
        }
        return result;
    }

    //Disable default copy constructor and default assignment operator
    StructuredVisitor(const StructuredVisitor&);
    StructuredVisitor& operator=(const StructuredVisitor&);
};
    

} } //end namespace

#endif
