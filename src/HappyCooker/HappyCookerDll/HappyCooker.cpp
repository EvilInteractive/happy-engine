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


void HappyCooker::binobjNodeRunner(aiNode* pNode, const aiScene* pScene, const he::mat44& p_Transformation)
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
        
        pMesh->mName = pNode->mName;

        CookData data;
        data.pMesh = pMesh;
        data.mtxTransformation = mtxTransformation;
        if (std::string(pMesh->mName.data).find("pxcv_") == 0)
        {
            m_ConvexCookData.push_back(data);
        }
        else if (std::string(pMesh->mName.data).find("pxcc_") == 0)
        {
            m_TriangleMeshCookData.push_back(data);
        }
        else
        {
            m_ModelCookData.push_back(data);
        } 
    }

    for (uint iChild = 0; iChild < pNode->mNumChildren; ++iChild)
    {
        binobjNodeRunner(pNode->mChildren[iChild], pScene, mtxTransformation);
    }
}
bool HappyCooker::cook(const std::string& input, const std::string& output)
{
    m_ModelCookData.clear();
    m_ConvexCookData.clear();
    m_TriangleMeshCookData.clear();

    using namespace he;

    std::stringstream infoStream;
    infoStream << "Cooking: " << input << " to " << output;
    addInfo(infoStream.str());
    addInfo("Importing...");

    Assimp::Importer importer;

    importer.SetPropertyInteger("AI_CONFIG_PP_RVC_FLAGS", aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_MATERIALS);
    importer.SetPropertyInteger("AI_CONFIG_PP_LBW_MAX_WEIGHTS", he::gfx::Bone::MAX_BONEWEIGHTS);
    const aiScene* pScene( importer.ReadFile(m_ImportPath + input,
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
    addInfo("Preparing cooking...");
    binobjNodeRunner(pScene->mRootNode, pScene, mat44::Identity);

    addInfo("Starting cooking...");

    if (m_ModelCookData.size() > 0)
    {
        io::BinaryStream stream(m_ModelExportPath + output + ".binobj", io::BinaryStream::Write);
        cookBinObj(stream);
    }
    if (m_ConvexCookData.size() > 0)
    {
        io::BinaryStream convexStream(m_PhysicsExportPath + output + ".pxcv", io::BinaryStream::Write);
        cookConvex(convexStream);
    }
    if (m_TriangleMeshCookData.size() > 0)
    {
        io::BinaryStream concaveStream(m_PhysicsExportPath + output + ".pxcc", io::BinaryStream::Write);
        cookTriangleMesh(concaveStream);
    }

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

bool HappyCooker::cookBinObj( he::io::BinaryStream& stream )
{
    using namespace he;

    std::stringstream infoStream;
    infoStream << "  BinObj cooking " << m_ModelCookData.size() << " meshes";
    addInfo(infoStream.str());

    stream.storeDword(m_ModelCookData.size());
    std::for_each(m_ModelCookData.cbegin(), m_ModelCookData.cend(), [&](const CookData& data)
    {
        using namespace he;
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);

        //////////////////////////////////////////////////////////////////////////
        ///                              Mesh                                  ///
        //////////////////////////////////////////////////////////////////////////
        //store name
        stream.storeString(pMesh->mName.data);


        //////////////////////////////////////////////////////////////////////////
        ///                              Bones                                 ///
        //////////////////////////////////////////////////////////////////////////
        //checks
        if (pMesh->mNumBones > he::gfx::Bone::MAX_BONES)
        {
            std::stringstream infoStream;
            infoStream << "Failed cooking bones: more than " << he::gfx::Bone::MAX_BONES << " found (" << pMesh->mNumBones << ") in mesh " << pMesh->mName.data;
            addInfo(infoStream.str());
            pMesh->mNumBones = 0; //cook no bones
        }

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
        infoStream << "    cooking: " << pMesh->mName.data << " - " << pMesh->mNumVertices << " vertices, " << pMesh->mNumFaces * 3 << " indices";
        infoStream << ", " << pMesh->mNumBones << " bones";
        addInfo(infoStream.str());

        //store #bones
        stream.storeByte(static_cast<byte>(boneList.size()));
        io::BinaryStream& localStream(stream); // HACK: lambda scope loss
        std::for_each(boneList.cbegin(), boneList.cend(), [&](const Bone& bone)
        {
            //store bone name
            localStream.storeString(bone.name);
            //store bone transform
            using namespace he;

            mat44 transformation(bone.transformation);
            transformation(0, 3) *= 0.01f;
            transformation(1, 3) *= 0.01f;
            transformation(2, 3) *= 0.01f;
            transformation(3, 3) = 1.0f;
            localStream.storeMatrix(transformation * mtxTransformation.inverse());
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
            //ASSERT(pMesh->mFaces[iFace].mNumIndices == 3, "mesh is not triangulated");
            //store indices
            for (uint iInd = 0; iInd < pMesh->mFaces[iFace].mNumIndices; ++iInd)
            {
                switch (stride)
                {
                case gfx::IndexStride_Byte:   stream.storeByte(static_cast<byte>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                case gfx::IndexStride_UShort: stream.storeWord(static_cast<ushort>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                case gfx::IndexStride_UInt:   stream.storeDword(static_cast<uint>(pMesh->mFaces[iFace].mIndices[iInd])); break;
                //default: ASSERT("Unkown stride type"); break;
                }
            }
        }
    });
    return true;
}


void optimizeMeshForPhysX(aiMesh* pMesh, const he::mat44& mtxTransformation, std::vector<he::vec3>& vertices, std::vector<he::ushort>& indices)
{
    using namespace he;

    std::map<vec3, ushort> vertMap;
    for (uint iFace(0); iFace < pMesh->mNumFaces; ++iFace)
    {
        ASSERT(pMesh->mFaces[iFace].mNumIndices == 3, "mesh not triangulated");
        for (uint i(0); i < 3; ++i)
        {
            uint index(pMesh->mFaces[iFace].mIndices[i]);
            vec3 vert(pMesh->mVertices[index].x, pMesh->mVertices[index].y, pMesh->mVertices[index].z);
            std::map<vec3, ushort>::const_iterator it(vertMap.find(vert));
            if (it == vertMap.cend())
            {
                vertMap[vert] = static_cast<ushort>(vertices.size());
                indices.push_back(static_cast<ushort>(vertices.size()));
                vertices.push_back(mtxTransformation * mat44::createScale(0.01f) * vert);
            }
            else
            {
                indices.push_back(vertMap[vert]);
            }
        }
    }
}

bool HappyCooker::cookConvex( he::io::BinaryStream& stream )
{
    using namespace he;

    std::stringstream infoStream;
    infoStream << "  PXCV cooking " << m_ConvexCookData.size() << " meshes";
    addInfo(infoStream.str());

    byte numMeshes(static_cast<byte>(min<uint>(m_ConvexCookData.size(), 255)));
    if (numMeshes < m_ConvexCookData.size())
        addInfo("Warning to many convex meshes cooking 255");

    physx::PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), physx::PxCookingParams()));

    stream.storeByte(numMeshes);
    bool succes(true);
    std::for_each(m_ConvexCookData.cbegin(), m_ConvexCookData.cend(), [&](const CookData& data)
    {
        using namespace he;
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);
        
        physx::PxConvexMeshDesc desc;   
        desc.flags = physx::PxConvexFlag::e16_BIT_INDICES;

        //Optimize vertex and indices
        std::vector<vec3> vertices;
        std::vector<ushort> indices;
        optimizeMeshForPhysX(pMesh, mtxTransformation, vertices, indices);

        infoStream.str("");
        infoStream.clear();
        infoStream << "   cooking " << pMesh->mName.data << " - " << vertices.size() << " vertices and " << indices.size() << " indices";   
        addInfo(infoStream.str()); 

        desc.points.count = vertices.size();
        desc.points.data = &vertices[0];
        desc.points.stride = sizeof(vec3);
        desc.triangles.count = indices.size() / 3;
        desc.triangles.data = &indices[0];
        desc.triangles.stride = sizeof(ushort) * 3; //stride of triangle = 3 indices

        if (cooking->cookConvexMesh(desc, stream))
        {
        }
        else
        {
            addInfo("    FAILED"); 
            succes = false;
        }        
    });

    cooking->release();

    return succes;
}
bool HappyCooker::cookTriangleMesh( he::io::BinaryStream& stream )
{
    using namespace he;

    std::stringstream infoStream;
    infoStream << "  PXCC cooking " << m_ConvexCookData.size() << " meshes";
    addInfo(infoStream.str());

    byte numMeshes(static_cast<byte>(min<uint>(m_ConvexCookData.size(), 255)));
    if (numMeshes < m_ConvexCookData.size())
        addInfo("Warning to many convex meshes cooking 255");

    physx::PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), physx::PxCookingParams()));

    stream.storeByte(numMeshes);
    bool succes(true);
    std::for_each(m_ConvexCookData.cbegin(), m_ConvexCookData.cend(), [&](const CookData& data)
    {
        using namespace he;
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);

        physx::PxTriangleMeshDesc desc;   
        desc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

        //Optimize vertex and indices
        std::vector<vec3> vertices;
        std::vector<ushort> indices;
        optimizeMeshForPhysX(pMesh, mtxTransformation, vertices, indices);

        infoStream.str("");
        infoStream.clear();
        infoStream << "   cooking " << pMesh->mName.data << " - " << vertices.size() << " vertices and " << indices.size() << " indices";   
        addInfo(infoStream.str()); 

        desc.points.count = vertices.size();
        desc.points.data = &vertices[0];
        desc.points.stride = sizeof(vec3);
        desc.triangles.count = indices.size() / 3;
        desc.triangles.data = &indices[0];
        desc.triangles.stride = sizeof(ushort) * 3; //stride of triangle = 3 indices

        if (cooking->cookTriangleMesh(desc, stream))
        {
        }
        else
        {
            addInfo("    FAILED"); 
            succes = false;
        }        
    });

    cooking->release();

    return succes;
}

void HappyCooker::setModelExportPath( const std::string& path )
{
    m_ModelExportPath = path;
}

void HappyCooker::setPhysicsExportPath( const std::string& path )
{
    m_PhysicsExportPath = path;
}

void HappyCooker::setImportPath( const std::string& path )
{
    m_ImportPath = path;
}

} //end namespace