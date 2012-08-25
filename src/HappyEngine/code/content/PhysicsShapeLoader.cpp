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
//Created: 14/09/2011
#include "HappyPCH.h" 

#include "PhysicsShapeLoader.h"

#include "BinaryStream.h"
#include "PhysicsConvexMesh.h"
#include "PhysicsConcaveMesh.h"

#define CC_FACTORY ResourceFactory<px::PhysicsConcaveMesh>::getInstance()
#define CV_FACTORY ResourceFactory<px::PhysicsConvexMesh>::getInstance()

namespace he {
namespace ct {

PhysicsShapeLoader::PhysicsShapeLoader()
{
}


PhysicsShapeLoader::~PhysicsShapeLoader()
{
    CC_FACTORY->garbageCollect();
    CV_FACTORY->garbageCollect();
}

ObjectHandle PhysicsShapeLoader::loadConvex(const std::string& path)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_ConvexAssetContainer.isAssetPresent(path) && CV_FACTORY->isAlive(m_ConvexAssetContainer.getAsset(path)))
    {
        ObjectHandle handle(m_ConvexAssetContainer.getAsset(path));
        CV_FACTORY->instantiate(handle);
        return handle;
    }
    else
    {
        if (path.rfind(".pxcv") != std::string::npos)
        {
            io::BinaryStream stream;
            if (stream.open(path, io::BinaryStream::Read) == false)
            {
                HE_ERROR("Error loading convex mesh: %s", path.c_str());
                return ObjectHandle::unassigned;
            }

            ObjectHandle handle(CV_FACTORY->create());
            px::PhysicsConvexMesh* mesh(CV_FACTORY->get(handle));
            mesh->setName(path);
            mesh->load(stream);

            m_ConvexAssetContainer.addAsset(path, handle);
            return handle;
        }
        else
        {
            HE_ASSERT(false, "no loader defined for this extension");
            return ObjectHandle::unassigned;
        }
    }
}
ObjectHandle PhysicsShapeLoader::loadConcave(const std::string& path)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_ConcaveAssetContainer.isAssetPresent(path) && CC_FACTORY->isAlive(m_ConcaveAssetContainer.getAsset(path)))
    {
        ObjectHandle handle(m_ConcaveAssetContainer.getAsset(path));
        CC_FACTORY->instantiate(handle);
        return handle;
    }
    else
    {
        if (path.rfind(".pxcc") != std::string::npos)
        {
            io::BinaryStream stream;
            if (stream.open(path, io::BinaryStream::Read) == false)
            {
                HE_ERROR("Error loading concave mesh: %s", path.c_str());
                return ObjectHandle::unassigned;
            }

            ObjectHandle handle(CC_FACTORY->create());
            px::PhysicsConcaveMesh* mesh(CC_FACTORY->get(handle));
            mesh->setName(path);
            mesh->load(stream);

            m_ConcaveAssetContainer.addAsset(path, handle);
            return m_ConcaveAssetContainer.getAsset(path);
        }
        else
        {
            HE_ASSERT(false, "no loader defined for this extension");
            return ObjectHandle::unassigned;
        }
    }
}

} } //end namespace