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
//Created: 17/12/2011

#ifndef _HE_DRAWLIST_CONTAINER_H_
#define _HE_DRAWLIST_CONTAINER_H_
#pragma once

#include "Bound.h"

namespace he {
namespace gfx {

class Drawable;
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
    void draw(BlendFilter blend, const ICamera* camera, const std::function<void(Drawable*)>& drawFunc) const;
    void drawAndCreateDebugMesh(BlendFilter blend, const ICamera* camera, const std::function<void(Drawable*)>& drawFunc,
        he::PrimitiveList<vec3>& vertices, he::PrimitiveList<uint32>& indices) const;

    void insert(Drawable* drawable);
    void remove(Drawable* drawable);
    void forceReevalute(Drawable* drawable);
    void doReevalute(Drawable* drawable);

    const PrimitiveList<Drawable*>* getList() const;
    
private:

    void getContainerIndex(const Drawable* drawable, BlendFilter& main);

    he::PrimitiveList<Drawable*> m_Dynamics;
#ifdef HE_USE_OCTREE
    CullOctree* m_DrawList[BlendFilter_MAX];
#else
    he::PrimitiveList<Drawable*> m_DrawList[BlendFilter_MAX];
#endif

    //Disable default copy constructor and default assignment operator
    DrawListContainer(const DrawListContainer&);
    DrawListContainer& operator=(const DrawListContainer&);
};

} } //end namespace

#endif
