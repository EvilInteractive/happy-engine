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
//Created: 17/12/2011

#ifndef _HE_DRAWLIST_CONTAINER_H_
#define _HE_DRAWLIST_CONTAINER_H_
#pragma once

#include "Bound.h"

namespace he {
namespace gfx {

class IDrawable;
class ICamera;
class CullOctree;

class DrawListContainer
{
public:
    DrawListContainer();
    virtual ~DrawListContainer();

    enum BlendFilter
    {
        BlendFilter_Opac,
        BlendFilter_Blend,

        BlendFilter_MAX
    };

    void prepareForRendering();
    void draw(BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc) const;
    void drawAndCreateDebugMesh(BlendFilter blend, const ICamera* camera, const boost::function1<void, IDrawable*>& drawFunc,
        std::vector<vec3>& vertices, std::vector<uint>& indices) const;

    void insert(IDrawable* drawable, bool autoReevalute);
    void remove(IDrawable* drawable);
    void forceReevalute(IDrawable* drawable);
    
private:

    void getContainerIndex(const IDrawable* drawable, BlendFilter& main);

    std::vector<IDrawable*> m_Dynamics;
    CullOctree* m_DrawList[BlendFilter_MAX];

    //Disable default copy constructor and default assignment operator
    DrawListContainer(const DrawListContainer&);
    DrawListContainer& operator=(const DrawListContainer&);
};

} } //end namespace

#endif
