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
//Created: 22/12/2012

#ifndef _HE_MaterialGeneratorRootNodes_H_
#define _HE_MaterialGeneratorRootNodes_H_
#pragma once

#include "MaterialGeneratorNodeClassHelper.h"

namespace he {
namespace ct {
    class ShaderGeneratorVariable;
}
}

namespace hs {

class MaterialGeneratorNodeRootNormalDraw : public MaterialGeneratorNode
{
    public:
    MaterialGeneratorNodeRootNormalDraw();
    ~MaterialGeneratorNodeRootNormalDraw() {}
    
    void init() final; 
    void destroy() final;    
    bool evaluate() final;
    
    MaterialGeneratorNodeType getType() const final { return MaterialGeneratorNodeType_RootNormalDraw; } 
    
private:
    bool areConnectionsValid();
    void removeAllTempVars();
    he::ct::ShaderGeneratorVariable* addTempVar(const char* nameHint);
    he::ObjectList<he::ObjectHandle> m_TempVars;
    
    MaterialGeneratorNodeRootNormalDraw(const MaterialGeneratorNodeRootNormalDraw&);
    MaterialGeneratorNodeRootNormalDraw& operator=(const MaterialGeneratorNodeRootNormalDraw&);
};


} //end namespace

#endif
