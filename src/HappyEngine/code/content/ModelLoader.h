//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Author:	Bastian Damman
//Created:	23/08/2011
//Extended:	Sebastiaan Sprengers
//Removed concurrency queue because not cross platform: Bastian Damman - 29/10/2011

#ifndef _HE_MODEL_LOADER_H_
#define _HE_MODEL_LOADER_H_
#pragma once

#include "Model.h"
#include "BufferLayout.h"
#include "IModelLoader.h"
#include "AssetContainer.h"

#include <string>
#include <queue>

#include "boost/thread.hpp"

namespace he {
namespace ct {

class ModelLoader
{
public:
    ModelLoader();
    virtual ~ModelLoader();
    
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    gfx::Model::pointer asyncLoadModel(const std::string& path, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh::pointer asyncLoadModelMesh(const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout);

private:
    struct ModelLoadData
    {
    public:
        std::string path;
        gfx::BufferLayout vertexLayout;
        gfx::Model::pointer pModel;
        models::IModelLoader* loader;

        ModelLoadData() {}
        virtual ~ModelLoadData() {}
    private:
        //Disable default copy constructor and default assignment operator
        ModelLoadData(const ModelLoadData&);
        ModelLoadData& operator=(const ModelLoadData&);
    };

    std::queue<ModelLoadData*> m_ModelLoadQueue;
    boost::mutex m_ModelLoadQueueMutex;
    std::queue<ModelLoadData*> m_ModelInvokeQueue;
    boost::mutex m_ModelInvokeQueueMutex;

    boost::mutex m_WaitListMutex;

    boost::thread m_ModelLoadThread;

    void ModelLoadThread();
    bool m_isModelThreadRunning;

    AssetContainer<gfx::Model::pointer>* m_pAssetContainer;

    //Disable default copy constructor and default assignment operator
    ModelLoader(const ModelLoader&);
    ModelLoader& operator=(const ModelLoader&);
};

} } //end namespace

#endif
