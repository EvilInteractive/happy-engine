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

#ifndef _HE_MODEL_LOADER_H_
#define _HE_MODEL_LOADER_H_
#pragma once

#include "Model.h"
#include "VertexLayout.h"
#include "IModelLoader.h"
#include "AssetContainer.h"

#include <ppl.h>
#include <concurrent_queue.h>
#include <string>

#include "boost/thread.hpp"

namespace happyengine {
namespace content {

class ModelLoader
{
public:
	ModelLoader();
    virtual ~ModelLoader();
    
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

	graphics::Model::pointer asyncLoadModel(const std::string& path, const graphics::VertexLayout& vertexLayout);

private:
    struct ModelLoadData
    {
    public:
        std::string path;
        graphics::VertexLayout vertexLayout;
        graphics::Model::pointer pModel;
        models::IModelLoader* loader;

        ModelLoadData() {}
        virtual ~ModelLoadData() {}
    private:
        //Disable default copy constructor and default assignment operator
        ModelLoadData(const ModelLoadData&);
        ModelLoadData& operator=(const ModelLoadData&);
    };

    Concurrency::concurrent_queue<ModelLoadData*> m_ModelLoadQueue;
    Concurrency::concurrent_queue<ModelLoadData*> m_ModelInvokeQueue;
    boost::thread m_ModelLoadThread;
    void ModelLoadThread();
    bool m_isModelThreadRunning;

	AssetContainer<graphics::Model::pointer>* m_pAssetContainer;

    //Disable default copy constructor and default assignment operator
    ModelLoader(const ModelLoader&);
    ModelLoader& operator=(const ModelLoader&);
};

} } //end namespace

#endif
