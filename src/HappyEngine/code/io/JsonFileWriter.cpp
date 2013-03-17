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

#include "JsonFileWriter.h"

#include <json/json.h>
#include "FileWriter.h"

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
        m_Root = json::Value();
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

void JsonFileWriter::open( const Path& path )
{
    m_SavePath = path;
    m_Writer->init();
    m_OpenType = StructuredVisitor::eOpenType_Write;
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
    bool result(true);
    json::Value& root(m_Writer->getRoot());
    json::Value& val(root[key.c_str()] = json::Value(json::objectValue));
    if (comment != NULL)
    {
        val.setComment(comment, json::commentAfterOnSameLine);
    }
    m_Writer->pushRoot(key, val);
    return result;
}

void JsonFileWriter::exitNode( const he::FixedString& key )
{
    HE_ASSERT(m_Writer->getRootName() == key, "exiting wrong node! got: %s, expected: %s", key.c_str(), m_Writer->getRootName().c_str()); key;
    m_Writer->popRoot();
}


template<typename T>
bool visitDefaultType(JsonFileWriter::Writer* const writer, const he::FixedString& key, T& value, const char* comment = NULL)
{
    bool result(true);
    json::Value& root(writer->getRoot());
    json::Value& val(root[key.c_str()] = value);
    if (comment != NULL)
    {
        val.setComment(comment, json::commentBefore);
    }
    return result;
}

bool JsonFileWriter::visit( const he::FixedString& key, he::String& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, bool& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, int8& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, uint8& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, int16& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, uint16& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, int32& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, uint32& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, int64& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, uint64& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, float& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, double& value, const char* comment /*= NULL*/ )
{
    return visitDefaultType(m_Writer, key, value, comment);
}

bool JsonFileWriter::visit( const he::FixedString& key, vec2& value, const char* comment /*= NULL*/ )
{
    if (enterNode(key, comment))
    {
        visit(HEFS::strX, value.x);
        visit(HEFS::strY, value.y);
        exitNode(key);
        return true;
    }
    return false;
}

bool JsonFileWriter::visit( const he::FixedString& key, vec3& value, const char* comment /*= NULL*/ )
{
    if (enterNode(key, comment))
    {
        visit(HEFS::strX, value.x);
        visit(HEFS::strY, value.y);
        visit(HEFS::strZ, value.z);
        exitNode(key);
        return true;
    }
    return false;
}

bool JsonFileWriter::visit( const he::FixedString& key, vec4& value, const char* comment /*= NULL*/ )
{
    if (enterNode(key, comment))
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

bool JsonFileWriter::visit( const he::FixedString& key, Guid& value, const char* comment /*= NULL*/ )
{
    std::string guid(value.toString());
    return visit(key, guid, comment);
}


} } //end namespace
