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
//Created: 14/11/2012

#ifndef _HE_MaterialGeneratorNodeInOut_H_
#define _HE_MaterialGeneratorNodeInOut_H_
#pragma once

#include "MaterialGeneratorNodeTypes.h"

namespace he {
namespace tools {
class MaterialGeneratorNode;
class MaterialGeneratorNodeInput
{
public:
    MaterialGeneratorNodeInput()
        : m_Type(MaterialGeneratorVariableType_Unknown)
        , m_Parent(nullptr) 
        , m_Index(UINT8_MAX)
    {}
    MaterialGeneratorNodeInput(MaterialGeneratorNode* const parent, const uint8 index)
        : m_Type(MaterialGeneratorVariableType_Unknown)
        , m_Parent(parent)
        , m_Index(index) 
    {}
    virtual ~MaterialGeneratorNodeInput() {}
    // Default copy == OK

    inline void setType(const MaterialGeneratorVariableType type) { m_Type = type; }
    inline MaterialGeneratorVariableType getType() const { return m_Type; }

    inline MaterialGeneratorNode* getParent() const { return m_Parent; }

    inline uint8 getIndex() const { return m_Index; }

private:

    MaterialGeneratorVariableType m_Type;
    MaterialGeneratorNode* m_Parent;
    uint8 m_Index;
};

class MaterialGeneratorNodeOutput
{
public:
    MaterialGeneratorNodeOutput()
        : m_Type(MaterialGeneratorVariableType_Unknown)
        , m_Variable(ObjectHandle::unassigned)
        , m_Parent(nullptr) 
        , m_Index(UINT8_MAX)
    {}
    MaterialGeneratorNodeOutput(MaterialGeneratorNode* const parent, const uint8 index)
        : m_Type(MaterialGeneratorVariableType_Unknown)
        , m_Variable(ObjectHandle::unassigned)
        , m_Parent(parent) 
        , m_Index(index)
    {}
    ~MaterialGeneratorNodeOutput() {}
    // Default copy == OK

    inline void setType(const MaterialGeneratorVariableType type) { m_Type = type; }
    inline MaterialGeneratorVariableType getType() const { return m_Type; }

    inline const ObjectHandle& getVar() const { return m_Variable; }
    inline void setVar(const ObjectHandle& var) { m_Variable = var; }

    inline MaterialGeneratorNode* getParent() const { return m_Parent; }

    inline uint8 getIndex() const { return m_Index; }

private:

    MaterialGeneratorVariableType m_Type;
    ObjectHandle m_Variable;
    MaterialGeneratorNode* m_Parent;
    uint8 m_Index;
};


} } //end namespace

#endif
