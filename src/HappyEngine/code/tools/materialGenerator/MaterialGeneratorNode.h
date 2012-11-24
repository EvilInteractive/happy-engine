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
//Author:  Bastian Damman
//Created: 14/11/2012

#ifndef _HE_MaterialGeneratorNode_H_
#define _HE_MaterialGeneratorNode_H_
#pragma once

#include "NodeGraphNode.h"
#include "MaterialGeneratorNodeInOut.h"

namespace he {
namespace tools {

typedef NodeGraphNode<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorNodeBase;
typedef NodeGraphError<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorError;
typedef NodeGraphConnection<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorConnection;

class MaterialGeneratorNode : public MaterialGeneratorNodeBase
{
public:
    MaterialGeneratorNode();
    virtual ~MaterialGeneratorNode();

    virtual bool evaluate(MaterialGeneratorError& error);

    virtual bool canConnect(const MaterialGeneratorNodeOutput& fromOutput, 
                            const MaterialGeneratorNodeInput& toInput, MaterialGeneratorError& error) const;

    void addOverload(uint8 outputCount, uint8 inputCount, ...);

private:
    struct Overload
    {
        he::PrimitiveList<MaterialGeneratorVariableType> outputs;
        he::PrimitiveList<MaterialGeneratorVariableType> inputs;
    
        Overload() {}
        ~Overload() {}
        Overload(const Overload& other): outputs(other.outputs.size()), inputs(other.inputs.size()) 
        {
            outputs.append(other.outputs);
            inputs.append(other.inputs);
        }
        Overload& operator=(const Overload& other) 
        { 
            outputs.clear(); outputs.append(other.outputs); 
            inputs.clear();  inputs.append(other.inputs); 
            return *this;
        }
    };

    he::ObjectList<Overload> m_Overloads;
    uint8 m_SelectedOverload;

    void activateOverload(uint8 overload);
    bool findOverload(uint8& outOverload) const;
    
    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNode(const MaterialGeneratorNode&);
    MaterialGeneratorNode& operator=(const MaterialGeneratorNode&);
};

} } //end namespace

#endif
