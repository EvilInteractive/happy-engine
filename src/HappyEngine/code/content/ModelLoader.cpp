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
#include "HappyNew.h"
#include "BinObjLoader.h"
#include "ObjLoader.h"
#include "FileNotFoundException.h"
#include "HappyEngine.h"
#include "Console.h"

#include <vector>

namespace he {
namespace ct {

ModelLoader::ModelLoader(): m_isModelThreadRunning(false), m_pAssetContainer(nullptr)                
{
    m_pAssetContainer = NEW AssetContainer<gfx::Model::pointer>([](const gfx::Model::pointer& model)
    {
        std::for_each(model->cbegin(), model->cend(), [](const gfx::ModelMesh::pointer& mesh)
        {
            if (mesh.use_count() > 1)
            {
                HE_ERROR("Possible mesh leak: " + mesh->getName());
            }
        });
    });
}

ModelLoader::~ModelLoader()
{
    delete m_pAssetContainer;
}

void ModelLoader::tick(float /*dTime*/)
{
    if (m_isModelThreadRunning == false)
        if (m_ModelLoadQueue.empty() == false)
        {
            m_isModelThreadRunning = true; //must be here else it could happen that the load thread starts twice
            m_ModelLoadThread = boost::thread(boost::bind(&ModelLoader::ModelLoadThread, this));
        }
}
void ModelLoader::glThreadInvoke()  //needed for all of the gl operations
{
    while (m_ModelInvokeQueue.empty() == false)
    {
        m_ModelInvokeQueueMutex.lock();
        ModelLoadData* data(m_ModelInvokeQueue.front());
        m_ModelInvokeQueue.pop();
        m_ModelInvokeQueueMutex.unlock();

        m_WaitListMutex.lock();

        uint unloadedMeshes(data->pModel->getNumMeshes());
        for (uint i = 0; i < data->loader->getNumMeshes(); ++i)
        {
            gfx::ModelMesh::pointer pMesh;
            if (unloadedMeshes > 0)
            {
                for (uint iUnloaded = 0; iUnloaded < unloadedMeshes; ++iUnloaded) // TODO: optimize
                {
                    if (data->loader->getMeshName(i) == data->pModel->getMesh(iUnloaded)->getName())
                    {
                        pMesh = data->pModel->getMesh(iUnloaded);
                        break;
                    }
                }
            }
            if (pMesh == nullptr)
            {
                pMesh = gfx::ModelMesh::pointer(NEW gfx::ModelMesh(data->loader->getMeshName(i)));
                data->pModel->addMesh(pMesh);
            }

            pMesh->init();
            pMesh->setBones(data->loader->getBones(i));
            pMesh->setVertices(data->loader->getVertices(i), data->loader->getNumVertices(i), data->vertexLayout);
            pMesh->setIndices(data->loader->getIndices(i), data->loader->getNumIndices(i), data->loader->getIndexStride(i));

            HE_INFO("Model create completed: " + data->path);
        }
        data->pModel->setComplete();
        m_WaitListMutex.unlock();
        delete data->loader;
        delete data;
    }
}

gfx::Model::pointer ModelLoader::asyncLoadModel(const std::string& path, const gfx::BufferLayout& vertexLayout)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        return m_pAssetContainer->getAsset(path);
    }
    else
    {
        ModelLoadData* data(NEW ModelLoadData());
        data->path = path;
        data->vertexLayout = vertexLayout;
        data->pModel = gfx::Model::pointer(NEW gfx::Model(vertexLayout));

        if (data->path.rfind(".obj") != std::string::npos)
        {
            data->loader = NEW models::ObjLoader();
        }
        else if (data->path.rfind(".binobj") != std::string::npos)
        {
            data->loader = NEW models::BinObjLoader();
        }
        else
        {
            gfx::Model::pointer m(data->pModel);
            m->setComplete();
            delete data;
            return m;
        }

        m_ModelLoadQueueMutex.lock();
        m_ModelLoadQueue.push(data);
        m_ModelLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(path, data->pModel);

        return data->pModel;
    }
}
gfx::ModelMesh::pointer ModelLoader::asyncLoadModelMesh( const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout )
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        gfx::ModelMesh::pointer pMesh;
        m_WaitListMutex.lock();

        gfx::Model::pointer pModel(m_pAssetContainer->getAsset(path));
        if (pModel->isComplete())
        {
            pMesh = pModel->getMesh(meshName);
            if (pMesh == nullptr)
                pMesh = pModel->getMesh(0);
        }
        else
        {
            if (pModel->getNumMeshes() > 0)
            {
                pMesh = pModel->getMesh(meshName);
            }
            if (pMesh == nullptr)
            {
                gfx::ModelMesh::pointer pMesh = gfx::ModelMesh::pointer(NEW gfx::ModelMesh(meshName));
                
                pModel->addMesh(pMesh);
            }
        }

        m_WaitListMutex.unlock();
        return pMesh;
    }
    else
    {
        ModelLoadData* data(NEW ModelLoadData());
        data->path = path;
        data->vertexLayout = vertexLayout;
        data->pModel = gfx::Model::pointer(NEW gfx::Model(vertexLayout));
        data->pModel->addMesh(gfx::ModelMesh::pointer(NEW gfx::ModelMesh(meshName)));

        if (data->path.rfind(".obj") != std::string::npos)
        {
            data->loader = NEW models::ObjLoader();
        }
        else if (data->path.rfind(".binobj") != std::string::npos)
        {
            data->loader = NEW models::BinObjLoader();
        }
        else
        {
            gfx::ModelMesh::pointer m(data->pModel->getMesh(0));
            delete data;
            std::stringstream stream;
            stream << "Model error: unkown extension (" << path << ")";
            CONSOLE->addMessage(stream.str(), CMSG_TYPE_ERROR);
            return m;
        }

        m_ModelLoadQueueMutex.lock();
        m_ModelLoadQueue.push(data);
        m_ModelLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(path, data->pModel);

        return data->pModel->getMesh(0);
    }
}


void ModelLoader::ModelLoadThread()
{
    HE_INFO("Model Load thread started");
    while (m_ModelLoadQueue.empty() == false)
    {
        m_ModelLoadQueueMutex.lock();
        ModelLoadData* data(m_ModelLoadQueue.front());
        m_ModelLoadQueue.pop();
        m_ModelLoadQueueMutex.unlock();

        if (data->path.rfind(".obj") != std::string::npos || data->path.rfind(".binobj") != std::string::npos)
        {
            try 
            { 
                data->loader->load(data->path, data->vertexLayout); 
                HE_INFO("Model load completed: " + data->path);
                m_ModelInvokeQueueMutex.lock();
                m_ModelInvokeQueue.push(data);
                m_ModelInvokeQueueMutex.unlock();
            }
            catch (err::FileNotFoundException& e)
            {
                HE_ERROR(std::string(e.getMsg().cbegin(), e.getMsg().cend()));
            }            
        }
        else
        {
            delete data->loader;
            delete data;
        }
    }
    HE_INFO("Model load thread stopped");
    m_isModelThreadRunning = false;
}

gfx::Model::pointer ModelLoader::loadModel(const std::string& path, const gfx::BufferLayout& vertexLayout)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        return m_pAssetContainer->getAsset(path);
    }
    else
    {
        ModelLoadData* data(NEW ModelLoadData());
        data->path = path;
        data->vertexLayout = vertexLayout;
        data->pModel = gfx::Model::pointer(NEW gfx::Model(vertexLayout));

        if (data->path.rfind(".obj") != std::string::npos)
        {
            data->loader = NEW models::ObjLoader();
        }
        else if (data->path.rfind(".binobj") != std::string::npos)
        {
            data->loader = NEW models::BinObjLoader();
        }
        else
        {
            gfx::Model::pointer m(data->pModel);
            m->setComplete();
            delete data;
            return m;
        }

        m_pAssetContainer->addAsset(path, data->pModel);

        if (data->path.rfind(".obj") != std::string::npos || data->path.rfind(".binobj") != std::string::npos)
        {
            try 
            { 
                data->loader->load(data->path, data->vertexLayout); 
                std::cout << "**ML INFO** obj load completed: " << data->path << "\n";
            }
            catch (err::FileNotFoundException& e)
            {
                CONSOLE->addMessage(std::string(e.getMsg().cbegin(), e.getMsg().cend()), CMSG_TYPE_ERROR);
                std::wcout << e.getMsg() << "\n";
            }

            uint unloadedMeshes(data->pModel->getNumMeshes());
            for (uint i = 0; i < data->loader->getNumMeshes(); ++i)
            {
                gfx::ModelMesh::pointer pMesh;
                if (unloadedMeshes > 0)
                {
                    for (uint iUnloaded = 0; iUnloaded < unloadedMeshes; ++iUnloaded) // TODO: optimize
                    {
                        if (data->loader->getMeshName(i) == data->pModel->getMesh(iUnloaded)->getName())
                        {
                            pMesh = data->pModel->getMesh(iUnloaded);
                            break;
                        }
                    }
                }
                if (pMesh == nullptr)
                {
                    pMesh = gfx::ModelMesh::pointer(NEW gfx::ModelMesh(data->loader->getMeshName(i)));
                    data->pModel->addMesh(pMesh);
                }

                pMesh->init();
                pMesh->setBones(data->loader->getBones(i));
                pMesh->setVertices(data->loader->getVertices(i), data->loader->getNumVertices(i), data->vertexLayout);
                pMesh->setIndices(data->loader->getIndices(i), data->loader->getNumIndices(i), data->loader->getIndexStride(i));

                std::cout << "**ML INFO** model create completed: " << data->path << "\n";
            }

            data->pModel->setComplete();
        }

        delete data->loader;
        gfx::Model::pointer m(data->pModel);
        delete data;

        return m;
    }
}

gfx::ModelMesh::pointer ModelLoader::loadModelMesh(const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        gfx::ModelMesh::pointer pMesh;

        gfx::Model::pointer pModel(m_pAssetContainer->getAsset(path));
        if (pModel->isComplete())
        {
            pMesh = pModel->getMesh(meshName);
            if (pMesh == nullptr)
                pMesh = pModel->getMesh(0);
        }
        else
        {
            if (pModel->getNumMeshes() > 0)
            {
                pMesh = pModel->getMesh(meshName);
            }
            if (pMesh == nullptr)
            {
                gfx::ModelMesh::pointer pMesh = gfx::ModelMesh::pointer(NEW gfx::ModelMesh(meshName));

                pModel->addMesh(pMesh);
            }
        }

        return pMesh;
    }
    else
    {
        ModelLoadData* data(NEW ModelLoadData());
        data->path = path;
        data->vertexLayout = vertexLayout;
        data->pModel = gfx::Model::pointer(NEW gfx::Model(vertexLayout));
        data->pModel->addMesh(gfx::ModelMesh::pointer(NEW gfx::ModelMesh(meshName)));

        if (data->path.rfind(".obj") != std::string::npos)
        {
            data->loader = NEW models::ObjLoader();
        }
        else if (data->path.rfind(".binobj") != std::string::npos)
        {
            data->loader = NEW models::BinObjLoader();
        }
        else
        {
            gfx::ModelMesh::pointer m(data->pModel->getMesh(0));
            delete data;
            std::stringstream stream;
            stream << "Model error: unkown extension (" << path << ")";
            CONSOLE->addMessage(stream.str(), CMSG_TYPE_ERROR);
            return m;
        }

        m_pAssetContainer->addAsset(path, data->pModel);

        if (data->path.rfind(".obj") != std::string::npos || data->path.rfind(".binobj") != std::string::npos)
        {
            try 
            { 
                data->loader->load(data->path, data->vertexLayout); 
                std::cout << "**ML INFO** obj load completed: " << data->path << "\n";
            }
            catch (err::FileNotFoundException& e)
            {
                CONSOLE->addMessage(std::string(e.getMsg().cbegin(), e.getMsg().cend()), CMSG_TYPE_ERROR);
                std::wcout << e.getMsg() << "\n";
            }            
        }

        delete data->loader;
        gfx::ModelMesh::pointer m(data->pModel->getMesh(0));
        delete data;

        return m;
    }
}

/* GETTERS */
bool ModelLoader::isLoading() const
{
    return m_isModelThreadRunning;
}

} } //end namespace