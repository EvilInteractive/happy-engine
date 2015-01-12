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

#include "JsonFileWriter.h"

#include <json/json.h>
#include "FileWriter.h"
#include "GlobalStringTable.h"

namespace he {
namespace io {

class JsonFileWriter::Writer
{
    struct RootStackItem
    {
        RootStackItem() 
            : m_CurrentRoot(nullptr)
            , m_CurrentRootName(HEFS::str)
        {
        }
        RootStackItem(const FixedString& rootName, json::Value* root)
            : m_CurrentRoot(root)
            , m_CurrentRootName(rootName)
        {
        }
        ~RootStackItem() {}
        json::Value* m_CurrentRoot;
        FixedString m_CurrentRootName;
    };

public:

    inline json::Value& getRoot() { return *m_RootStack.back().m_CurrentRoot; }
    inline const FixedString& getRootName() const { return m_RootStack.back().m_CurrentRootName; }
    inline json::StyledWriter& getWriter() { return m_Writer; }
    inline void pushRoot(const FixedString& rootName, json::Value& root)
    {
        m_RootStack.add(RootStackItem(rootName, &root));
    }
    inline void popRoot() 
    {
        HE_ASSERT(m_RootStack.empty() == false, "No more roots to pop!");
        m_RootStack.removeAt(m_RootStack.size() - 1);
    }
    inline bool isTopRoot() const { return m_RootStack.size() == 1; }

    void init()
    {
        m_Root = json::Value(json::objectValue);
        m_RootStack.clear();
        pushRoot(HEFS::str, m_Root);
    }

private:
    json::StyledWriter m_Writer;
    ObjectList<RootStackItem> m_RootStack;
    json::Value m_Root;
};

JsonFileWriter::JsonFileWriter()
    : m_Writer(NEW Writer())
    , m_SavePath("")
{
}

JsonFileWriter::~JsonFileWriter()
{
    HE_ASSERT(m_OpenType == StructuredVisitor::eOpenType_Closed, "Stream is still open when disposing JsonFileWriter!");
    delete m_Writer;
}

bool JsonFileWriter::open( const Path& path )
{
    m_SavePath = path;
    m_Writer->init();
    m_OpenType = StructuredVisitor::eOpenType_Write;
    return true;
}

void JsonFileWriter::close()
{
    HE_ASSERT(m_OpenType != StructuredVisitor::eOpenType_Closed, "JsonFileWriter not open!");
    m_OpenType = StructuredVisitor::eOpenType_Closed;
    io::FileWriter writer;
    if (writer.open(m_SavePath, false))
    {
        HE_ASSERT(m_Writer->isTopRoot(), "Closing file when not on the top root!");
        writer << m_Writer->getWriter().write(m_Writer->getRoot());
        writer.close();
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "File save failed! %s", m_SavePath.str().c_str());
    }
}

bool JsonFileWriter::enterNode( const he::FixedString& key, const char* comment /*= NULL*/ )
{
    bool result(false);
    json::Value& root(m_Writer->getRoot());
    HE_IF_ASSERT(root.isNull() || root.isArray() == false, "Root can not be of type array!")
    {
        json::Value& val(root[key.c_str()] = json::Value(json::objectValue));
        if (comment != NULL)
        {
            val.setComment(comment, json::commentAfterOnSameLine);
        }
        m_Writer->pushRoot(key, val);
        result = true;
    }
    return result;
}

void JsonFileWriter::exitNode( const he::FixedString& key )
{
    HE_ASSERT(m_Writer->getRootName() == key, "exiting wrong node! got: %s, expected: %s", key.c_str(), m_Writer->getRootName().c_str()); key;
    m_Writer->popRoot();
}

bool JsonFileWriter::enterNode( const size_t index, const char* comment /*= NULL*/ )
{
    bool result(true);
    json::Value& root(m_Writer->getRoot());
    HE_IF_ASSERT(root.isArray(), "Root must be of type array!")
    {
        json::Value& val(root[index] = json::Value(json::objectValue));
        if (comment != NULL)
        {
            val.setComment(comment, json::commentAfterOnSameLine);
        }
        m_Writer->pushRoot(HEFS::strArray, val);
    }
    return result;
}

void JsonFileWriter::exitNode( const size_t /*index*/ )
{
    HE_ASSERT(m_Writer->getRootName() == HEFS::strArray, "exiting wrong node! got: %s, expected: %s", HEFS::strArray.c_str(), m_Writer->getRootName().c_str());
    m_Writer->popRoot();
}

bool JsonFileWriter::enterArray( const he::FixedString& key, const char* comment /*= NULL*/ )
{
    bool result(true);
    json::Value& root(m_Writer->getRoot());
    json::Value* val(nullptr);
    if (root.isArray())
    {
        val = &root.append(json::Value(json::arrayValue));
    }
    else
    {
        val = &(root[key.c_str()] = json::Value(json::arrayValue));
    }
    if (comment != NULL)
    {
        val->setComment(comment, json::commentAfterOnSameLine);
    }
    m_Writer->pushRoot(key, *val);
    return result;
}

void JsonFileWriter::exitArray( const he::FixedString& key )
{
    HE_ASSERT(m_Writer->getRootName() == key, "exiting wrong node! got: %s, expected: %s", key.c_str(), m_Writer->getRootName().c_str()); key;
    m_Writer->popRoot();
}

size_t JsonFileWriter::getArraySize()
{
    const json::Value& val(m_Writer->getRoot());
    size_t result(val.size());
    return result;
}

he::FixedString JsonFileWriter::getMemberName( const size_t index )
{
    const json::Value& val(m_Writer->getRoot());
    return he::GlobalStringTable::getInstance()->add(val.getMemberNames()[index].c_str());
}

template<typename T>
bool visitDefaultType(JsonFileWriter::Writer* const writer, T& value, const char* comment = NULL)
{
    bool result(true);
    json::Value& root(writer->getRoot());
    json::Value* val(nullptr);
    if (root.isNull() == false && root.isArray())
    {
        val = &root.append(value);
    }
    else
    {
        val = &(root = value);
    }
    if (comment != NULL)
    {
        val->setComment(comment, json::commentAfterOnSameLine);
    }
    return result;
}

bool JsonFileWriter::visit( he::String& value, const char* comment /*= NULL*/ )
{
    const char* str(value.c_str());
    return visitDefaultType(m_Writer, str, comment);
}

bool JsonFileWriter::visit( bool& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( int8& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( uint8& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( int16& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( uint16& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( int32& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( uint32& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( int64& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( uint64& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( float& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

bool JsonFileWriter::visit( double& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, value, comment);
}

} } //end namespace
