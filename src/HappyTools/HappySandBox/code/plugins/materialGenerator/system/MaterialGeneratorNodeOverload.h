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
//Created: 2014/10/31
#ifndef _HE_MaterialGeneratorNodeOverload_H_
#define _HE_MaterialGeneratorNodeOverload_H_
#pragma once

#include "MaterialGeneratorNodeTypes.h"

namespace hs {
class MaterialGeneratorNodeOverload
{
public:
    typedef he::PrimitiveList<MaterialGeneratorVariableType> TTypeList;

    MaterialGeneratorNodeOverload();
    ~MaterialGeneratorNodeOverload();
    MaterialGeneratorNodeOverload(MaterialGeneratorNodeOverload&& other);
    MaterialGeneratorNodeOverload& operator=(MaterialGeneratorNodeOverload&& other);

    void setInputs(he::uint8 inputCount, ...);   // Takes MaterialGeneratorVariableType's
    void setOutputs(he::uint8 outputCount, ...);   // Takes MaterialGeneratorVariableType's
    
    const TTypeList& getOutputs() const { return m_Outputs; }
    const TTypeList& getInputs() const { return m_Inputs; }

private:
    TTypeList m_Outputs;
    TTypeList m_Inputs;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNodeOverload(const MaterialGeneratorNodeOverload&);
    MaterialGeneratorNodeOverload& operator=(const MaterialGeneratorNodeOverload&);
};

} //end namespace

#endif
