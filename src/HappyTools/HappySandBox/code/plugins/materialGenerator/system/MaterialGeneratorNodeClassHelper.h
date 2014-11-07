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
    className() extraCtor {}\
    ~className() {}\
    \
    void init() final; \
    void destroy() final destroyImpl \
    \
    bool evaluate() final;\
    \
    MaterialGeneratorNodeType getType() const final { return MaterialGeneratorNodeType_##type; } \
    \
private:\
    extra\
    \
    className(const className&);\
    className& operator=(const className&);\
};

#define MaterialGeneratorNodeClass(type) _MaterialGeneratorNodeClass(MaterialGeneratorNode##type, type, { MaterialGeneratorNode::destroy(); },,)
#define MaterialGeneratorNodeClassTempVar(type, var) _MaterialGeneratorNodeClass(MaterialGeneratorNode##type, type,;, he::ObjectHandle var;, : var(he::ObjectHandle::unassigned))

#define MGO_IN(count, ...) count, __VA_ARGS__
#define MGO_OUT(count, ...) count, __VA_ARGS__
#define MGO_ADD_OVERLOAD_IO(inputs, outputs)\
{\
    MaterialGeneratorNodeOverload overload;\
    overload.setInputs(inputs);\
    overload.setOutputs(outputs);\
    addOverload(std::move(overload));\
}
#define MGO_ADD_OVERLOAD_I(inputs)\
{\
    MaterialGeneratorNodeOverload overload;\
    overload.setInputs(inputs);\
    addOverload(std::move(overload));\
}
#define MGO_ADD_OVERLOAD_O(outputs)\
{\
    MaterialGeneratorNodeOverload overload;\
    overload.setOutputs(outputs);\
    addOverload(std::move(overload));\
}

}

#endif
