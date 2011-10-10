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
//Author:  Bastian Damman
//Created: 23/08/2011
//Extended:	Sebastiaan Sprengers
#include "StdAfx.h" 

#include "ModelLoader.h"
#include "HappyNew.h"
#include "BinObjLoader.h"
#include "ObjLoader.h"
#include "FileNotFoundException.h"

namespace he {
namespace ct {

ModelLoader::ModelLoader(): m_isModelThreadRunning(false),
							m_pAssetContainer(NEW AssetContainer<gfx::Model::pointer>())
{
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
        ModelLoadData* data;
        if (m_ModelInvokeQueue.try_pop(data))
        {
            for (uint i = 0; i < data->loader->getNumMeshes(); ++i)
            {
                gfx::ModelMesh::pointer pMesh(NEW gfx::ModelMesh(data->loader->getMeshName(i)));

                pMesh->init();
                pMesh->setVertices(data->loader->getVertices(i), data->loader->getNumVertices(i), data->vertexLayout);
                pMesh->setIndices(data->loader->getIndices(i), data->loader->getNumIndices(i), data->loader->getIndexStride(i));

                data->pModel->addMesh(pMesh);
                std::cout << "**ML INFO** model create completed: " << data->path << "\n";
            }
            data->pModel->setComplete();
            delete data->loader;
            delete data;
        }
    }
}

gfx::Model::pointer ModelLoader::asyncLoadModel(const std::string& path, const gfx::VertexLayout& vertexLayout)
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

		m_ModelLoadQueue.push(data);

		m_pAssetContainer->addAsset(path, data->pModel);

		return data->pModel;
	}
}

void ModelLoader::ModelLoadThread()
{
    std::cout << "**ML INFO** load thread started.\n";
    while (m_ModelLoadQueue.empty() == false)
    {
        ModelLoadData* data;
        if (m_ModelLoadQueue.try_pop(data))
        {
            if (data->path.rfind(".obj") != std::string::npos || data->path.rfind(".binobj") != std::string::npos)
            {
                try 
                { 
                    data->loader->load(data->path, data->vertexLayout); 
                    std::cout << "**ML INFO** obj load completed: " << data->path << "\n";
                    m_ModelInvokeQueue.push(data);
                }
                catch (err::FileNotFoundException& e)
                {
                    std::wcout << e.getMsg() << "\n";
                }            
            }
            else
            {
                delete data->loader;
                delete data;
            }
        }
        else
        {
            //boost::this_thread::yield(); //yes/no?
        }
    }
    std::cout << "**ML INFO** load thread stopped.\n";
    m_isModelThreadRunning = false;
}

} } //end namespace