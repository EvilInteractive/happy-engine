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

#include "PhysicsShapeLoader.h"
#include "HappyNew.h"

#include "BinaryStream.h"
#include "PhysicsConvexShape.h"

namespace happyengine {
namespace content {

PhysicsShapeLoader::PhysicsShapeLoader(): m_pAssetContainer(NEW AssetContainer<std::vector<physics::shapes::IPhysicsShape::pointer>>())
{
}


PhysicsShapeLoader::~PhysicsShapeLoader()
{
}

const std::vector<physics::shapes::IPhysicsShape::pointer>& PhysicsShapeLoader::load(const std::string& path)
{
    if (m_pAssetContainer->IsAssetPresent(path))
	{
		return m_pAssetContainer->GetAsset(path);
	}
	else
	{
		if (path.rfind(".bphys") != std::string::npos)
		{
            io::BinaryStream stream(path, io::BinaryStream::Read);
            byte numConvex(stream.readByte());

            std::vector<physics::shapes::IPhysicsShape::pointer> shapes;
            for (int i = 0; i < numConvex; ++i)
            {
                shapes.push_back(physics::shapes::IPhysicsShape::pointer(
                    NEW physics::shapes::PhysicsConvexShape(stream)));
            }

            m_pAssetContainer->AddAsset(path, shapes);
            return m_pAssetContainer->GetAsset(path);
        }
        else
        {
            ASSERT("no loader defined for this extension");
            return m_pAssetContainer->GetAsset("");
        }
    }
}

} } //end namespace