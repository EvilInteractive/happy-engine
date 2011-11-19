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
//Created: 14/09/2011
#include "StdAfx.h" 

#include "PhysicsShapeLoader.h"
#include "HappyNew.h"

#include "BinaryStream.h"
#include "PhysicsConvexShape.h"

namespace he {
namespace ct {

PhysicsShapeLoader::PhysicsShapeLoader(): 
        m_pConvexAssetContainer(NEW AssetContainer<std::vector<px::PhysicsConvexMesh::pointer>>()),
        m_pConcaveAssetContainer(NEW AssetContainer<std::vector<px::PhysicsConcaveMesh::pointer>>())
{
}


PhysicsShapeLoader::~PhysicsShapeLoader()
{
    delete m_pConvexAssetContainer;
    delete m_pConcaveAssetContainer;
}

const std::vector<px::PhysicsConvexMesh::pointer>& PhysicsShapeLoader::loadConvex(const std::string& path)
{
    if (m_pConvexAssetContainer->isAssetPresent(path))
    {
        return m_pConvexAssetContainer->getAsset(path);
    }
    else
    {
        if (path.rfind(".pxcv") != std::string::npos)
        {
            io::BinaryStream stream(path, io::BinaryStream::Read);
            byte numConvex(stream.readByte());

            std::vector<px::PhysicsConvexMesh::pointer> shapes;
            for (int i = 0; i < numConvex; ++i)
            {
                shapes.push_back(px::PhysicsConvexMesh::pointer(NEW px::PhysicsConvexMesh(stream)));
            }

            m_pConvexAssetContainer->addAsset(path, shapes);
            return m_pConvexAssetContainer->getAsset(path);
        }
        else
        {
            ASSERT(false, "no loader defined for this extension");
            return m_pConvexAssetContainer->getAsset("");
        }
    }
}
const std::vector<px::PhysicsConcaveMesh::pointer>& PhysicsShapeLoader::loadConcave(const std::string& path)
{
    if (m_pConcaveAssetContainer->isAssetPresent(path))
    {
        return m_pConcaveAssetContainer->getAsset(path);
    }
    else
    {
        if (path.rfind(".pxcc") != std::string::npos)
        {
            io::BinaryStream stream(path, io::BinaryStream::Read);
            byte numConcave(stream.readByte());

            std::vector<px::PhysicsConcaveMesh::pointer> shapes;
            for (int i = 0; i < numConcave; ++i)
            {
                shapes.push_back(px::PhysicsConcaveMesh::pointer(NEW px::PhysicsConcaveMesh(stream)));
            }

            m_pConcaveAssetContainer->addAsset(path, shapes);
            return m_pConcaveAssetContainer->getAsset(path);
        }
        else
        {
            ASSERT(false, "no loader defined for this extension");
            return m_pConcaveAssetContainer->getAsset("");
        }
    }
}

} } //end namespace