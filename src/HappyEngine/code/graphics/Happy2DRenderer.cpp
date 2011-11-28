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
#include "StdAfx.h" 

#include "Happy2DRenderer.h"
#include "HappyNew.h"
#include "OpenGL.h"
#include "HappyEngine.h"
#include "MathFunctions.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"

#include <algorithm>
#include <vector>

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Happy2DRenderer::Happy2DRenderer() :	m_pColorEffect(NEW Simple2DEffect()),
                                        m_pTextureEffect(NEW Simple2DTextureEffect()),
                                        m_bAntiAliasing(false),
                                        m_bBlending(false),
                                        m_CurrentColor(1.0f,1.0f,1.0f,1.0f),
                                        m_ViewPortSize(0.0f,0.0f),
                                        m_pModelBuffer(NEW ct::AssetContainer<ModelMesh::pointer, int>()),
                                        m_pTextureBuffer(NEW ct::AssetContainer<std::pair<gfx::Texture2D::pointer, vec2> >()),
                                        m_pTextureQuad(NEW ModelMesh("")),
                                        m_CurrentLayer("default"),
                                        m_RenderFboID(UINT_MAX),
                                        m_DepthRenderTarget(UINT_MAX),
                                        m_pRenderTexture(NEW Texture2D())
{
    
}

Happy2DRenderer::~Happy2DRenderer()
{
    delete m_pColorEffect;
    delete m_pTextureEffect;
    delete m_pModelBuffer;
    delete m_pTextureBuffer;

    glDeleteFramebuffers(1, &m_RenderFboID);
    glDeleteRenderbuffers(1, &m_DepthRenderTarget);
}

void Happy2DRenderer::drawMesh(gui::Shape2D& shape, bool buffered)
{
    if (shape.getPolygon().getVertexCount() < 2)
        return;

    ModelMesh::pointer p;

    int id(0);
    
    for (uint i(0); i < 2; ++i)
    {
        if ((i + 1) > shape.getPolygon().getVertices().size())
            break;

        id += static_cast<int>(shape.getPolygon().getVertices()[(((shape.getPolygon().getVertices().size() / 2) - 1) + i)].x);
        id += static_cast<int>(shape.getPolygon().getVertices()[(((shape.getPolygon().getVertices().size() / 2) - 1) + i)].y);
    }

    id *= 10;

    if (m_pModelBuffer->isAssetPresent(id) && buffered)
    {
        p = m_pModelBuffer->getAsset(id);
    }
    else
    {
        std::vector<VertexPos2D> vertices;
        std::vector<uint> indices(shape.getPolygon().getIndices());

        uint i(0);
        std::for_each(shape.getPolygon().getVertices().cbegin(), shape.getPolygon().getVertices().cend(), [&](vec2 point)
        {
            vertices.push_back(VertexPos2D(point));
            indices.push_back(i++);
        });

        p = ModelMesh::pointer(NEW ModelMesh(""));
        p->init();
        p->setVertices(&vertices[0], vertices.size(), m_VertexLayoutColor);
        p->setIndices(&indices[0], indices.size(), IndexStride_UInt);

        if (buffered)
            m_pModelBuffer->addAsset(id, p);
    }

    GL::heBindVao(p->getVertexArraysID());
    glDrawElements(GL_LINE_LOOP, p->getNumVertices(), p->getIndexType(), 0);
}

void Happy2DRenderer::fillMesh(gui::Shape2D& shape, bool buffered)
{
    if (shape.getPolygon().getVertexCount() < 3)
        return;

    ModelMesh::pointer p;

    int id(0);
    
    if (shape.getPolygon().getVertexCount() == 3)
    {
        for (uint i(0); i < 3; ++i)
        {
            id += static_cast<int>(shape.getPolygon().getVertices()[i].x);
            id += static_cast<int>(shape.getPolygon().getVertices()[i].y);
        }
    }
    else
    {
        for (uint i(0); i < 3; ++i)
        {
            if ((i + 1) > shape.getPolygon().getVertices().size())
                break;

            id += static_cast<int>(shape.getPolygon().getVertices()[(((shape.getPolygon().getVertices().size() / 2) - 2) + i)].x);
            id += static_cast<int>(shape.getPolygon().getVertices()[(((shape.getPolygon().getVertices().size() / 2) - 2) + i)].y);
        }
    }

    if (m_pModelBuffer->isAssetPresent(id) && buffered)
    {
        p = m_pModelBuffer->getAsset(id);
    }
    else
    {
        if (shape.getPolygon().isTriangulated() == false)
        {
            shape.getPolygon().triangulate();
        }

        std::vector<VertexPos2D> vertices;
        std::vector<uint> indices(shape.getPolygon().getIndices());

        std::for_each(shape.getPolygon().getVertices().cbegin(), shape.getPolygon().getVertices().cend(), [&](vec2 point)
        {
            vertices.push_back(VertexPos2D(point));
        });

        p = ModelMesh::pointer(NEW ModelMesh(""));
        p->init();
        p->setVertices(&vertices[0], vertices.size(), m_VertexLayoutColor);
        p->setIndices(&indices[0], indices.size(), IndexStride_UInt);

        if (buffered)
            m_pModelBuffer->addAsset(id, p);
    }

    GL::heBindVao(p->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, p->getNumIndices(), p->getIndexType(), 0);
}

void Happy2DRenderer::drawTexture(const Texture& tex)
{
    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (tex.regionToDraw != RectF(vec2(0.0f,0.0f), vec2(0.0f,0.0f)))
    {
        tcScale.x = tex.regionToDraw.width / tex.tex2D->getWidth();
        tcScale.y = tex.regionToDraw.height / tex.tex2D->getHeight();

        tcOffset.x = tex.regionToDraw.x / tex.tex2D->getWidth();
        tcOffset.y = 1 - (tex.regionToDraw.y / tex.tex2D->getHeight()) - tcScale.y;
    }

    if (tex.newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(tex.newDimensions.x), abs(tex.newDimensions.y));

        if (tex.newDimensions.x < 0) tcScale.x *= -1.0f;
        if (tex.newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {
        size = vec2(static_cast<float>(tex.tex2D->getWidth()), static_cast<float>(tex.tex2D->getHeight()));
    }

    mat44 world(mat44::createTranslation(vec3(tex.pos.x + size.x/2, tex.pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

    m_pTextureEffect->setWorldMatrix(m_matOrthoGraphic * world);

    m_pTextureEffect->setDiffuseMap(tex.tex2D);
    m_pTextureEffect->setAlpha(tex.alpha);
    m_pTextureEffect->setTCOffset(tcOffset);
    m_pTextureEffect->setTCScale(tcScale);

    glDrawElements(GL_TRIANGLES, m_pTextureQuad->getNumIndices(), m_pTextureQuad->getIndexType(), 0);
}

float Happy2DRenderer::getDepth()
{
    float layerDepth(m_Layers[m_CurrentLayer]);
    float depth(0.0f);

    if (m_DepthMap.find(layerDepth) != m_DepthMap.end())
    {
        depth = layerDepth - m_DepthMap[layerDepth];
        m_DepthMap[layerDepth] += 0.001f;
    }
    else
    {
        depth = layerDepth;
        m_DepthMap[layerDepth] = 0.001f;
    }

    return depth;
}

void Happy2DRenderer::resize()
{

    uint renderTexture;
    glGenTextures(1, &renderTexture);

    GL::heBindTexture2D(0, renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_ViewPortSize.x), static_cast<GLsizei>(m_ViewPortSize.y), 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pRenderTexture->init(renderTexture, static_cast<uint>(m_ViewPortSize.x), static_cast<uint>(m_ViewPortSize.y), GL_RGBA8);


    if (m_DepthRenderTarget != UINT_MAX)
        glDeleteRenderbuffers(1, &m_DepthRenderTarget);
    glGenRenderbuffers(1, &m_DepthRenderTarget);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderTarget);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(m_ViewPortSize.x), static_cast<GLsizei>(m_ViewPortSize.y));

    if (m_RenderFboID != UINT_MAX)
        glDeleteFramebuffers(1, &m_RenderFboID);
    glGenFramebuffers(1, &m_RenderFboID);
    GL::heBindFbo(m_RenderFboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderTarget);

    m_matOrthoGraphic = mat44::createOrthoLH(0.0f, m_ViewPortSize.x, 0.0f, m_ViewPortSize.y, 0.0f, 100.0f);
}

/* GENERAL */
void Happy2DRenderer::draw()
{
    PROFILER_BEGIN("Happy2DRenderer::draw");
    vec2 viewPortSize(static_cast<float>(GRAPHICS->getScreenWidth()), static_cast<float>(GRAPHICS->getScreenHeight()));

    if (m_ViewPortSize != viewPortSize)
    {
        m_ViewPortSize = viewPortSize;
        resize();
    }

    setLayer();

    //glScissor(0,200,400,520);
    //glEnable(GL_SCISSOR_TEST);

    GL::heBindFbo(m_RenderFboID);
    GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_bBlending == false)
    {
        GL::heBlendEnabled(false);
    }
    else
    {
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEnabled(true);
    }
    
    GL::heSetDepthWrite(true);

    /* DRAW */

    // shapes
    m_pColorEffect->begin();
    m_pColorEffect->setBlending(m_bBlending);

    PROFILER_BEGIN("Happy2DRenderer::drawshapes");
    std::for_each(m_ShapeBuffer.begin(), m_ShapeBuffer.end(), [&](std::pair<Shape, float> p)
    {
        m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * p.first.shape2D.getWorldMatrix());
        m_pColorEffect->setColor(p.first.color);
        m_pColorEffect->setDepth(p.second);

        if (p.first.fill)
        {
            fillMesh(p.first.shape2D, p.first.buffered);

            if (p.first.antiAliasing && m_bBlending)
            {
                GL::heLineSmoothEnabled(true);
                drawMesh(p.first.shape2D, p.first.buffered);
            }
        }
        else
        {
            if (p.first.antiAliasing && m_bBlending)
                GL::heLineSmoothEnabled(true);
            else
                GL::heLineSmoothEnabled(false);

            drawMesh(p.first.shape2D, p.first.buffered);
        }
    });
    PROFILER_END("Happy2DRenderer::drawshapes");

    GL::heLineSmoothEnabled(false);

    // textures
    m_pTextureEffect->begin();
    m_pTextureEffect->setBlending(m_bBlending);

    PROFILER_BEGIN("Happy2DRenderer::drawtextures");
    GL::heBindVao(m_pTextureQuad->getVertexArraysID());
    std::for_each(m_TextureBuffer.begin(), m_TextureBuffer.end(), [&](std::pair<Texture, float> p)
    {
        m_pTextureEffect->setDepth(p.second);

        drawTexture(p.first);
    });
    PROFILER_END("Happy2DRenderer::drawtextures");

    m_ShapeBuffer.clear();
    m_TextureBuffer.clear();
    m_DepthMap.clear();

    //glDisable(GL_SCISSOR_TEST);

    // fullscreen quad
    GL::heBindFbo(0);
    
    if (m_bBlending == false)
    {
        GL::heBlendEnabled(false);
        //glAlphaFunc(GL_GREATER,0.5f);
    }
    else
    {
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEnabled(true);
    }

    m_pTextureEffect->setDepth(0.5f);
    drawTexture(Texture(m_pRenderTexture, vec2(), vec2(), 1.0f, RectF(), "", false));

    GL::heBlendEnabled(false);
    PROFILER_END("Happy2DRenderer::draw");
}

void Happy2DRenderer::createTextureQuad()
{
    // model texturequad
    m_pTextureQuad->init();

    std::vector<VertexPosTex2D> vertices;
    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, 0.5f),
        vec2(0, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, 0.5f),
        vec2(1, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, -0.5f),
        vec2(0, 1)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, -0.5f),
        vec2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

    m_pTextureQuad->setVertices(&vertices[0], 4, m_VertexLayoutTexture);
    m_pTextureQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

void Happy2DRenderer::init()
{
    m_VertexLayoutColor.addElement(VertexElement(0, VertexElement::Type_Vec2, VertexElement::Usage_Position, 8, 0));

    m_VertexLayoutTexture.addElement(VertexElement(0, VertexElement::Type_Vec2, VertexElement::Usage_Position, 8, 0));
    m_VertexLayoutTexture.addElement(VertexElement(1, VertexElement::Type_Vec2, VertexElement::Usage_TextureCoordinate, 8, 8));
    
    m_ViewPortSize.x = static_cast<float>(GRAPHICS->getScreenWidth());
    m_ViewPortSize.y = static_cast<float>(GRAPHICS->getScreenHeight());

    resize();

    // effects
    m_pColorEffect->load();
    m_pTextureEffect->load();

    createTextureQuad();

    createLayer("default", 50);
    setLayer();
}

void Happy2DRenderer::clearBuffers()
{
    m_pModelBuffer->removeAllAssets();
    m_pTextureBuffer->removeAllAssets();
}

void Happy2DRenderer::createLayer(const std::string& name, byte depth)
{
    #if _DEBUG
    std::string error("Layer: ");
    error += name;
    error += " already exists!";

    std::for_each(m_Layers.cbegin(), m_Layers.cend(), [&](std::pair<std::string, float> p)
    {
        ASSERT(p.first != name, error);

        std::for_each(m_Layers.cbegin(), m_Layers.cend(), [&](std::pair<std::string, float> p2)
        {
            ASSERT((p.second != p2.second) || (p.first == p2.first), "Layer depth already assigned!");
        });
    });
    #endif

    m_Layers[name] = depth;
}

/* SETTERS */
void Happy2DRenderer::setColor(float r, float g, float b, float a)
{
    m_CurrentColor.r(r);
    m_CurrentColor.g(g);
    m_CurrentColor.b(b);
    m_CurrentColor.a(a);
}

void Happy2DRenderer::setColor(const Color& color)
{
    if (m_CurrentColor != color)
        m_CurrentColor = color;
}

void Happy2DRenderer::setAntiAliasing(bool bAA)
{
    m_bAntiAliasing = bAA;
}

void Happy2DRenderer::setBlending(bool bB)
{
    m_bBlending = bB;
}

void Happy2DRenderer::setLayer(const std::string& layer)
{
    if (m_CurrentLayer != layer)
        m_CurrentLayer = layer;
}

/* DRAW METHODS */
void Happy2DRenderer::drawText(const gui::Text& txt, const vec2& pos, bool buffered)
{
    for (uint i(0); i < txt.getText().size(); ++i)
    {
        vec2 position(pos);
        position.y += (txt.getFont()->getFontPixelHeight() + txt.getFont()->getFontLineSpacing()) * i;

        Texture2D::pointer texFont;

        std::stringstream stream;
        stream << txt.getLine(i) << "." << m_CurrentColor.r() << "." << m_CurrentColor.g() << "." << m_CurrentColor.b() << "." << m_CurrentColor.a();

        if (m_pTextureBuffer->isAssetPresent(stream.str()) && buffered)
        {
            texFont = m_pTextureBuffer->getAsset(stream.str()).first;
        }
        else
        {
            texFont = Texture2D::pointer(txt.getFont()->createTextureText(txt.getLine(i), m_CurrentColor, m_bAntiAliasing));

            if (buffered)
                m_pTextureBuffer->addAsset(stream.str(), std::pair<gfx::Texture2D::pointer, vec2>(texFont, vec2()));
        }

        drawTexture2D(texFont, position, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
    }
}
void Happy2DRenderer::drawText(const gui::Text& txt, const RectF& rect, bool buffered)
{
    uint width(0), height(0);
    height = (txt.getFont()->getFontPixelHeight() * txt.getText().size()) + (txt.getFont()->getFontLineSpacing() * (txt.getText().size() - 1));
    std::for_each(txt.getText().cbegin(), txt.getText().cend(), [&](const std::string s)
    {
        uint w(txt.getFont()->getStringWidth(s));

        if (width < w)
            width = w;
    });

    for (uint i(0); i < txt.getText().size(); ++i)
    {
        vec2 textSize;
        Texture2D::pointer texFont;

        std::stringstream stream;
        stream << txt.getLine(i) << "." << m_CurrentColor.r() << "." << m_CurrentColor.g() << "." << m_CurrentColor.b() << "." << m_CurrentColor.a();

        if (m_pTextureBuffer->isAssetPresent(stream.str()) && buffered)
        {
            texFont = m_pTextureBuffer->getAsset(stream.str()).first;
            textSize = m_pTextureBuffer->getAsset(stream.str()).second;
        }
        else
        {
            texFont = Texture2D::pointer(txt.getFont()->createTextureText(txt.getLine(i), m_CurrentColor, m_bAntiAliasing, &textSize));

            if (buffered)
                m_pTextureBuffer->addAsset(stream.str(), std::pair<gfx::Texture2D::pointer, vec2>(texFont, textSize));
        }

        vec2 position;

        switch (txt.getHorizontalAlignment())
        {
        case gui::Text::HAlignment_Left: position.x = rect.x; break;
        case gui::Text::HAlignment_Center: position.x = rect.x + rect.width/2 - textSize.x/2; break;
        case gui::Text::HAlignment_Right: position.x = rect.x + rect.width - textSize.x; break;
        default: ASSERT("unkown font alignment");
        }
        switch (txt.getVerticalAlignment())
        {
        case gui::Text::VAlignment_Top: position.y = rect.y; break;
        case gui::Text::VAlignment_Center: position.y = rect.y + rect.height/2 - height/2; break;
        case gui::Text::VAlignment_Bottom: position.y = rect.y + rect.height - height; break;
        default: ASSERT("unkown font alignment");
        }

        position.y += (txt.getFont()->getFontPixelHeight() + txt.getFont()->getFontLineSpacing()) * i;

        drawTexture2D(texFont, position, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
    }
}

void Happy2DRenderer::drawShape2D(const gui::Shape2D& shape, bool buffered)
{
    m_ShapeBuffer.push_back(std::pair<Shape, float>
        (Shape(shape, m_CurrentColor, false, m_bAntiAliasing, m_CurrentLayer, buffered), getDepth()));
}

void Happy2DRenderer::fillShape2D(const gui::Shape2D& shape, bool buffered)
{
    m_ShapeBuffer.push_back(std::pair<Shape, float>
        (Shape(shape, m_CurrentColor, true, m_bAntiAliasing, m_CurrentLayer, buffered), getDepth()));
}

void Happy2DRenderer::drawTexture2D(const Texture2D::pointer& tex2D, const vec2& pos, const vec2& newDimensions, const float alpha, const RectF& regionToDraw)
{
    m_TextureBuffer.push_back(std::pair<Texture, float>
        (Texture(tex2D, pos, newDimensions, alpha, regionToDraw, m_CurrentLayer, true), getDepth()));
}

} } //end namespace