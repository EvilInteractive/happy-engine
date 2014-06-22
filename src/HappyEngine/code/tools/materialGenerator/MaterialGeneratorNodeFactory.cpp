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
#include "HappyPCH.h" 

#include "MaterialGeneratorNodeFactory.h"

#include "MaterialGeneratorNodeTypes.h"
#include "MaterialGeneratorNode.h"

#include "MaterialGeneratorConstNodes.h"
#include "MaterialGeneratorMathNodes.h"
#include "MaterialGeneratorTextureNodes.h"
#include "MaterialGeneratorRootNodes.h"
#include "MaterialGeneratorVectorNodes.h"

namespace he {
namespace tools {

MaterialGeneratorNodeFactory::MaterialGeneratorNodeFactory()
{
    init(10, 10, "MaterialGeneratorNodeFactory");
}

MaterialGeneratorNodeFactory::~MaterialGeneratorNodeFactory()
{
}

MaterialGeneratorNode* MaterialGeneratorNodeFactory::create( const MaterialGeneratorNodeType type )
{
    return create(type, Guid::generateGuid());
}

MaterialGeneratorNode* MaterialGeneratorNodeFactory::create( const MaterialGeneratorNodeType type, const Guid& id )
{
    MaterialGeneratorNode* node(nullptr);
    switch (type)
    {
        // Const
        case MaterialGeneratorNodeType_RootNormalDraw: node = NEW MaterialGeneratorNodeRootNormalDraw(); break;

        // Const
        case MaterialGeneratorNodeType_Float1: node = NEW MaterialGeneratorNodeFloat1(); break;
        case MaterialGeneratorNodeType_Float2: node = NEW MaterialGeneratorNodeFloat2(); break;
        case MaterialGeneratorNodeType_Float3: node = NEW MaterialGeneratorNodeFloat3(); break;
        case MaterialGeneratorNodeType_Float4: node = NEW MaterialGeneratorNodeFloat4(); break;

        // Globals
        //case MaterialGeneratorNodeType_Position: node = NEW MaterialGeneratorNodeFloat1(); break;
        //case MaterialGeneratorNodeType_Normal: node = NEW MaterialGeneratorNodeFloat1(); break;

        // Vector
        case MaterialGeneratorNodeType_Swizzle: node = NEW MaterialGeneratorNodeSwizzle(); break;
        case MaterialGeneratorNodeType_ComposeVector: node = NEW MaterialGeneratorNodeComposeVector(); break;

        // Math
        // Basic
        case MaterialGeneratorNodeType_Abs: node = NEW MaterialGeneratorNodeAbs(); break;
        case MaterialGeneratorNodeType_Add: node = NEW MaterialGeneratorNodeAdd(); break;
        case MaterialGeneratorNodeType_Cos: node = NEW MaterialGeneratorNodeCos(); break;
        case MaterialGeneratorNodeType_Divide: node = NEW MaterialGeneratorNodeDivide(); break;
        case MaterialGeneratorNodeType_Multiply: node = NEW MaterialGeneratorNodeMultiply(); break;
        case MaterialGeneratorNodeType_Sin: node = NEW MaterialGeneratorNodeSin(); break;
        case MaterialGeneratorNodeType_Subtract: node = NEW MaterialGeneratorNodeSubtract(); break;
        // Func
        case MaterialGeneratorNodeType_Ceil: node = NEW MaterialGeneratorNodeCeil(); break;
        case MaterialGeneratorNodeType_Clamp: node = NEW MaterialGeneratorNodeClamp(); break;
        case MaterialGeneratorNodeType_Cross: node = NEW MaterialGeneratorNodeCross(); break;
        case MaterialGeneratorNodeType_Distance: node = NEW MaterialGeneratorNodeDistance(); break;
        case MaterialGeneratorNodeType_DistanceSqr: node = NEW MaterialGeneratorNodeDistanceSqr(); break;
        case MaterialGeneratorNodeType_Dot: node = NEW MaterialGeneratorNodeDot(); break;
        case MaterialGeneratorNodeType_Floor: node = NEW MaterialGeneratorNodeFloor(); break;
        case MaterialGeneratorNodeType_Frac: node = NEW MaterialGeneratorNodeFrac(); break;
        case MaterialGeneratorNodeType_Lerp: node = NEW MaterialGeneratorNodeLerp(); break;
        case MaterialGeneratorNodeType_Min: node = NEW MaterialGeneratorNodeMin(); break;
        case MaterialGeneratorNodeType_Max: node = NEW MaterialGeneratorNodeMax(); break;
        case MaterialGeneratorNodeType_OneMin: node = NEW MaterialGeneratorNodeOneMin(); break;
        case MaterialGeneratorNodeType_Normalize: node = NEW MaterialGeneratorNodeNormalize(); break;
        case MaterialGeneratorNodeType_Power: node = NEW MaterialGeneratorNodePower(); break;
        case MaterialGeneratorNodeType_Reflect: node = NEW MaterialGeneratorNodeReflect(); break;
        case MaterialGeneratorNodeType_Sign: node = NEW MaterialGeneratorNodeSign(); break;

        // Texture
        //case MaterialGeneratorNodeType_FlipBook: node = NEW MaterialGeneratorNodeFlipBook(); break;
        //case MaterialGeneratorNodeType_Panner: node = NEW MaterialGeneratorNodePanner(); break;
        //case MaterialGeneratorNodeType_Rotator: node = NEW MaterialGeneratorNodeRotator(); break;
        //case MaterialGeneratorNodeType_Texture2D: node = NEW MaterialGeneratorNodeTexture2D(); break;
        //case MaterialGeneratorNodeType_TextureCube: node = NEW MaterialGeneratorNodeTextureCube(); break;
        //case MaterialGeneratorNodeType_Texcoord: node = NEW MaterialGeneratorNodeTexcoord(); break;

        default:
            LOG(LogType_ProgrammerAssert, "Unknown node in factory: %s", materialGeneratorNodeTypeToString(type));
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

} } //end namespace
