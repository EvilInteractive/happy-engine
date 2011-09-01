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

#ifndef _ASSET_CONTAINER_H_
#define _ASSET_CONTAINER_H_
#pragma once

#include <map>
#include <string>

namespace happyengine {
namespace content {

using namespace std;

template<typename T>
class AssetContainer
{
public:
    AssetContainer(void) { }
    virtual ~AssetContainer(void) { RemoveAllAssets(); }

	bool IsAssetPresent(const string &key) const
    {
	    return m_Map.find(key) != m_Map.end();
    }
    void AddAsset(const string &key, T asset)
    {        
        #if defined DEBUG || _DEBUG
	    cout << "Adding Asset: " << key << "\n\n";
        #endif
	    m_Map[key] = asset;
    }
	void RemoveAsset(const string &key)
    {
	    #if defined DEBUG || _DEBUG
	    cout << "Releasing Asset: " << key << "\n";
        #endif

	    delete m_Map[key];
	    m_Map.erase(key);
    }
	void RemoveAllAssets()
    {
	    /*for_each(m_Map.begin(), m_Map.end(), [&](pair<string, T> obj)
	    {
	        #if defined DEBUG || _DEBUG
	        cout << "Releasing Asset: " << obj.first << "\n";
            #endif

			delete obj.second;
	    });*/
	    m_Map.clear();
    }
    
    T GetAsset(const string& key)
    {
        return m_Map[key];
    }

private:
    map<string, T> m_Map;
	
    //Disable default copy constructor and assignment operator
    AssetContainer(const AssetContainer& copy);
    AssetContainer& operator=(const AssetContainer& other);
};

} } //end namespace

#endif