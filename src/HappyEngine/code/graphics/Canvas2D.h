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

#include "Color.h"
#include "vec2.h"
#include "Text.h"
#include "mat33.h"
#include "HappyTypes.h"
#include "Mesh2D.h"
#include "Simple2DEffect.h"

#include <stack>

namespace he {
namespace gfx {

class Canvas2D
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
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2D(Data* pData);
    virtual ~Canvas2D();

    /* GENERAL */
    void translate(const vec2& translation);
    void rotate(float rotation);
    void scale(const vec2& scale);

    void save();
    void restore();

    /* GETTERS */
    Data* getData() const;

    /* SETTERS */
    void setStrokeColor(const Color& newColor);
    void setFillColor(const Color& newColor);

    void setLineWidth(float width);

    void setGlobalAlpha(float alpha);
    
    /* DRAW METHODS */
    void draw();

    void strokeRect(const vec2& pos, const vec2& size);
    void fillRect(const vec2& pos, const vec2& size);

    void strokeArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise);
    void fillArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise);

    void beginPath();
    void closePath();
    void moveTo(const vec2& pos);
    void lineTo(const vec2& pos);
    void arcTo(float radius, float startAngle, float endAngle, bool antiClockwise);
    void quadraticCurveTo(const vec2& cp, const vec2& pos);
    void bezierCurveTo(const vec2& cp1, const vec2& cp2, const vec2& pos);
    void fill();
    void stroke();

    void fillText(const gui::Text& txt, const vec2& pos);

    void drawImage(	const Texture2D* tex2D, const vec2& pos,
                    const vec2& newDimensions = vec2(0.0f,0.0f),
                    const RectF& regionToDraw = RectF(0.0f,0.0f,0.0f,0.0f));

private:

    /* EXTRA */
    void init();

    /* DATAMEMBERS */
    std::stack<mat33> m_TransformationStack;
    ushort m_StackDepth;

    mat44 m_OrthographicMatrix;

    Color m_StrokeColor;
    Color m_FillColor;

    float m_LineWidth;
    float m_GlobalAlpha;

    Data* m_pBufferData;

    Mesh2D* m_pBufferMesh;

    Simple2DEffect* m_pColorEffect;

    Texture2D* m_pRenderTexture;

    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2D(const Canvas2D&);
    Canvas2D& operator=(const Canvas2D&);
};

} } //end namespace

#endif
