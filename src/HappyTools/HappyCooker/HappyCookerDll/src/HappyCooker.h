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

#include "PhysicsEngine.h"

#include "BinaryFileVisitor.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace happycooker {

struct BoneWeight
{
    size_t boneID;
    float weight;
};
struct Bone
{
    he::mat44 transformation;
    he::String name;
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

    bool cook(const he::String& inputFilename, const he::String& outputName);

    void setImportPath(const he::String& path);
    void setModelExportPath(const he::String& path);
    void setPhysicsExportPath(const he::String& path);

    void setInfoCallback(bool (__stdcall *infoCallback)(const char*));

private:

    bool cookBinObj(he::io::BinaryFileVisitor& stream);
    bool cookConvex(he::io::BinaryFileVisitor& stream);
    bool cookTriangleMesh(he::io::BinaryFileVisitor& stream);

    struct CookData
    {
        aiMesh* pMesh;
        he::mat44 mtxTransformation;
    };
    he::ObjectList<CookData> m_ModelCookData;
    he::ObjectList<CookData> m_ConvexCookData;
    he::ObjectList<CookData> m_TriangleMeshCookData;

    he::String m_ImportPath;
    he::String m_ModelExportPath;
    he::String m_PhysicsExportPath;

    static HappyCooker* s_Singleton;
    HappyCooker();

    he::px::PhysicsEngine* m_PhysicsEngine;
    void addInfo(he::String info);
    bool (__stdcall *m_InfoCallback)(const char*);

    void binobjNodeRunner(aiNode* pNode, const aiScene* pScene, const he::mat44& p_Transformation);
    
    //Disable default copy constructor and default assignment operator
    HappyCooker(const HappyCooker&);
    HappyCooker& operator=(const HappyCooker&);
};

} //end namespace

#endif
