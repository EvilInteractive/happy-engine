//HappyCooker Copyright (C) 2011  Bastian Damman
//
//This file is part of HappyCooker.
//
//    HappyCooker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyCooker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyCooker.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 21/08/2011

#include "HappyCooker.h"
#include "cooking/PxCooking.h"
#include "ObjLoader.h"
#include <vector>
#include "VertexLayout.h"
#include "FileNotFoundException.h"
#include "ObjLineLoader.h"
#include "MathFunctions.h"
#include "Bone.h"

#pragma warning(disable:4100) //unreferenced formal parameter
#include "assimp.hpp"
#pragma warning(default:4100)
#include "aiScene.h"
#include "aiPostProcess.h"

namespace happycooker {

HappyCooker* HappyCooker::s_pSingleton = nullptr;

HappyCooker* HappyCooker::getInstance()
{
    if (s_pSingleton == nullptr)
        s_pSingleton = new HappyCooker();
    return s_pSingleton;
}
void HappyCooker::dispose()
{
    delete s_pSingleton;
    s_pSingleton = nullptr;
}

HappyCooker::HappyCooker(): m_pPhysicsEngine(new he::px::PhysicsEngine())
{
    m_InfoCallback = nullptr;
}


HappyCooker::~HappyCooker()
{
    delete m_pPhysicsEngine;
}

bool HappyCooker::cookToConvex(const char* input, const char* output)
{
    using namespace he;

    std::cout << "\nhappycooker cooking: " << input << " to " << output << "\n";

    ct::models::ObjLoader objLoader;

    gfx::VertexLayout layout;
    layout.addElement(gfx::VertexElement(0, gfx::VertexElement::Type_Vec3, gfx::VertexElement::Usage_Position,
        sizeof(vec3), 0));

    try { objLoader.load(input, layout, false); }
    catch (err::FileNotFoundException e)
    {
        std::wcout << "err while trying to read obj: " << e.getMsg();
        return false;
    }
    std::cout << "starting cooking... \n";
    PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), PxCookingParams()));

    if (cooking == nullptr)
    {
        std::cout << "cooking failed to start\n";
        return false;
    }
    io::BinaryStream stream(output, io::BinaryStream::Write);

    byte numMeshes(static_cast<byte>(min<uint>(objLoader.getNumMeshes(), 255)));
    stream.storeByte(numMeshes);
    bool succes(true);
    for (uint i = 0; i < numMeshes; ++i)
    {
        std::cout << "   cooking " << objLoader.getMeshName(i) << " - " << objLoader.getNumVertices(i) << " vertices and " << objLoader.getNumIndices(i) << " indices";    

        PxConvexMeshDesc desc;   
        switch (objLoader.getIndexStride(i))
        {
        case gfx::IndexStride_Byte: ASSERT("byte indices are not supported"); break;
        case gfx::IndexStride_UShort: desc.flags = PxConvexFlag::e16_BIT_INDICES; break;
        case gfx::IndexStride_UInt: break;
        default: ASSERT("unkown indexType"); break;
        }

        desc.points.count = objLoader.getNumVertices(i);
        desc.points.data = objLoader.getVertices(i);
        desc.points.stride = sizeof(VertexPos);
        desc.triangles.count = objLoader.getNumIndices(i) / 3;
        desc.triangles.data = objLoader.getIndices(i);
        desc.triangles.stride = objLoader.getIndexStride(i) * 3; //stride of triangle = 3 indices

        if (cooking->cookConvexMesh(desc, stream))
        {
            std::cout << "     DONE!\n";
        }
        else
        {
            std::cout << "     FAILED!\n";
            succes = false;
        }        
    }
    cooking->release();

    std::cout << "cooking done! \n";
    if (succes)
        std::cout << "cooking successful! :)\n";
    else
        std::cout << "cooking failed! :(\n";

    return succes;
}

bool HappyCooker::binobjNodeRunner(he::io::BinaryStream& stream, aiNode* pNode, const aiScene* pScene, const he::mat44& p_Transformation)
{
    using namespace he;
    aiMatrix4x4 mtxLocalTransform = pNode->mTransformation;
    mat44 mtxTransformation = mat44(mtxLocalTransform.a1, mtxLocalTransform.a2, mtxLocalTransform.a3, mtxLocalTransform.a4,
                                    mtxLocalTransform.b1, mtxLocalTransform.b2, mtxLocalTransform.b3, mtxLocalTransform.b4,
                                    mtxLocalTransform.c1, mtxLocalTransform.c2, mtxLocalTransform.c3, mtxLocalTransform.c4,
                                    mtxLocalTransform.d1, mtxLocalTransform.d2, mtxLocalTransform.d3, mtxLocalTransform.d4) * p_Transformation;

    ASSERT(pNode->mNumMeshes <= 1, "more than one mesh in a node... didn't know this could happen, what is this?");
    for (uint iMesh = 0; iMesh < pNode->mNumMeshes; ++iMesh)
    {
        aiMesh* pMesh(pScene->mMeshes[pNode->mMeshes[iMesh]]);
        
        //////////////////////////////////////////////////////////////////////////
        ///                              Mesh                                  ///
        //////////////////////////////////////////////////////////////////////////
        //store name
        stream.storeString(pNode->mName.data);


        //////////////////////////////////////////////////////////////////////////
        ///                              Bones                                 ///
        //////////////////////////////////////////////////////////////////////////
        //checks
        if (pMesh->mNumBones > he::gfx::Bone::MAX_BONES)
        {
            std::stringstream infoStream;
            infoStream << "Failed cooking: more than " << he::gfx::Bone::MAX_BONES << " found (" << pMesh->mNumBones << ") in mesh " << pNode->mName.data;
            addInfo(infoStream.str());
            return false;
        }

        struct BoneWeight
        {
            uint boneID;
            float weight;
        };
        struct Bone
        {
            mat44 transformation;
            std::string name;
        };
        std::vector<Bone> boneList;
        std::vector<std::vector<BoneWeight>> bonePerVertexID;
        bonePerVertexID.resize(pMesh->mNumVertices);
        for (uint iBone = 0; iBone < pMesh->mNumBones; ++iBone)
        {
            Bone bone;
            bone.name = pMesh->mBones[iBone]->mName.data;
            const aiMatrix4x4& mtxLocalTransform(pMesh->mBones[iBone]->mOffsetMatrix);
            bone.transformation = mat44(mtxLocalTransform.a1, mtxLocalTransform.a2, mtxLocalTransform.a3, mtxLocalTransform.a4,
                                        mtxLocalTransform.b1, mtxLocalTransform.b2, mtxLocalTransform.b3, mtxLocalTransform.b4,
                                        mtxLocalTransform.c1, mtxLocalTransform.c2, mtxLocalTransform.c3, mtxLocalTransform.c4,
                                        mtxLocalTransform.d1, mtxLocalTransform.d2, mtxLocalTransform.d3, mtxLocalTransform.d4);
            boneList.push_back(bone);
            for (uint iWeight = 0; iWeight < pMesh->mBones[iBone]->mNumWeights; ++iWeight)
            {
                BoneWeight weight;
                weight.boneID = iBone;
                weight.weight = pMesh->mBones[iBone]->mWeights[iWeight].mWeight;
                bonePerVertexID[pMesh->mBones[iBone]->mWeights[iWeight].mVertexId].push_back(weight);
            }
        }

        std::stringstream infoStream;
        infoStream << "    cooking: " << pNode->mName.data << " - " << pMesh->mNumVertices << " vertices, " << pMesh->mNumFaces * 3 << " indices";
        infoStream << ", " << pMesh->mNumBones << " bones";
        addInfo(infoStream.str());
        std::cout << "mesh: " << pNode->mMeshes[iMesh] << "," << iMesh << "\n";

        //store #bones
        stream.storeByte(static_cast<byte>(boneList.size()));
        std::for_each(boneList.cbegin(), boneList.cend(), [&](const Bone& bone)
        {
            //store bone name
            stream.storeString(bone.name);
            //store bone transform
            using namespace he;

            mat44 transformation(bone.transformation);
            transformation(0, 3) *= 0.01f;
            transformation(1, 3) *= 0.01f;
            transformation(2, 3) *= 0.01f;
            transformation(3, 3) = 1.0f;
            stream.storeMatrix(transformation * mtxTransformation.inverse());
        });

        //////////////////////////////////////////////////////////////////////////
        ///                              Vertices                              ///
        //////////////////////////////////////////////////////////////////////////
        //store numVert
        stream.storeDword(pMesh->mNumVertices);
        for (uint iVert = 0; iVert < pMesh->mNumVertices; ++iVert)
        {
            //store position
            stream.storeVector3((mtxTransformation * mat44::createScale(0.01f) * vec4(pMesh->mVertices[iVert].x, pMesh->mVertices[iVert].y, pMesh->mVertices[iVert].z, 1.0f)).xyz());
            //store texture coordinate  
            stream.storeVector2(vec2(pMesh->mTextureCoords[0][iVert].x, pMesh->mTextureCoords[0][iVert].y));
            //store normal
            stream.storeVector3((mtxTransformation * vec4(pMesh->mNormals[iVert].x, pMesh->mNormals[iVert].y, pMesh->mNormals[iVert].z, 0.0f)).xyz());
            //store tangent
            stream.storeVector3((mtxTransformation * vec4(pMesh->mTangents[iVert].x, pMesh->mTangents[iVert].y, pMesh->mTangents[iVert].z, 0.0f)).xyz());
            //store bone thingies
            for (uint i = 0; i < he::gfx::Bone::MAX_BONEWEIGHTS; ++i)
            {
                if (bonePerVertexID[iVert].size() > i)
                    stream.storeByte(static_cast<byte>(bonePerVertexID[iVert][i].boneID));
                else
                    stream.storeByte(0);
            }
            for (uint i = 0; i < he::gfx::Bone::MAX_BONEWEIGHTS; ++i)
            {
                if (bonePerVertexID[iVert].size() > i)
                    stream.storeFloat(bonePerVertexID[iVert][i].weight);
                else
                    stream.storeFloat(0.0f);
            }
        }
        std::cout << "Indices:" << "\n";
        //store numIndices
        uint numInd(pMesh->mNumFaces * 3);
        stream.storeDword(numInd);
        //store indexType
        gfx::IndexStride stride(gfx::IndexStride_UInt);
        if (numInd < UCHAR_MAX)
            stride = gfx::IndexStride_Byte;
        else if (numInd < USHRT_MAX)
            stride = gfx::IndexStride_UShort;
        stream.storeByte(static_cast<byte>(stride));
        for (uint iFace = 0; iFace < pMesh->mNumFaces; ++iFace)
        {
            ASSERT(pMesh->mFaces[iFace].mNumIndices == 3, "mesh is not triangulated");
            //store indices
            for (uint iInd = 0; iInd < pMesh->mFaces[iFace].mNumIndices; ++iInd)
            {
                switch (stride)
                {
                case gfx::IndexStride_Byte:   stream.storeByte(static_cast<byte>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                case gfx::IndexStride_UShort: stream.storeWord(static_cast<ushort>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                case gfx::IndexStride_UInt:   stream.storeDword(static_cast<uint>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                default: ASSERT("Unkown stride type"); break;
                }
            }
        }
    }

    for (uint iChild = 0; iChild < pNode->mNumChildren; ++iChild)
    {
        if (binobjNodeRunner(stream, pNode->mChildren[iChild], pScene, mtxTransformation) == false)
            return false;
    }
    return true;
}
bool HappyCooker::cookToBinObj(const char* input, const char* output)
{
    using namespace he;

    std::stringstream infoStream;
    infoStream << "Cooking: " << input << " to " << output;
    addInfo(infoStream.str());
    addInfo("Importing...");

    Assimp::Importer importer;

    importer.SetPropertyInteger("AI_CONFIG_PP_RVC_FLAGS", aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_MATERIALS);
    importer.SetPropertyInteger("AI_CONFIG_PP_LBW_MAX_WEIGHTS", he::gfx::Bone::MAX_BONEWEIGHTS);
    const aiScene* pScene( importer.ReadFile(input,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_RemoveComponent | 
        aiProcess_LimitBoneWeights | 
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_FindInvalidData// |
        //aiProcess_ConvertToLeftHanded
        ));


    if (pScene == nullptr)
    {
        std::stringstream infoStream;
        infoStream << "Import Error: " << importer.GetErrorString();
        addInfo(infoStream.str());
        return false;
    }

    infoStream.str("");
    infoStream.clear();
    infoStream << "Import Info: " << importer.GetErrorString();
    addInfo(infoStream.str());

    //////////////////////////////////////////////////////////////////////////
    ///                             Start Cooking                          ///
    //////////////////////////////////////////////////////////////////////////
    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(pScene->mNumMeshes);

    infoStream.str("");
    infoStream.clear();
    infoStream << "Cooking " << pScene->mNumMeshes << "meshes...";
    addInfo(infoStream.str());

    return binobjNodeRunner(stream, pScene->mRootNode, pScene, mat44::Identity);
}

bool HappyCooker::cookLineToBinObj(const char* input, const char* output)
{
    using namespace he;

    std::cout << "happycooker cooking: " << input << " to " << output << "\n";

    ct::lines::ObjLineLoader objLoader;

    try { objLoader.load(input); }
    catch (err::FileNotFoundException e)
    {
        std::wcout << "err while trying to read obj: " << e.getMsg();
        return false;
    }

    std::cout << "read " << objLoader.getPoints().size() << " vertices and " << objLoader.getIndices().size() << " indices\n";

    std::cout << "starting cooking... \n";

    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(objLoader.getPoints().size());
    stream.storeBuffer(&objLoader.getPoints()[0], sizeof(vec3) * objLoader.getPoints().size());
    stream.storeDword(objLoader.getIndices().size());
    stream.storeBuffer(&objLoader.getIndices()[0], sizeof(ushort) * objLoader.getIndices().size());
    std::cout << "cooking successful! :)\n";
    return true;
}

void HappyCooker::addInfo( std::string info )
{
    std::cout << info << "\n";
    if (m_InfoCallback != nullptr)
        m_InfoCallback(info.c_str());
}

void HappyCooker::setInfoCallback( bool (__stdcall *infoCallback)(const char*) )
{
    m_InfoCallback = infoCallback;
}

} //end namespace