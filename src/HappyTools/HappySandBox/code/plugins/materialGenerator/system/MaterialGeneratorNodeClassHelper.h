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
//Created: 15/12/2012

#ifndef _HE_MaterialGeneratorNodeClassHelpers_H_
#define _HE_MaterialGeneratorNodeClassHelpers_H_
#pragma once

#include "MaterialGeneratorNode.h"

namespace hs {
class MaterialGeneratorGraph;

#define _MaterialGeneratorNodeClass(className, type, destroyImpl, extra, extraCtor) \
class className : public MaterialGeneratorNode\
{\
public:\
    explicit className(MaterialGraph* const parent) : MaterialGeneratorNode(parent) extraCtor {}\
    ~className() {}\
    \
    void init() final; \
    void destroy() final destroyImpl \
    \
    bool evaluate() final;\
    \
    const he::FixedString& getType() const final { return HSFS::str##type; } \
    \
private:\
    extra\
    \
    className(const className&);\
    className& operator=(const className&);\
};

#define MaterialGeneratorNodeCOMMA ,
#define MaterialGeneratorNodeClass(type) _MaterialGeneratorNodeClass(MaterialGeneratorNode##type, type, { MaterialGeneratorNode::destroy(); },,)
#define MaterialGeneratorNodeClassTempVar(type, var) _MaterialGeneratorNodeClass(MaterialGeneratorNode##type, type,;, he::ObjectHandle var;, MaterialGeneratorNodeCOMMA var(he::ObjectHandle::unassigned))

}

#endif
