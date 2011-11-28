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
//Author:  Sebastiaan Sprengers
//Created: 25/08/2011

#ifndef _HE_HAPPY_2D_RENDERER_H_
#define _HE_HAPPY_2D_RENDERER_H_
#pragma once

#include "Vertex.h"
#include "Color.h"
#include "Texture2D.h"
#include "Simple2DEffect.h"
#include "Simple2DTextureEffect.h"
#include "mat44.h"
#include "Font.h"
#include "vec2.h"
#include "HappyTypes.h"
#include "AssetContainer.h"
#include "ModelMesh.h"
#include "Rect.h"
#include "Text.h"
#include "Polygon.h"
#include "Shape2D.h"
#include "Rectangle2D.h"
#include "RoundedRectangle2D.h"
#include "Ellipse2D.h"
#include "Polygon2D.h"
#include "Line2D.h"

#include <map>

namespace he {
namespace gfx {

class Happy2DRenderer
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Happy2DRenderer();
    virtual ~Happy2DRenderer();

    /* GENERAL */
    void init();
    void draw();

    void clearBuffers();

    // * Create new GUI layer, max depth 99, min depth 0, 50 is default. *
    void createLayer(const std::string& name, byte depth = 50);

    /* SETTERS */

    // * Set the color of the 2D brush. *
    void setColor(float r, float g, float b, float a = 1.0f);
    void setColor(const Color& color);
    // * Turn on/off anti-aliasing. *
    void setAntiAliasing(bool bAA);
    // * Switch between blending & testing. *
    void setBlending(bool bB);
    // * Set current GUI layer, default = depth 50
    void setLayer(const std::string& layer = "default");	

    /* DRAW METHODS */

    // * Draws the text as a 2D texture on the screen. *
    void drawText(const gui::Text& txt, const vec2& pos, bool buffered = false);
    void drawText(const gui::Text& txt, const RectF& rect, bool buffered = false);
    // * Draws a 2D shape. *
    void drawShape2D(const gui::Shape2D& shape, bool buffered = false);
    // * Draws a filled 2D shape. *
    void fillShape2D(const gui::Shape2D& shape, bool buffered = false);
    // * Draws a 2D texture with options for resizing, alpha, cliprect. *
    void drawTexture2D(	const Texture2D::pointer& tex2D, const vec2& pos,
                        const vec2& newDimensions = vec2(0.0f,0.0f),
                        const float alpha = 1.0f, const RectF& regionToDraw = RectF(0.0f,0.0f,0.0f,0.0f));

private:

    struct Shape
    {
    public:

        Shape(	const gui::Shape2D& s,
            const Color& c,
            bool f, bool a,
            const std::string& l,
            bool buffered)
            : shape2D(s),
            color(c),
            fill(f),
            antiAliasing(a),
            layer(l),
            buffered(buffered)
        {
        }

        gui::Shape2D shape2D;
        Color color;
        bool fill;
        bool antiAliasing;
        std::string layer;
        bool buffered;
    };

    struct Texture
    {
    public:

        Texture(	const Texture2D::pointer& tex,
            const vec2& pos,
            const vec2& newDimensions,
            const float alpha,
            const RectF& regionToDraw,
            const std::string& layer,
            bool buffered)
            : tex2D(tex),
            pos(pos),
            newDimensions(newDimensions),
            alpha(alpha),
            regionToDraw(regionToDraw),
            layer(layer),
            buffered(buffered)
        {
        }

        Texture2D::pointer tex2D;
        vec2 pos;
        vec2 newDimensions;
        float alpha;
        RectF regionToDraw;
        std::string layer;
        bool buffered;
    };

    void drawMesh(gui::Shape2D& shape, bool buffered = false);
    void fillMesh(gui::Shape2D& shape, bool buffered = false);
    void drawTexture(const Texture& tex);
    float getDepth();
    void createTextureQuad();
    void resize();

    /* DATAMEMBERS */
    uint m_RenderFboID;
    uint m_DepthRenderTarget;
    Texture2D::pointer m_pRenderTexture;

    bool m_bAntiAliasing;
    bool m_bBlending;

    Color m_CurrentColor;

    VertexLayout m_VertexLayoutColor;
    VertexLayout m_VertexLayoutTexture;

    Simple2DEffect* m_pColorEffect;
    Simple2DTextureEffect* m_pTextureEffect;

    vec2 m_ViewPortSize;
    mat44 m_matOrthoGraphic;

    ModelMesh::pointer m_pTextureQuad;

    ct::AssetContainer<ModelMesh::pointer, int>* m_pModelBuffer;
    ct::AssetContainer<std::pair<gfx::Texture2D::pointer, vec2> >* m_pTextureBuffer;

    std::map<std::string, float> m_Layers;
    std::string m_CurrentLayer;
    
    std::vector<std::pair<Shape, float> > m_ShapeBuffer;
    std::vector<std::pair<Texture, float> > m_TextureBuffer;
    std::map<float, float> m_DepthMap;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Happy2DRenderer(const Happy2DRenderer&);
    Happy2DRenderer& operator=(const Happy2DRenderer&);
};

} } //end namespace

#endif
