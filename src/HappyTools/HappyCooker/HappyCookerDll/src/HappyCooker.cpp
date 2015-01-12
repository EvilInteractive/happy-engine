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

#include "HappyCookerPCH.h"

#include "HappyCooker.h"
#include "PhysicsEngine.h"
#include "ObjLoader.h"
#include "BufferLayout.h"
#include "Bone.h"
#include "Map.h"
#include "PhysicsBinaryStream.h"
#include "MeshEnums.h"

#include <cooking/PxCooking.h>
#include <PxPhysics.h>

#pragma warning(disable:4100) //unreferenced formal parameter
#include "assimp.hpp"
#pragma warning(default:4100)
#include "aiScene.h"
#include "aiPostProcess.h"

namespace happycooker {

HappyCooker* HappyCooker::s_Singleton = nullptr;

HappyCooker* HappyCooker::getInstance()
{
    if (s_Singleton == nullptr)
        s_Singleton = NEW HappyCooker();
    return s_Singleton;
}
void HappyCooker::dispose()
{
    delete s_Singleton;
    s_Singleton = nullptr;
}

HappyCooker::HappyCooker(): m_PhysicsEngine(NEW he::px::PhysicsEngine())
{
    m_InfoCallback = nullptr;
}


HappyCooker::~HappyCooker()
{
    delete m_PhysicsEngine;
}


void HappyCooker::binobjNodeRunner(aiNode* pNode, const aiScene* pScene, const he::mat44& p_Transformation)
{
    using namespace he;
    aiMatrix4x4 mtxLocalTransform = pNode->mTransformation;
    mat44 mtxTransformation = p_Transformation * mat44(mtxLocalTransform.a1, mtxLocalTransform.a2, mtxLocalTransform.a3, mtxLocalTransform.a4,
                                    mtxLocalTransform.b1, mtxLocalTransform.b2, mtxLocalTransform.b3, mtxLocalTransform.b4,
                                    mtxLocalTransform.c1, mtxLocalTransform.c2, mtxLocalTransform.c3, mtxLocalTransform.c4,
                                    mtxLocalTransform.d1, mtxLocalTransform.d2, mtxLocalTransform.d3, mtxLocalTransform.d4);

    HE_ASSERT(pNode->mNumMeshes <= 1, "more than one mesh in a node... didn't know this could happen, what is this?");
    for (size_t iMesh = 0; iMesh < pNode->mNumMeshes; ++iMesh)
    {
        aiMesh* pMesh(pScene->mMeshes[pNode->mMeshes[iMesh]]);
        
        pMesh->mName = pNode->mName;

        CookData data;
        data.pMesh = pMesh;
        data.mtxTransformation = mtxTransformation;
        if (std::string(pMesh->mName.data).find("pxcv_") == 0)
        {
            m_ConvexCookData.add(data);
        }
        else if (std::string(pMesh->mName.data).find("pxcc_") == 0)
        {
            m_TriangleMeshCookData.add(data);
        }
        else
        {
            m_ModelCookData.add(data);
        } 
    }

    for (size_t iChild = 0; iChild < pNode->mNumChildren; ++iChild)
    {
        binobjNodeRunner(pNode->mChildren[iChild], pScene, mtxTransformation);
    }
}
bool HappyCooker::cook(const he::String& input, const he::String& output)
{
    m_ModelCookData.clear();
    m_ConvexCookData.clear();
    m_TriangleMeshCookData.clear();

    using namespace he;

    he::StringStream infoStream;
    infoStream << "Cooking: " << input << " to " << output;
    addInfo(infoStream.str());
    addInfo("Importing...");

    Assimp::Importer importer;

    importer.SetPropertyInteger("AI_CONFIG_PP_RVC_FLAGS", aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_MATERIALS);
    importer.SetPropertyInteger("AI_CONFIG_PP_LBW_MAX_WEIGHTS", he::gfx::Bone::MAX_BONEWEIGHTS);
    const aiScene* pScene( importer.ReadFile((m_ImportPath + input).c_str(),
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
        he::StringStream infoStream;
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
        io::BinaryFileVisitor stream;
        he::Path modelPath(m_ModelExportPath + output + ".binobj");
        if (stream.openWrite(modelPath) == false)
        {
            HE_ERROR("Opening bin stream failed: %s", modelPath.str().c_str());
        }
        else
        {
            cookBinObj(stream);
        }
        stream.close();
    }
    if (m_ConvexCookData.size() > 0)
    {
        io::BinaryFileVisitor convexStream;
        he::Path convexPath(m_PhysicsExportPath + output + ".pxcv");
        if (convexStream.openWrite(convexPath) == false)
        {
            HE_ERROR("Opening bin stream failed: %s", convexPath.str().c_str());
        }
        else
        {
            cookConvex(convexStream);
        }
        convexStream.close();
    }
    if (m_TriangleMeshCookData.size() > 0)
    {
        io::BinaryFileVisitor concaveStream;
        he::Path concavePath(m_PhysicsExportPath + output + ".pxcc");
        if (concaveStream.openWrite(concavePath) == false)
        {
            HE_ERROR("Opening bin stream failed: %s", concavePath.str().c_str());
        }
        else
        {
            cookTriangleMesh(concaveStream);
        }
        concaveStream.close();
    }

    return true;
}

void HappyCooker::addInfo( he::String info )
{
    std::cout << info << "\n";
    if (m_InfoCallback != nullptr)
        m_InfoCallback(info.c_str());
}

void HappyCooker::setInfoCallback( bool (__stdcall *infoCallback)(const char*) )
{
    m_InfoCallback = infoCallback;
}

bool HappyCooker::cookBinObj( he::io::BinaryFileVisitor& stream )
{
    using namespace he;

    he::StringStream infoStream;
    infoStream << "  BinObj cooking " << m_ModelCookData.size() << " meshes";
    addInfo(infoStream.str());

    uint32 dataSize(m_ModelCookData.size());
    stream.visit(dataSize);
    m_ModelCookData.forEach([&](const CookData& data)
    {
        using namespace he;
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);

        //////////////////////////////////////////////////////////////////////////
        ///                              Mesh                                  ///
        //////////////////////////////////////////////////////////////////////////
        //store name
        he::String name(pMesh->mName.data);
        stream.visit(name);


        //////////////////////////////////////////////////////////////////////////
        ///                              Bones                                 ///
        //////////////////////////////////////////////////////////////////////////
        //checks
        if (pMesh->mNumBones > he::gfx::Bone::MAX_BONES)
        {
            infoStream.clear();
            infoStream.str("");
            infoStream << "Failed cooking bones: more than " << he::gfx::Bone::MAX_BONES << " found (" << pMesh->mNumBones << ") in mesh " << pMesh->mName.data;
            addInfo(infoStream.str());
            pMesh->mNumBones = 0; //cook no bones
        }

        he::ObjectList<Bone> boneList;
        he::ObjectList<he::ObjectList<BoneWeight>> bonePerVertexID;
        bonePerVertexID.resize(pMesh->mNumVertices);
        for (size_t iBone = 0; iBone < pMesh->mNumBones; ++iBone)
        {
            Bone bone;
            bone.name = pMesh->mBones[iBone]->mName.data;
            const aiMatrix4x4& mtxLocalTransform(pMesh->mBones[iBone]->mOffsetMatrix);
            bone.transformation = mat44(mtxLocalTransform.a1, mtxLocalTransform.a2, mtxLocalTransform.a3, mtxLocalTransform.a4,
                mtxLocalTransform.b1, mtxLocalTransform.b2, mtxLocalTransform.b3, mtxLocalTransform.b4,
                mtxLocalTransform.c1, mtxLocalTransform.c2, mtxLocalTransform.c3, mtxLocalTransform.c4,
                mtxLocalTransform.d1, mtxLocalTransform.d2, mtxLocalTransform.d3, mtxLocalTransform.d4);
            boneList.add(bone);
            for (size_t iWeight = 0; iWeight < pMesh->mBones[iBone]->mNumWeights; ++iWeight)
            {
                BoneWeight weight;
                weight.boneID = iBone;
                weight.weight = pMesh->mBones[iBone]->mWeights[iWeight].mWeight;
                bonePerVertexID[pMesh->mBones[iBone]->mWeights[iWeight].mVertexId].add(weight);
            }
        }

        infoStream.clear();
        infoStream.str("");
        infoStream << "    cooking: " << pMesh->mName.data << " - " << pMesh->mNumVertices << " vertices, " << pMesh->mNumFaces * 3 << " indices";
        infoStream << ", " << pMesh->mNumBones << " bones";
        addInfo(infoStream.str());

        //store #bones
        uint8 boneCount(static_cast<uint8>(boneList.size()));
        stream.visit(boneCount);
        io::BinaryFileVisitor& localStream(stream); // HACK: lambda scope loss
        boneList.forEach([&](Bone& bone)
        {
            //store bone name
            localStream.visit(bone.name);
            //store bone transform
            mat44 transformation(bone.transformation);
            transformation(0, 3) *= 0.01f;
            transformation(1, 3) *= 0.01f;
            transformation(2, 3) *= 0.01f;
            transformation(3, 3) = 1.0f;
            mat44 invTrans(transformation * mtxTransformation.inverse());
            localStream.visit(invTrans);
        });

        //////////////////////////////////////////////////////////////////////////
        ///                              Vertices                              ///
        //////////////////////////////////////////////////////////////////////////
        //store numVert
        uint32 vertexCount(pMesh->mNumVertices);
        stream.visit(vertexCount);
        for (uint32 iVert(0); iVert < vertexCount; ++iVert)
        {
            //store position
            he::vec3 pos((mat44::createScale(0.01f) * mtxTransformation *vec4(pMesh->mVertices[iVert].x, pMesh->mVertices[iVert].y, pMesh->mVertices[iVert].z, 1.0f)).xyz());
            stream.visit(pos);
            //store texture coordinate  
            he::vec2 tex(pMesh->mTextureCoords[0][iVert].x, pMesh->mTextureCoords[0][iVert].y);
            stream.visit(tex);
            //store normal
            he::vec3 norm((mtxTransformation * vec4(pMesh->mNormals[iVert].x, pMesh->mNormals[iVert].y, pMesh->mNormals[iVert].z, 0.0f)).xyz());
            stream.visit(norm);
            //store tangent
            he::vec3 tan((mtxTransformation * vec4(pMesh->mTangents[iVert].x, pMesh->mTangents[iVert].y, pMesh->mTangents[iVert].z, 0.0f)).xyz());
            stream.visit(tan);
            //store bone thingies
            for (size_t i = 0; i < he::gfx::Bone::MAX_BONEWEIGHTS; ++i)
            {
                uint8 boneID(bonePerVertexID[iVert].size() > i? static_cast<byte>(bonePerVertexID[iVert][i].boneID) : 0);
                stream.visit(boneID);
            }
            for (size_t i = 0; i < he::gfx::Bone::MAX_BONEWEIGHTS; ++i)
            {
                float boneWeight(bonePerVertexID[iVert].size() > i? bonePerVertexID[iVert][i].weight : 0.0f);
                stream.visit(boneWeight);
            }
        }
        //store numIndices
        uint32 numInd(pMesh->mNumFaces * 3);
        stream.visit(numInd);
        //store indexType
        gfx::IndexStride stride(gfx::IndexStride_UInt);
        if (numInd < USHRT_MAX)
            stride = gfx::IndexStride_UShort;
        stream.visitEnum<gfx::IndexStride, he::uint8>(stride);
        for (size_t iFace = 0; iFace < pMesh->mNumFaces; ++iFace)
        {
            //ASSERT(pMesh->mFaces[iFace].mNumIndices == 3, "mesh is not triangulated");
            //store indices
            for (size_t iInd = 0; iInd < pMesh->mFaces[iFace].mNumIndices; ++iInd)
            {
                switch (stride)
                {
                case gfx::IndexStride_UShort: 
                    {
                        uint16 index(static_cast<uint16>(pMesh->mFaces[iFace].mIndices[iInd]));
                        stream.visit(index);
                    } break;
                case gfx::IndexStride_UInt:   
                    {
                        uint32 index(static_cast<uint32>(pMesh->mFaces[iFace].mIndices[iInd]));
                        stream.visit(index);
                    } break;
                default: LOG(he::LogType_ProgrammerAssert, "Unknown stride type"); break;
                }
            }
        }
    });
    return true;
}

struct Vec3Hasher
{
    size_t operator()(const he::vec3& pos)
    {
        using namespace he;
        return *reinterpret_cast<const size_t*>(&pos.x) + 
            *reinterpret_cast<const size_t*>(&pos.y) * 10000 + 
            *reinterpret_cast<const size_t*>(&pos.z) * 100000000;
    }
};

void optimizeMeshForPhysX(aiMesh* pMesh, const he::mat44& mtxTransformation, he::ObjectList<he::vec3>& vertices, he::ObjectList<he::uint16>& indices)
{
    using namespace he;

    he::Map<vec3, uint16, Vec3Hasher> vertMap;
    for (size_t iFace(0); iFace < pMesh->mNumFaces; ++iFace)
    {
        HE_ASSERT(pMesh->mFaces[iFace].mNumIndices == 3, "mesh not triangulated");
        for (size_t i(0); i < 3; ++i)
        {
            size_t index(pMesh->mFaces[iFace].mIndices[i]);
            vec3 vert(pMesh->mVertices[index].x, pMesh->mVertices[index].y, pMesh->mVertices[index].z);
            he::Map<vec3, uint16, Vec3Hasher>::const_iterator it(vertMap.find(vert));
            if (it == vertMap.cend())
            {
                vertMap[vert] = static_cast<uint16>(vertices.size());
                indices.add(static_cast<uint16>(vertices.size()));
                vertices.add( mat44::createScale(0.01f) * mtxTransformation * vert);
            }
            else
            {
                indices.add(vertMap[vert]);
            }
        }
    }
}

bool HappyCooker::cookConvex( he::io::BinaryFileVisitor& stream )
{
    using namespace he;

    he::StringStream infoStream;
    infoStream << "  PXCV cooking " << m_ConvexCookData.size() << " meshes";
    addInfo(infoStream.str());

    uint8 numMeshes(static_cast<uint8>(min<size_t>(m_ConvexCookData.size(), 255)));
    if (numMeshes < m_ConvexCookData.size())
        addInfo("Warning to many convex meshes cooking 255");

    physx::PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, m_PhysicsEngine->getSDK()->getFoundation(), physx::PxCookingParams()));
    
    px::PhysicsBinaryStream physxStream(&stream);
    stream.visit(numMeshes);
    bool succes(true);
    m_ConvexCookData.forEach([&](const CookData& data)
    {
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);
        
        physx::PxConvexMeshDesc desc;   
        desc.flags = physx::PxConvexFlag::e16_BIT_INDICES;

        //Optimize vertex and indices
        he::ObjectList<vec3> vertices;
        he::ObjectList<uint16> indices;
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
        desc.triangles.stride = sizeof(uint16) * 3; //stride of triangle = 3 indices

        he::String name(data.pMesh->mName.data);
        stream.visit(name);
        if (!cooking->cookConvexMesh(desc, physxStream))
        {
            addInfo("    FAILED"); 
            succes = false;
        }        
    });

    cooking->release();

    return succes;
}
bool HappyCooker::cookTriangleMesh( he::io::BinaryFileVisitor& stream )
{
    using namespace he;

    he::StringStream infoStream;
    infoStream << "  PXCC cooking " << m_TriangleMeshCookData.size() << " meshes";
    addInfo(infoStream.str());

    uint8 numMeshes(static_cast<uint8>(min<size_t>(m_TriangleMeshCookData.size(), 255)));
    if (numMeshes < m_TriangleMeshCookData.size())
        addInfo("Warning to many convex meshes cooking 255");

    physx::PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, m_PhysicsEngine->getSDK()->getFoundation(), physx::PxCookingParams()));

    px::PhysicsBinaryStream physxStream(&stream);

    stream.visit(numMeshes);
    bool succes(true);
    m_TriangleMeshCookData.forEach([&](const CookData& data)
    {
        aiMesh* pMesh(data.pMesh);
        const mat44& mtxTransformation(data.mtxTransformation);

        physx::PxTriangleMeshDesc desc;   
        desc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

        //Optimize vertex and indices
        he::ObjectList<vec3> vertices;
        he::ObjectList<uint16> indices;
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
        desc.triangles.stride = sizeof(uint16) * 3; //stride of triangle = 3 indices

        he::String name(data.pMesh->mName.data);
        stream.visit(name);
        if (!cooking->cookTriangleMesh(desc, physxStream))
        {
            addInfo("    FAILED"); 
            succes = false;
        }        
    });

    cooking->release();

    return succes;
}

void HappyCooker::setModelExportPath( const he::String& path )
{
    m_ModelExportPath = path;
}

void HappyCooker::setPhysicsExportPath( const he::String& path )
{
    m_PhysicsExportPath = path;
}

void HappyCooker::setImportPath( const he::String& path )
{
    m_ImportPath = path;
}

} //end namespace