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
#include "HappySandBoxPCH.h"
#include "MaterialGeneratorNodeOverload.h"

namespace hs {


    MaterialGeneratorNodeOverload::MaterialGeneratorNodeOverload()
    {

    }

    MaterialGeneratorNodeOverload::~MaterialGeneratorNodeOverload()
    {

    }

    MaterialGeneratorNodeOverload::MaterialGeneratorNodeOverload( MaterialGeneratorNodeOverload&& other ) 
        : m_Outputs(std::move(other.m_Outputs))
        , m_Inputs(std::move(other.m_Inputs))
    {

    }

    MaterialGeneratorNodeOverload& MaterialGeneratorNodeOverload::operator=( MaterialGeneratorNodeOverload&& other )
    {
        m_Outputs = std::move(other.m_Outputs);
        m_Inputs = std::move(other.m_Inputs);
        return *this;
    }

    void MaterialGeneratorNodeOverload::setInputs( he::uint8 inputCount, ... )
    {
        m_Inputs.reserve(inputCount);

        va_list argList;
        va_start(argList, inputCount);
        for (he::uint8 i(0); i < inputCount; ++i)
        {
            MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
            m_Inputs.add(type);
        }
        va_end(argList);
    }

    void MaterialGeneratorNodeOverload::setOutputs( he::uint8 outputCount, ... )
    {
        m_Outputs.reserve(outputCount);

        va_list argList;
        va_start(argList, outputCount);
        for (he::uint8 i(0); i < outputCount; ++i)
        {
            MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
            m_Outputs.add(type);
        }
        va_end(argList);
    }


} //end namespace
