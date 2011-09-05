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
//Created: 05/09/2011

#include "LineLoader.h"
#include "HappyNew.h"
#include "ObjLineLoader.h"
#include "FileNotFoundException.h"

namespace happyengine {
namespace content {

LineLoader::LineLoader(): m_pAssetContainer(NEW AssetContainer<std::vector<math::Vector3>>())
{
}

LineLoader::~LineLoader()
{
	delete m_pAssetContainer;
}


std::vector<math::Vector3> LineLoader::loadLine(const std::string& path)
{
	if (m_pAssetContainer->IsAssetPresent(path))
	{
		return m_pAssetContainer->GetAsset(path);
	}
	else
	{
		if (path.rfind(".obj") != std::string::npos)
		{
            lines::ObjLineLoader loader;
            try { loader.load(path); }
            catch (error::FileNotFoundException& e)
            {
                std::wcout << e.getMsg() << "\n";
            }
            
		    m_pAssetContainer->AddAsset(path, loader.getPoints());

		    return loader.getPoints();
		}
		else
		{
			ASSERT("unkown model extension");
            return std::vector<math::Vector3>();
		}
	}
}

} } //end namespace