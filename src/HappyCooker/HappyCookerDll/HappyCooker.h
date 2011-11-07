//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyCooker.
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
//Created: 21/08/2011

#ifndef _HAPPY_COOKER_H_
#define _HAPPY_COOKER_H_
#pragma once

#include "Assert.h"
#undef assert
#define assert ASSERT
#include "PhysicsEngine.h"
#include "vec3.h"

#include "BinaryStream.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace happycooker {

struct BoneWeight
{
    he::uint boneID;
    float weight;
};
struct Bone
{
    he::mat44 transformation;
    std::string name;
};
struct VertexPos
{
    he::vec3 position;
};

class HappyCooker
{
public:
    virtual ~HappyCooker();
    
    static HappyCooker* getInstance();
    static void dispose();

    bool cook(const std::string& inputFilename, const std::string& outputName);

    void setImportPath(const std::string& path);
    void setModelExportPath(const std::string& path);
    void setPhysicsExportPath(const std::string& path);

    void setInfoCallback(bool (__stdcall *infoCallback)(const char*));

private:

    bool cookBinObj(he::io::BinaryStream& stream);
    bool cookConvex(he::io::BinaryStream& stream);
    bool cookTriangleMesh(he::io::BinaryStream& stream);

    struct CookData
    {
        aiMesh* pMesh;
        he::mat44 mtxTransformation;
    };
    std::vector<CookData> m_ModelCookData;
    std::vector<CookData> m_ConvexCookData;
    std::vector<CookData> m_TriangleMeshCookData;

    std::string m_ImportPath;
    std::string m_ModelExportPath;
    std::string m_PhysicsExportPath;

    static HappyCooker* s_pSingleton;
	HappyCooker();

    he::px::PhysicsEngine* m_pPhysicsEngine;
    void addInfo(std::string info);
    bool (__stdcall *m_InfoCallback)(const char*);

    void binobjNodeRunner(aiNode* pNode, const aiScene* pScene, const he::mat44& p_Transformation);
    
    //Disable default copy constructor and default assignment operator
    HappyCooker(const HappyCooker&);
    HappyCooker& operator=(const HappyCooker&);
};

} //end namespace

#endif
