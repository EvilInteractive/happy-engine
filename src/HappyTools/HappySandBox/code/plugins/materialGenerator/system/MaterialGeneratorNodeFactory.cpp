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
#include "HappySandBoxPCH.h" 

#include "MaterialGeneratorNodeFactory.h"

#include "MaterialGeneratorNodeTypes.h"
#include "MaterialGeneratorNode.h"

#include "MaterialGeneratorConstNodes.h"
#include "MaterialGeneratorMathNodes.h"
#include "MaterialGeneratorTextureNodes.h"
#include "MaterialGeneratorRootNodes.h"
#include "MaterialGeneratorVectorNodes.h"

namespace hs {

MaterialGeneratorNodeFactory::MaterialGeneratorNodeFactory()
{
    init(10, 10, "MaterialGeneratorNodeFactory");
}

MaterialGeneratorNodeFactory::~MaterialGeneratorNodeFactory()
{
}

MaterialGeneratorNode* MaterialGeneratorNodeFactory::create( MaterialGraph* const graph, const he::FixedString& type )
{
    return create(graph, type, he::Guid::generateGuid());
}

MaterialGeneratorNode* MaterialGeneratorNodeFactory::create( MaterialGraph* const graph, const he::FixedString& type, const he::Guid& id )
{
    MaterialGeneratorNode* node(nullptr);
    {
        if (type == HSFS::strRootNormalDraw) node = HENew(MaterialGeneratorNodeRootNormalDraw)(graph); 

        // Const
        else if (type == HSFS::strFloat1) node = HENew(MaterialGeneratorNodeFloat1)(graph); 
        else if (type == HSFS::strFloat2) node = HENew(MaterialGeneratorNodeFloat2)(graph); 
        else if (type == HSFS::strFloat3) node = HENew(MaterialGeneratorNodeFloat3)(graph); 
        else if (type == HSFS::strFloat4) node = HENew(MaterialGeneratorNodeFloat4)(graph); 

        // Globals
        //else if (type == HSFS::strPosition) node = HENew(MaterialGeneratorNodeFloat1)(graph); 
        //else if (type == HSFS::strNormal) node = HENew(MaterialGeneratorNodeFloat1)(graph); 

        // Vector
        else if (type == HSFS::strSwizzle) node = HENew(MaterialGeneratorNodeSwizzle)(graph); 
        else if (type == HSFS::strComposeVector) node = HENew(MaterialGeneratorNodeComposeVector)(graph); 

        // Math
        // Basic
        else if (type == HSFS::strAbs) node = HENew(MaterialGeneratorNodeAbs)(graph); 
        else if (type == HSFS::strAdd) node = HENew(MaterialGeneratorNodeAdd)(graph); 
        else if (type == HSFS::strCos) node = HENew(MaterialGeneratorNodeCos)(graph); 
        else if (type == HSFS::strDivide) node = HENew(MaterialGeneratorNodeDivide)(graph); 
        else if (type == HSFS::strMultiply) node = HENew(MaterialGeneratorNodeMultiply)(graph); 
        else if (type == HSFS::strSin) node = HENew(MaterialGeneratorNodeSin)(graph); 
        else if (type == HSFS::strSubtract) node = HENew(MaterialGeneratorNodeSubtract)(graph); 
        // Func
        else if (type == HSFS::strCeil) node = HENew(MaterialGeneratorNodeCeil)(graph); 
        else if (type == HSFS::strClamp) node = HENew(MaterialGeneratorNodeClamp)(graph); 
        else if (type == HSFS::strCross) node = HENew(MaterialGeneratorNodeCross)(graph); 
        else if (type == HSFS::strDistance) node = HENew(MaterialGeneratorNodeDistance)(graph); 
        else if (type == HSFS::strDistanceSqr) node = HENew(MaterialGeneratorNodeDistanceSqr)(graph); 
        else if (type == HSFS::strDot) node = HENew(MaterialGeneratorNodeDot)(graph); 
        else if (type == HSFS::strFloor) node = HENew(MaterialGeneratorNodeFloor)(graph); 
        else if (type == HSFS::strFrac) node = HENew(MaterialGeneratorNodeFrac)(graph); 
        else if (type == HSFS::strLerp) node = HENew(MaterialGeneratorNodeLerp)(graph); 
        else if (type == HSFS::strMin) node = HENew(MaterialGeneratorNodeMin)(graph); 
        else if (type == HSFS::strMax) node = HENew(MaterialGeneratorNodeMax)(graph); 
        else if (type == HSFS::strOneMin) node = HENew(MaterialGeneratorNodeOneMin)(graph); 
        else if (type == HSFS::strNormalize) node = HENew(MaterialGeneratorNodeNormalize)(graph); 
        else if (type == HSFS::strPower) node = HENew(MaterialGeneratorNodePower)(graph); 
        else if (type == HSFS::strReflect) node = HENew(MaterialGeneratorNodeReflect)(graph); 
        else if (type == HSFS::strSign) node = HENew(MaterialGeneratorNodeSign)(graph); 

        // Texture
        //else if (type == HSFS::strFlipBook) node = HENew(MaterialGeneratorNodeFlipBook)(graph); 
        //else if (type == HSFS::strPanner) node = HENew(MaterialGeneratorNodePanner)(graph); 
        //else if (type == HSFS::strRotator) node = HENew(MaterialGeneratorNodeRotator)(graph); 
        //else if (type == HSFS::strTexture2D) node = HENew(MaterialGeneratorNodeTexture2D)(graph); 
        //else if (type == HSFS::strTextureCube) node = HENew(MaterialGeneratorNodeTextureCube)(graph); 
        //else if (type == HSFS::strTexcoord) node = HENew(MaterialGeneratorNodeTexcoord)(graph); 

        else { LOG(he::LogType_ProgrammerAssert, "Unknown node in factory: %s", type.c_str()); }
    }
    registerObject(node);
    node->init();
    node->setGuid(id);
    return node;
}

void MaterialGeneratorNodeFactory::destroy( MaterialGeneratorNode* const node )
{
    destroyObject(node->getHandle());
}

} //end namespace
