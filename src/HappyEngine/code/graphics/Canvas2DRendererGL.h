//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 23/11/2012

#ifndef _HE_CANVAS2DRENDERERGL_H_
#define _HE_CANVAS2DRENDERERGL_H_
#pragma once

#include "Text.h"
#include "Vertex.h"

namespace he {
namespace gui {
    class Sprite;
}
namespace gfx {
    struct Canvas2DBuffer;
    class Texture2D;
    class View;
    class ModelMesh;
    class Simple2DEffect;
    class Simple2DTextureEffect;
    class Simple2DFontEffect;
    class NinePatchEffect;

class Canvas2DRendererGL
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2DRendererGL(Canvas2DBuffer* canvasBuffer, GLContext* glContext);
    virtual ~Canvas2DRendererGL();

    /* GENERAL */
    void init();

    /* GETTERS */

    /* SETTERS */
    void setColor(const Color& col);

    /* DRAW */
    void fillText(const he::gui::Text& text, const vec2& pos);
    void drawImage( const Texture2D* tex2D, const vec2& pos,
                    const vec2& newDimensions = vec2(0.0f,0.0f),
                    const RectI regionToDraw = RectI(0,0,0,0));
    void drawSprite(const gui::Sprite* sprite, const vec2& pos,
                    const vec2& size = vec2(0.0f,0.0f));

private:

    /* INTERNAL */
    struct CharBufferData
    {
        mat44 wvp;
        //vec2 tcOffset;
        //vec2 tcScale;
    };

    /* STATIC */
    static Simple2DEffect* s_ColorEffect;
    static Simple2DTextureEffect* s_TextureEffect;
    static Simple2DFontEffect* s_FontEffect;
    static NinePatchEffect* s_NinePatchEffect;

    static ObjectHandle s_TextureQuadHandle;

    static uint16 s_Renderers;

    /* MEMBERS */
    vec2 m_Size;
    mat44 m_OrthographicMatrix;

    Color m_Color;

    bool m_SurfaceDirty;

    Canvas2DBuffer* m_CanvasBuffer;

    ModelMesh* m_TextureQuad;

    GLContext* m_Context;

    PrimitiveList<VertexPosTex2D> m_CharVertexBuffer;
    PrimitiveList<uint32> m_CharIndexBuffer;
    ModelMesh* m_DynamicFontMesh;
    
    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2DRendererGL(const Canvas2DRendererGL&);
    Canvas2DRendererGL& operator=(const Canvas2DRendererGL&);
};

} } //end namespace

#endif