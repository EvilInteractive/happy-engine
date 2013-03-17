//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
    void init(const json::Value& value)
    {
        m_Root = value;
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
        std::string file(freader.readToEnd());
        json::Value root;
        if (m_Reader->getReader().parse(file, root, true))
        {
            m_Reader->init(root);
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
    if (val.isObject())
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

bool JsonFileReader::visit( const he::FixedString& key, he::String& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isString())
        {
            value = val.asCString();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a string!: %s", key.c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( const he::FixedString& key, bool& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isBool())
        {
            value = val.asBool();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a bool!: %s", key.c_str());
        }
    }
    return result;
}

template<typename T>
bool visitIntType(JsonFileReader::Reader* const reader, const he::FixedString& key, T& value)
{
    bool result(false);
    const json::Value& val(reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isIntegral())
        {
            value = checked_numcast<T>(val.asInt());
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
bool visitUIntType(JsonFileReader::Reader* const reader, const he::FixedString& key, T& value)
{
    bool result(false);
    const json::Value& val(reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isIntegral())
        {
            value = checked_numcast<T>(val.asUInt());
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a uint!: %s", key.c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( const he::FixedString& key, int8& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, uint8& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, int16& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, uint16& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, int32& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, uint32& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, int64& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, uint64& value, const char* /*comment*/ /*= NULL*/ )
{
    return visitUIntType(m_Reader, key, value);
}

bool JsonFileReader::visit( const he::FixedString& key, float& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isDouble())
        {
            value = static_cast<float>(val.asDouble());
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a float!: %s", key.c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( const he::FixedString& key, double& value, const char* /*comment*/ /*= NULL*/ )
{
    bool result(false);
    const json::Value& val(m_Reader->getRoot()[key.c_str()]);
    if (val.isNull() == false)
    {
        if (val.isDouble())
        {
            value = val.asDouble();
            result = true;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Trying to get value witch is not a double!: %s", key.c_str());
        }
    }
    return result;
}

bool JsonFileReader::visit( const he::FixedString& key, vec2& value, const char* /*comment*/ /*= NULL*/ )
{
    if (enterNode(key))
    {
        visit(HEFS::strX, value.x);
        visit(HEFS::strY, value.y);
        exitNode(key);
        return true;
    }
    return false;
}

bool JsonFileReader::visit( const he::FixedString& key, vec3& value, const char* /*comment*/ /*= NULL*/ )
{
    if (enterNode(key))
    {
        visit(HEFS::strX, value.x);
        visit(HEFS::strY, value.y);
        visit(HEFS::strZ, value.z);
        exitNode(key);
        return true;
    }
    return false;
}

bool JsonFileReader::visit( const he::FixedString& key, vec4& value, const char* /*comment*/ /*= NULL*/ )
{
    if (enterNode(key))
    {
        visit(HEFS::strX, value.x);
        visit(HEFS::strY, value.y);
        visit(HEFS::strZ, value.z);
        visit(HEFS::strW, value.w);
        exitNode(key);
        return true;
    }
    return false;
}

bool JsonFileReader::visit( const he::FixedString& key, Guid& value, const char* /*comment*/ /*= NULL*/ )
{
    std::string guid;
    if (visit(key, guid))
    {
        value = Guid(guid.c_str());
        return true;
    }
    return false;
}


} } //end namespace
