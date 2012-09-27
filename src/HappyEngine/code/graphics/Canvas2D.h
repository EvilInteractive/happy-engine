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
//Created: 19/03/2012

#ifndef _HE_CANVAS2D_H_
#define _HE_CANVAS2D_H_
#pragma once

#include "I2DDrawable.h"

namespace he {
namespace gfx {
class View;
class Mesh2D;
class Simple2DEffect;
class Simple2DTextureEffect;
class Simple2DFontEffect;
class ModelMesh;
class Texture2D;

class Canvas2D : public I2DDrawable
{
public:

    struct Data
    {
        Data() : resolvedFbufferID(UINT_MAX),
                 resolvedRbufferID(UINT_MAX),
                 fbufferID(UINT_MAX),
                 colorRbufferID(UINT_MAX),
                 depthRbufferID(UINT_MAX)
        {}

        ObjectHandle renderTextureHnd;
        uint resolvedFbufferID;
        uint resolvedRbufferID;
        uint fbufferID;
        uint colorRbufferID;
        uint depthRbufferID;
        GLContext* context;
    };

    struct DrawingState
    {
        Color fillColor;
        Color strokeColor;
        Font* font;
        float globalAlpha;
        float lineWidth;
    };

    /* STATIC */
    static Data* create(GLContext* context, const vec2& size);
    static void resizeData(Data* data, const vec2& size);

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2D(const RectI& absoluteViewport);
    Canvas2D(View* view, const RectF& relativeViewport);
    virtual ~Canvas2D();

    /* GENERAL */
    void translate(const vec2& translation);
    void rotate(float rotation);
    void scale(const vec2& scale);

    void save();
    void restore();

    /* GETTERS */
    Data* getData() const;
    const vec2& getSize() const { return m_CanvasSize; }

    /* SETTERS */
    void setStrokeColor(const Color& newColor);
    void setFillColor(const Color& newColor);

    void setLineWidth(float width);

    void setGlobalAlpha(float alpha);

    void setAutoClearing(bool clearAfterDraw);
    void setPosition(const vec2& position) { m_Position = position; }
    
    /* DRAW METHODS */
    void clear();
    virtual void draw2D(Renderer2D* renderer);

    void strokeRect(const vec2& pos, const vec2& size);
    void fillRect(const vec2& pos, const vec2& size);

    void fillText(const gui::Text& txt, const vec2& pos);

    void drawImage(	const Texture2D* tex2D, const vec2& pos,
                    const vec2& newDimensions = vec2(0.0f,0.0f),
                    const RectF& regionToDraw = RectF(0.0f,0.0f,0.0f,0.0f));

private:

    /* CONSTANT */
    static const int STACK_DEPTH_LIMIT = 16;

    /* EXTRA */
    void init();
    void cleanup();
    mat44 getTransformation();
    float getNewDepth();

    void viewResized();
    void resize(const vec2& newSize);
    eventCallback0<void> m_ViewResizedHandler;

    /* DATAMEMBERS */
    std::vector<mat33> m_TransformationStack;
    ushort m_StackDepth;

    mat44 m_OrthographicMatrix;

    Color m_StrokeColor;
    Color m_FillColor;

    float m_LineWidth;
    float m_GlobalAlpha;

    Data* m_pBufferData;

    Mesh2D* m_pBufferMesh;

    Simple2DEffect* m_pColorEffect;
    Simple2DTextureEffect* m_pTextureEffect;
    Simple2DFontEffect* m_pFontEffect;

    Texture2D* m_pRenderTexture;
    Texture2D* m_pTextBuffer;


    View* m_View;
    RectF m_RelativeViewport;
    vec2 m_CanvasSize;
    vec2 m_Position;

    ModelMesh* m_pTextureQuad;

    float m_PixelDepth;

    bool m_AutoClear;

    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2D(const Canvas2D&);
    Canvas2D& operator=(const Canvas2D&);
};

} } //end namespace

#endif
