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
#include "HappyPCH.h"

#include "JsonFileReader.h"

#include <json/json.h>
#include "FileReader.h"

namespace he {
namespace io {

class JsonFileReader::Reader
{
    struct RootStackItem
    {
        RootStackItem() 
            : m_CurrentRoot(nullptr)
            , m_CurrentRootName(HEFS::str)
        {
        }
        RootStackItem(const FixedString& rootName, const json::Value* root)
            : m_CurrentRoot(root)
            , m_CurrentRootName(rootName)
        {
        }
        ~RootStackItem() {}
        const json::Value* m_CurrentRoot;
        FixedString m_CurrentRootName;
    };

public:

    inline const json::Value& getRoot() const { return *m_RootStack.back().m_CurrentRoot; }
    inline const FixedString& getRootName() const { return m_RootStack.back().m_CurrentRootName; }
    inline json::Reader& getReader() { return m_Reader; }
    void pushRoot(const FixedString& rootName, const json::Value& root)
    {
        m_RootStack.add(RootStackItem(rootName, &root));
    }
    void popRoot() 
    {
        HE_ASSERT(m_RootStack.empty() == false, "No more roots to pop!");
        m_RootStack.removeAt(m_RootStack.size() - 1);
    }
    void init(json::Value&& value)
    {
        m_Root = std::forward<json::Value>(value);
        m_RootStack.clear();
        pushRoot(HEFS::str, m_Root);
    }

private:
    json::Reader m_Reader;
    ObjectList<RootStackItem> m_RootStack;
    json::Value m_Root;
};

JsonFileReader::JsonFileReader()
    : m_Reader(NEW Reader())
{
}

JsonFileReader::~JsonFileReader()
{
    HE_ASSERT(m_OpenType == StructuredVisitor::eOpenType_Closed, "Stream is still open when disposing JsonFileReader!");
    delete m_Reader;
}

bool JsonFileReader::open( const Path& path )
{
    bool result(false);
    io::FileReader freader;
    if (freader.open(path, FileReader::OpenType_ASCII))
    {
        he::String file(freader.readToEnd());
        json::Value root;
        if (m_Reader->getReader().parse(file, root, true))
        {
            m_Reader->init(std::move(root));
            m_OpenType = StructuredVisitor::eOpenType_Read;
            result = true;
        }
        else
        {
            std::string error(m_Reader->getReader().getFormattedErrorMessages());
            HE_ERROR("File corrupt %s\n\n%s", path.str().c_str(), error.c_str());
        }
    }
    else
    {
        HE_ERROR("File open failed: %s", path.str().c_str());
    }
    return result;
}

void JsonFileReader::close()
{
    HE_ASSERT(m_OpenType != StructuredVisitor::eOpenType_Closed, "JsonFileReader not open!");
    m_OpenType = StructuredVisitor::eOpenType_Closed;
    
}

bool JsonFileReader::enterNode( const he::FixedString& key, const char* /*comment*/ /*= NULL*/ )
{
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        m_Reader->pushRoot(key, val);
        return true;
    }
    return false;
}

void JsonFileReader::exitNode( const he::FixedString& key )
{
    HE_ASSERT(m_Reader->getRootName() == key, "exiting wrong node! got: %s, expected: %s", key.c_str(), m_Reader->getRootName().c_str()); key;
    m_Reader->popRoot();
}

bool JsonFileReader::enterNode( const size_t index, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    HE_IF_ASSERT(m_Reader->getRoot().isArray(), "Root must be an array to use this method!")
    {
        const json::Value& val(m_Reader->getRoot()[checked_numcast<json::ArrayIndex>(index)]);
        if (val.isNull() == false)
        {
            m_Reader->pushRoot(HEFS::strArray, val);
            result = true;
        }
    }
    return result;
}

void JsonFileReader::exitNode( const size_t /*index*/ )
{
    HE_ASSERT(m_Reader->getRootName() == HEFS::strArray, "exiting wrong node! got: %s, expected: %s", HEFS::strArray.c_str(), m_Reader->getRootName().c_str());;
    m_Reader->popRoot();
}

bool JsonFileReader::enterArray( const he::FixedString& key, const char* /*comment*/ /*= NULL*/ )
{
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isArray())
    {
        m_Reader->pushRoot(key, val);
        return true;
    }
    return false;
}

void JsonFileReader::exitArray( const he::FixedString& key )
{
    HE_ASSERT(m_Reader->getRootName() == key, "exiting wrong node! got: %s, expected: %s", key.c_str(), m_Reader->getRootName().c_str()); key;
    m_Reader->popRoot();
}

size_t JsonFileReader::getArraySize()
{
    size_t result(0);
    const json::Value& val(m_Reader->getRoot());
    if (val.isArray() == true)
    {
        result = val.size();
    }
    return result;
}


bool JsonFileReader::visit( he::String& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& currentRoot(m_Reader->getRoot());
    HE_IF_ASSERT(currentRoot.isObject() == false && currentRoot.isArray() == false, "I did not enter a value yet!")
    {
        if (currentRoot.isString())
        {
            value = currentRoot.asCString();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a string!: %s", m_Reader->getRootName().c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( bool& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& currentRoot(m_Reader->getRoot());
    HE_IF_ASSERT(currentRoot.isObject() == false && currentRoot.isArray() == false, "I did not enter a value yet!")
    {
        if (currentRoot.isBool())
        {
            value = currentRoot.asBool();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a bool!: %s", m_Reader->getRootName().c_str());
        }
    }
    return result;
}

template<typename T>
bool visitIntType(T& value, const json::Value& currentValue, const he::FixedString& key)
{
    bool result(false);
    HE_IF_ASSERT(currentValue.isObject() == false && currentValue.isArray() == false, "I did not enter a value yet!")
    {
        if (currentValue.isIntegral())
        {
            value = checked_numcast<T>(currentValue.asInt());
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a int!: %s", key.c_str());
        }
    }
    return result;
}

template<typename T>
bool visitUIntType(T& value, const json::Value& currentValue, const he::FixedString& key)
{
    bool result(false);
    HE_IF_ASSERT(currentValue.isObject() == false && currentValue.isArray() == false, "I did not enter a value yet!")
    {
        if (currentValue.isIntegral())
        {
            value = checked_numcast<T>(currentValue.asUInt());
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a uint!: %s", key.c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( int8& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( uint8& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( int16& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( uint16& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( int32& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( uint32& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( int64& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( uint64& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(value, m_Reader->getRoot(), m_Reader->getRootName());
}

bool JsonFileReader::visit( float& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& currentRoot(m_Reader->getRoot());
    HE_IF_ASSERT(currentRoot.isObject() == false && currentRoot.isArray() == false, "I did not enter a value yet!")
    {
        if (currentRoot.isDouble())
        {
            value = static_cast<float>(currentRoot.asDouble());
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a float!: %s", m_Reader->getRootName().c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( double& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& currentRoot(m_Reader->getRoot());
    HE_IF_ASSERT(currentRoot.isObject() == false && currentRoot.isArray() == false, "I did not enter a value yet!")
    {
        if (currentRoot.isDouble())
        {
            value = currentRoot.asDouble();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a double!: %s", m_Reader->getRootName().c_str());
        }
    }
    return result;
}

} } //end namespace
