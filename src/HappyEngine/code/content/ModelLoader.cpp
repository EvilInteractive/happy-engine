//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 23/08/2011
//Extended:	Sebastiaan Sprengers
//Removed concurrency queue because not cross platform: Bastian Damman - 29/10/2011

#include "HappyPCH.h" 

#include "ModelLoader.h"
#include "BinObjLoader.h"
#include "ObjLoader.h"
#include "ModelMesh.h"

namespace he {
namespace ct {

ModelLoader::ModelLoader()
    : m_EmptyMesh(nullptr)
    , m_ModelInvokeQueue(10, 10, "ModelInvokeQueue")
    , m_ModelLoadQueue(10, 10, "ModelInvokeQueue")
{
    ObjectHandle handle(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
    m_EmptyMesh = ResourceFactory<gfx::ModelMesh>::getInstance()->get(handle);
}

ModelLoader::~ModelLoader()
{
    m_EmptyMesh->release();
    ResourceFactory<gfx::Model>::getInstance()->garbageCollect();
    ResourceFactory<gfx::ModelMesh>::getInstance()->garbageCollect();
}

void ModelLoader::tick(float /*dTime*/)
{
}

bool ModelLoader::loadTick()
{
    if (m_ModelLoadQueue.empty() == false)
    {
        ModelLoadData data;
        if (m_ModelLoadQueue.pop(data) && loadModel(data))
        {
            m_ModelInvokeQueue.push(data);
        }
        return true;
    }
    return false;
}

void ModelLoader::glThreadInvoke()  //needed for all of the gl operations
{
    while (m_ModelInvokeQueue.empty() == false)
    {
        ModelLoadData data;
        if (m_ModelInvokeQueue.pop(data))
        {
            createModel(data);
        }
    }
}

bool ModelLoader::loadModel( ModelLoadData& data )
{
    if (data.loader->load(data.path)) 
    { 
        HE_INFO("Model load completed: %s", data.path.c_str());
        return true;
    }
    else
    {
        HE_ERROR("Error loading model: %s", data.path.c_str());
        ResourceFactory<gfx::Model>::getInstance()->get(data.modelHandle)->release();
        delete data.loader;
        data.loader = nullptr;
        return false;
    } 
}
bool ModelLoader::createModel( ModelLoadData& data )
{
    m_WaitListMutex.lock(FILE_AND_LINE);

    gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(data.modelHandle));
    uint32 notLoadedMeshes(model->getNumMeshes());
    for (uint32 i = 0; i < data.loader->getNumMeshes(); ++i)
    {
        const he::String& meshName(data.loader->getMeshName(i));
        gfx::ModelMesh* mesh(nullptr);
        if (notLoadedMeshes > 0)
        {
            for (uint32 iNotLoaded = 0; iNotLoaded < notLoadedMeshes; ++iNotLoaded)
            {
                mesh = model->instantiateMesh(meshName);
                if (mesh != nullptr)
                    break;
            }
        }
        if (mesh == nullptr)
        {
            ObjectHandle handle(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
            mesh = ResourceFactory<gfx::ModelMesh>::getInstance()->get(handle);
            mesh->setName(meshName);
            model->addMesh(handle);
        }

        mesh->init(data.loader->getVertexLayout(), gfx::MeshDrawMode_Triangles);
        mesh->setBones(data.loader->getBones(i));
        mesh->setVertices(data.loader->getVertices(i), data.loader->getNumVertices(i), gfx::MeshUsage_Static, true);
        mesh->setIndices(data.loader->getIndices(i), data.loader->getNumIndices(i), data.loader->getIndexStride(i), gfx::MeshUsage_Static);
        if (data.savePickingData == true)
        {
            mesh->createPickingData(data.loader->getVertices(i), data.loader->getNumVertices(i), data.loader->getVertexLayout(), 
                data.loader->getIndices(i), data.loader->getNumIndices(i), data.loader->getIndexStride(i));
        }
        mesh->setLoaded();

        mesh->release();
    }
    HE_INFO("Model create completed: %s", data.path.c_str());
    model->setLoaded();
    m_WaitListMutex.unlock();

    model->release();
    delete data.loader;
    data.loader = nullptr;

    return true;
}

bool ModelLoader::isModelLoaded( const he::String& path, ObjectHandle& outHandle )
{
    bool isLoaded(true);
    isLoaded &= m_AssetContainer.isAssetPresent(path);
    if (isLoaded == true) // in assetContainer
    {
        outHandle = m_AssetContainer.getAsset(path);
        isLoaded &= ResourceFactory<gfx::Model>::getInstance()->isAlive(outHandle); // check if not GC'ed
    }
    return isLoaded;
}

gfx::Model* ModelLoader::asyncLoadModel(const he::String& path, const bool savePickingData)
{
    ObjectHandle handle;
    if (isModelLoaded(path, handle))
    {
        ResourceFactory<gfx::Model>::getInstance()->instantiate(handle);
        return ResourceFactory<gfx::Model>::getInstance()->get(handle);
    }
    else
    {
        ModelLoadData data;
        data.path = path;
        data.modelHandle = ResourceFactory<gfx::Model>::getInstance()->create();
        data.savePickingData = savePickingData;
        
        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(data.modelHandle));
        model->setName(path);

        startAsyncLoadModel(data);

        return model;
    }
}
gfx::ModelMesh* ModelLoader::asyncLoadModelMesh( const he::String& path, const he::String& meshName, const bool savePickingData )
{
    ObjectHandle modelHandle;
    if (isModelLoaded(path, modelHandle)) // if model is loading/loaded
    {
        gfx::ModelMesh* mesh(nullptr);
        m_WaitListMutex.lock(FILE_AND_LINE);

        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(modelHandle));
        if (model->isLoaded()) // if loading done
        { 
            //try get mesh
            mesh = model->instantiateMesh(meshName);
            if (mesh == nullptr)
            {
                //failed: get empty modelmesh
                HE_ERROR("Model Load error: mesh: %s not found in %s", meshName.c_str(), path.c_str());
                mesh = m_EmptyMesh;
                ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(mesh->getHandle());
            }
        }
        else
        {
            if (model->getNumMeshes() > 0) // model is partially loaded
            {
                mesh = model->instantiateMesh(meshName); // try get
            }
            if (mesh == nullptr) // not loaded yet -> add new empty mesh
            {
                ObjectHandle handle(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
                mesh = ResourceFactory<gfx::ModelMesh>::getInstance()->get(handle);
                model->addMesh(handle); // to be loaded
            }
        }

        m_WaitListMutex.unlock();

        HE_ASSERT(mesh != nullptr, "Post load model should not be nullptr");
        return mesh;
    }
    else
    {
        ObjectHandle modelHandle(ResourceFactory<gfx::Model>::getInstance()->create());
        ObjectHandle meshHandle(ResourceFactory<gfx::ModelMesh>::getInstance()->create());

        ModelLoadData data;
        data.path = path;
        data.modelHandle = modelHandle;
        data.savePickingData = savePickingData;

        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(modelHandle));
        model->setName(path);
        gfx::ModelMesh* mesh(ResourceFactory<gfx::ModelMesh>::getInstance()->get(meshHandle));
        mesh->setName(meshName);
        model->addMesh(meshHandle);

        startAsyncLoadModel(data);

        model->release(); // no need to keep the model instance
        return mesh; // instance passed to user
    }

}


bool ModelLoader::getModelLoader( ModelLoadData& data )
{
    if (data.path.rfind(".obj") != he::String::npos)
    {
        data.loader = NEW models::ObjLoader();
    }
    else if (data.path.rfind(".binobj") != he::String::npos)
    {
        data.loader = NEW models::BinObjLoader();
    }
    else
    {
        HE_ERROR("Model loading failed: %s has unknown extension", data.path.c_str());
        return false;
    }
    return true;
}

bool ModelLoader::startAsyncLoadModel(ModelLoadData& data)
{
    bool doLoad(getModelLoader(data));

    // failed assets are also saved -> we do not need to check it twice
    m_AssetContainer.addAsset(data.path, data.modelHandle);

    if (doLoad)
    {
        // need reference for async loading
        ResourceFactory<gfx::Model>::getInstance()->instantiate(data.modelHandle);
        m_ModelLoadQueue.push(data);
    }

    return doLoad;
}

bool ModelLoader::startSyncLoadModel( ModelLoadData& data )
{
    bool success(getModelLoader(data));

    // failed assets are also saved -> we do not need to check it twice
    m_AssetContainer.addAsset(data.path, data.modelHandle);

    if (success)
    {
        success &= loadModel(data);
        if (success)
            success &= createModel(data);
    }
    return success;
}


gfx::Model* ModelLoader::loadModel(const he::String& path, const bool savePickingData)
{
    ObjectHandle handle;
    if (isModelLoaded(path, handle))
    {
        ResourceFactory<gfx::Model>::getInstance()->instantiate(handle);
        return ResourceFactory<gfx::Model>::getInstance()->get(handle);
    }
    else
    {
        ModelLoadData data;
        data.path = path;
        data.modelHandle = ResourceFactory<gfx::Model>::getInstance()->create();
        data.savePickingData = savePickingData;
        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(data.modelHandle));
        model->setName(path);

        startSyncLoadModel(data);
        
        return model;
    }
}

gfx::ModelMesh* ModelLoader::loadModelMesh(const he::String& path, const he::String& meshName, const bool savePickingData)
{
    ObjectHandle modelHandle;
    if (isModelLoaded(path, modelHandle))
    {
        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(modelHandle));
        gfx::ModelMesh* mesh(nullptr);
        if (model->isLoaded())
        {
            mesh = model->instantiateMesh(meshName);
            if (mesh == nullptr)
            {
                //failed: get empty modelmesh
                HE_ERROR("Model Load error: mesh: %s not found in %s", meshName.c_str(), path.c_str());
                mesh = m_EmptyMesh;
                ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(mesh->getHandle());
            }
        }
        else
        {
            // this will be a problem because the model is loaded async as well
            HE_WARNING("Loading mesh: %s - %s problem. Trying to load sync but mesh is already loading async -> loading async", path.c_str(), meshName.c_str());
            mesh = asyncLoadModelMesh(path, meshName, savePickingData);
        }

        return mesh;
    }
    else
    {
        ModelLoadData data;
        data.path = path;
        data.modelHandle = ResourceFactory<gfx::Model>::getInstance()->create();
        data.savePickingData = savePickingData;

        ObjectHandle meshHandle(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
        gfx::Model* model(ResourceFactory<gfx::Model>::getInstance()->get(data.modelHandle));
        model->setName(path);
        gfx::ModelMesh* mesh(ResourceFactory<gfx::ModelMesh>::getInstance()->get(meshHandle));
        mesh->setName(meshName);
        model->addMesh(meshHandle);

        startSyncLoadModel(data);

        model->release();
        return mesh;
    }
}

/* GETTERS */
bool ModelLoader::isLoading() const
{
    return m_ModelLoadQueue.empty() == false || m_ModelInvokeQueue.empty() == false;
}


} } //end namespace
