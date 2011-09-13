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
#include "BinObjLineLoader.h"
#include "FileNotFoundException.h"

namespace happyengine {
namespace content {

LineLoader::LineLoader(): m_pAssetContainer(NEW AssetContainer<graphics::Line::pointer>())
{
}

LineLoader::~LineLoader()
{
	delete m_pAssetContainer;
}


graphics::Line::pointer LineLoader::loadLine(const std::string& path)
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
            
            graphics::Line::pointer pLine(NEW graphics::Line());
            pLine->setVertices(loader.getPoints());
            pLine->setIndices(loader.getIndices());
            m_pAssetContainer->AddAsset(path, pLine);

		    return pLine;
		}
		else if (path.rfind(".binobj") != std::string::npos)
		{
            lines::BinObjLineLoader loader;
            try { loader.load(path); }
            catch (error::FileNotFoundException& e)
            {
                std::wcout << e.getMsg() << "\n";
            }
            
            graphics::Line::pointer pLine(NEW graphics::Line());
            pLine->setVertices(loader.getPoints());
            pLine->setIndices(loader.getIndices());
            m_pAssetContainer->AddAsset(path, pLine);

		    return pLine;
		}
		else
		{
			ASSERT("unkown model extension");
            graphics::Line::pointer pLine(NEW graphics::Line());
            return pLine;
		}
	}
}

} } //end namespace