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
//Modified:	Sebastiaan Sprengers

#ifndef _HE_ASSET_CONTAINER_P_H_
#define _HE_ASSET_CONTAINER_P_H_
#pragma once

#include <map>
#include <string>
#include <iostream>

namespace happyengine {
namespace content {
    
template<typename T>
class AssetContainerP
{
public:
    AssetContainerP(void) { }
    virtual ~AssetContainerP(void) { removeAllAssets(); }

	bool isAssetPresent(const std::string &key) const
    {
	    return m_Map.find(key) != m_Map.end();
    }
    void addAsset(const std::string &key, T* asset)
    {        
        #if defined DEBUG || _DEBUG
	    std::cout << "Adding Asset: " << key << "\n\n";
        #endif
	    m_Map[key] = asset;
    }
	void removeAsset(const std::string &key)
    {
	    #if defined DEBUG || _DEBUG
	    std::cout << "Releasing Asset: " << key << "\n\n";
        #endif

	    delete m_Map[key];
	    m_Map.erase(key);
    }
	void removeAllAssets()
    {
	    std::for_each(m_Map.begin(), m_Map.end(), [&](std::pair<std::string, T*> obj)
	    {
	        #if defined DEBUG || _DEBUG
	        std::cout << "Releasing Asset: " << obj.first << "\n\n";
            #endif

			delete obj.second;
	    });

	    m_Map.clear();
    }
    
    T* getAsset(const std::string& key)
    {
        return m_Map[key];
    }

private:
    std::map<std::string, T*> m_Map;
	
    //Disable default copy constructor and assignment operator
    AssetContainerP(const AssetContainerP& copy);
    AssetContainerP& operator=(const AssetContainerP& other);
};

} } //end namespace

#endif