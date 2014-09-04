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
    class BezierEffect;
    class Simple2DEffect;
    class Simple2DTextureEffect;
    class Simple2DFontEffect;
    class NinePatchEffect;
    class Mesh2D;

class Canvas2DRendererGL
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2DRendererGL(Canvas2DBuffer* canvasBuffer, GLContext* glContext);
    virtual ~Canvas2DRendererGL();

    /* GENERAL */
    void init();
    void resize();

    /* SETTERS */
    void setColor(const Color& col);

    /* DRAW */
    void fillText(const he::gui::Text& text, const vec2& pos);
    void drawImage( const Texture2D* tex2D, const vec2& pos,
                    const vec2& newDimensions = vec2(),
                    const RectI regionToDraw = RectI());
    void drawSprite(const gui::Sprite* sprite, const vec2& pos,
                    const vec2& size = vec2());
    void blitImage( const Texture2D* tex2D, const vec2& pos,
                    bool useBlending = true,
                    const vec2& newDimensions = vec2(),
                    const RectI regionToDraw = RectI());
    void blitImage3D( const Texture2D* tex2D, const gfx::ICamera* const camera);
    void strokeShape(Mesh2D* const shape);
    void fillShape(Mesh2D* const shape);
    void strokeRect(const RectI& rect);
    void fillRect(const RectI& rect);
    void drawLine(const vec2& p1, const vec2& p2);
    void fillCurve(const vec2& p1, const vec2& tan1, const vec2& tan2, const vec2& p2, const float radius);

private:

    /* EXTRA */
    float addTextToTextBuffer(const char* const buffer,
                              const size_t count,
                              const vec2& pos,
                              const gui::Text::HAlignment alignment,
                              const float maxWidth,
                              const he::gui::Font* const font,
                              const Color& color);

    // For Quad
    Simple2DEffect* m_ColorEffectShape;
    Simple2DTextureEffect* m_TextureEffectQuad;
    NinePatchEffect* m_NinePatchEffectQuad;

    // For dynamic font mesh
    Simple2DFontEffect* m_FontEffectDynFont;

    // For curves
    BezierEffect* m_BezierEffect;
    ModelMesh* m_Bezier;

    uint16 s_Renderers;

    /* MEMBERS */
    vec2 m_Size;
    mat44 m_OrthographicMatrix;

    Color m_Color;

    Canvas2DBuffer* m_CanvasBuffer;

    ModelMesh* m_TextureQuad;

    GLContext* m_Context;

    PrimitiveList<VertexText> m_CharVertexBuffer;
    PrimitiveList<uint32> m_CharIndexBuffer;
    ModelMesh* m_DynamicFontMesh;

    Mesh2D* m_DynamicShapeMesh;
    
    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2DRendererGL(const Canvas2DRendererGL&);
    Canvas2DRendererGL& operator=(const Canvas2DRendererGL&);
};

} } //end namespace

#endif
