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
        if (type == HSFS::strRootNormalDraw) node = NEW MaterialGeneratorNodeRootNormalDraw(graph); 

        // Const
        else if (type == HSFS::strFloat1) node = NEW MaterialGeneratorNodeFloat1(graph); 
        else if (type == HSFS::strFloat2) node = NEW MaterialGeneratorNodeFloat2(graph); 
        else if (type == HSFS::strFloat3) node = NEW MaterialGeneratorNodeFloat3(graph); 
        else if (type == HSFS::strFloat4) node = NEW MaterialGeneratorNodeFloat4(graph); 

        // Globals
        //else if (type == HSFS::strPosition) node = NEW MaterialGeneratorNodeFloat1(graph); 
        //else if (type == HSFS::strNormal) node = NEW MaterialGeneratorNodeFloat1(graph); 

        // Vector
        else if (type == HSFS::strSwizzle) node = NEW MaterialGeneratorNodeSwizzle(graph); 
        else if (type == HSFS::strComposeVector) node = NEW MaterialGeneratorNodeComposeVector(graph); 

        // Math
        // Basic
        else if (type == HSFS::strAbs) node = NEW MaterialGeneratorNodeAbs(graph); 
        else if (type == HSFS::strAdd) node = NEW MaterialGeneratorNodeAdd(graph); 
        else if (type == HSFS::strCos) node = NEW MaterialGeneratorNodeCos(graph); 
        else if (type == HSFS::strDivide) node = NEW MaterialGeneratorNodeDivide(graph); 
        else if (type == HSFS::strMultiply) node = NEW MaterialGeneratorNodeMultiply(graph); 
        else if (type == HSFS::strSin) node = NEW MaterialGeneratorNodeSin(graph); 
        else if (type == HSFS::strSubtract) node = NEW MaterialGeneratorNodeSubtract(graph); 
        // Func
        else if (type == HSFS::strCeil) node = NEW MaterialGeneratorNodeCeil(graph); 
        else if (type == HSFS::strClamp) node = NEW MaterialGeneratorNodeClamp(graph); 
        else if (type == HSFS::strCross) node = NEW MaterialGeneratorNodeCross(graph); 
        else if (type == HSFS::strDistance) node = NEW MaterialGeneratorNodeDistance(graph); 
        else if (type == HSFS::strDistanceSqr) node = NEW MaterialGeneratorNodeDistanceSqr(graph); 
        else if (type == HSFS::strDot) node = NEW MaterialGeneratorNodeDot(graph); 
        else if (type == HSFS::strFloor) node = NEW MaterialGeneratorNodeFloor(graph); 
        else if (type == HSFS::strFrac) node = NEW MaterialGeneratorNodeFrac(graph); 
        else if (type == HSFS::strLerp) node = NEW MaterialGeneratorNodeLerp(graph); 
        else if (type == HSFS::strMin) node = NEW MaterialGeneratorNodeMin(graph); 
        else if (type == HSFS::strMax) node = NEW MaterialGeneratorNodeMax(graph); 
        else if (type == HSFS::strOneMin) node = NEW MaterialGeneratorNodeOneMin(graph); 
        else if (type == HSFS::strNormalize) node = NEW MaterialGeneratorNodeNormalize(graph); 
        else if (type == HSFS::strPower) node = NEW MaterialGeneratorNodePower(graph); 
        else if (type == HSFS::strReflect) node = NEW MaterialGeneratorNodeReflect(graph); 
        else if (type == HSFS::strSign) node = NEW MaterialGeneratorNodeSign(graph); 

        // Texture
        //else if (type == HSFS::strFlipBook) node = NEW MaterialGeneratorNodeFlipBook(graph); 
        //else if (type == HSFS::strPanner) node = NEW MaterialGeneratorNodePanner(graph); 
        //else if (type == HSFS::strRotator) node = NEW MaterialGeneratorNodeRotator(graph); 
        //else if (type == HSFS::strTexture2D) node = NEW MaterialGeneratorNodeTexture2D(graph); 
        //else if (type == HSFS::strTextureCube) node = NEW MaterialGeneratorNodeTextureCube(graph); 
        //else if (type == HSFS::strTexcoord) node = NEW MaterialGeneratorNodeTexcoord(graph); 

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
