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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#ifndef _HE_PICKER_H_
#define _HE_PICKER_H_
#pragma once

namespace he {
namespace gui {
    class Canvas2D;
}
namespace gfx {
    class Texture2D;
    class PickEffect;
    class IDrawable;
    class Scene;
    class RenderTarget;
    class View;

class Picker
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Picker();
    virtual ~Picker();

    /* GENERAL */
    void init(View* view, Scene* scene);
    // returns ID of mesh in drawlist which is picked, or UINT32_MAX if none
    uint32 pick(const vec2& screenPoint);
    uint32 pick(const vec2& screenPoint, const he::PrimitiveList<IDrawable*>& drawList);

    void drawDebug(gui::Canvas2D* canvas) const;

private:

    /* DATAMEMBERS */
    PickEffect* m_PickEffect;

    Texture2D* m_RenderTexture;
    RenderTarget* m_RenderTarget;

    bool m_Initialized;

    Scene* m_Scene;
    View* m_View;

    /* DEFAULT COPY & ASSIGNMENT */
    Picker(const Picker&);
    Picker& operator=(const Picker&);
};

} } //end namespace

#endif
