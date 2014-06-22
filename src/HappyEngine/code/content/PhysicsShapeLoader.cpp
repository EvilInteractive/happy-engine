//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "PhysicsBinaryStream.h"
#include "PhysicsConvexMesh.h"
#include "PhysicsConcaveMesh.h"

#include "BinaryFileVisitor.h"

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

ObjectHandle PhysicsShapeLoader::loadConvex(const he::String& path)
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
        if (path.rfind(".pxcv") != he::String::npos)
        {
            io::BinaryFileVisitor visitor;
            if (visitor.openRead(path) == false)
            {
                HE_ERROR("Error loading convex mesh: %s", path.c_str());
                return ObjectHandle::unassigned;
            }

            px::PhysicsBinaryStream stream(&visitor);
            ObjectHandle handle(CV_FACTORY->create());
            px::PhysicsConvexMesh* mesh(CV_FACTORY->get(handle));
            mesh->setName(path);
            mesh->load(stream);
            visitor.close();
            m_ConvexAssetContainer.addAsset(path, handle);
            return handle;
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "no loader defined for this extension");
            return ObjectHandle::unassigned;
        }
    }
}
ObjectHandle PhysicsShapeLoader::loadConcave(const he::String& path)
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
        if (path.rfind(".pxcc") != he::String::npos)
        {
            io::BinaryFileVisitor visitor;
            if (visitor.openRead(path) == false)
            {
                HE_ERROR("Error loading concave mesh: %s", path.c_str());
                return ObjectHandle::unassigned;
            }

            px::PhysicsBinaryStream stream(&visitor);
            ObjectHandle handle(CC_FACTORY->create());
            px::PhysicsConcaveMesh* mesh(CC_FACTORY->get(handle));
            mesh->setName(path);
            mesh->load(stream);

            visitor.close();

            m_ConcaveAssetContainer.addAsset(path, handle);
            return m_ConcaveAssetContainer.getAsset(path);
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "no loader defined for this extension");
            return ObjectHandle::unassigned;
        }
    }
}

} } //end namespace