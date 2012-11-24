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
#include "HappyPCH.h" 

#include "MaterialGeneratorNode.h"

namespace he {
namespace tools {

MaterialGeneratorNode::MaterialGeneratorNode(): m_SelectedOverload(0), m_Overloads(1)
{
}


MaterialGeneratorNode::~MaterialGeneratorNode()
{
}

bool MaterialGeneratorNode::canConnect( const MaterialGeneratorNodeOutput& fromOutput, 
                                        const MaterialGeneratorNodeInput& toInput, MaterialGeneratorError& error ) const
{
    bool result(true);

    // Overloads are impossible to type check, so they are checked @compile time
    if (fromOutput.getType() != toInput.getType() && m_Overloads.size() == 1) // types don't match and we have no overloads
    {
        error.setMessage("Type %s is not compatible with %s", 
            materialGeneratorVariableTypeToString(fromOutput.getType()), 
            materialGeneratorVariableTypeToString(toInput.getType()));
        result = false;
    }
    return result;
}

void MaterialGeneratorNode::addOverload( uint8 outputCount, uint8 inputCount, ... )
{
    HE_ASSERT(m_Overloads.size() != 0 && m_Overloads[0].outputs.size() == outputCount && m_Overloads[0].inputs.size() == inputCount, 
        "Incompatible amount of inputs or outputs supplied with overload:\n Outputs: %d/%d\n Inputs: %d/%d", 
        outputCount, m_Overloads[0].outputs.size(), inputCount, m_Overloads[0].inputs.size());

    Overload overload;
    overload.outputs.reserve(outputCount);
    overload.inputs.reserve(inputCount);

    va_list argList;
    va_start(argList, inputCount);
    for (uint8 i(0); i < outputCount; ++i)
    {
        MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
        overload.outputs.add(type);
    }
    for (uint8 i(0); i < inputCount; ++i)
    {
        MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
        overload.inputs.add(type);
    }
    va_end(argList);

    m_Overloads.add(overload);

    if (m_Overloads.size() == 1)
    {
        for (uint8 i(0); i < outputCount; ++i)
        {
            addOutput(MaterialGeneratorNodeOutput());
        }
        for (uint8 i(0); i < inputCount; ++i)
        {
            addInput(MaterialGeneratorNodeInput());
        }
        activateOverload(0);
    }
}

bool MaterialGeneratorNode::evaluate( MaterialGeneratorError& error )
{
    bool result(false);
    uint8 overload(UINT8_MAX);
    if (findOverload(overload))
    {
        result = true;
        activateOverload(overload);
    }
    else
    {
        error.setMessage("Error! Incorrect input types, could not find compatible overload!");
        result = false;
    }
    return result;
}

void MaterialGeneratorNode::activateOverload( uint8 overloadIndex )
{
    HE_IF_ASSERT(overloadIndex < m_Overloads.size(), "Overload out of range!")
    {
        uint8 outputCount(getOutputCount());
        uint8 inputCount(getInputCount());
        Overload& overload(m_Overloads[overloadIndex]);

        for (uint8 i(0); i < outputCount; ++i)
        {
            MaterialGeneratorNodeOutput& output(getOutput(i));
            output.setType(overload.outputs[i]);
        }
        for (uint8 i(0); i < inputCount; ++i)
        {
            HE_ASSERT(getInputConnection(i).isConnected() == false || getInputConnection(i).getConnection().getType() == overload.inputs[i], "Incompatible overload assigned");
            MaterialGeneratorNodeInput& input(getInput(i));
            input.setType(overload.inputs[i]);
        }
    }
}

bool MaterialGeneratorNode::findOverload( uint8& outOverload ) const
{
    bool result(false);
    const uint8 inputCount(getInputCount());

    const size_t overloadCount(m_Overloads.size());
    for (uint8 i(0); i < overloadCount && result == false; ++i)
    {
        const Overload& overload(m_Overloads[i]);
        result = true;
        outOverload = i;
        for (uint8 inputIndex(0); inputIndex < inputCount; ++inputIndex)
        {
            const MaterialGeneratorConnection& connection(getInputConnection(inputIndex));
            if (connection.isConnected() == true && // if input is connected
                getInputConnection(inputIndex).getConnection().getType() != overload.inputs[inputIndex]) // if inputs type is not compatible with overload
            {
                result = false; // result = false -> try other overload
                break;
            }
        }
    }
    return result;
}

} } //end namespace
