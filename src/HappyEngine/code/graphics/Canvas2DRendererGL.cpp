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

#include "HappyPCH.h" 

#include "Canvas2DRendererGL.h"
#include "Canvas2DBuffer.h"
#include "Texture2D.h"
#include "Simple2DEffect.h"
#include "Simple2DFontEffect.h"
#include "Simple2DTextureEffect.h"
#include "NinePatchEffect.h"
#include "ModelMesh.h"
#include "Vertex.h"
#include "Text.h"
#include "Font.h"
#include "Sprite.h"
#include "Mesh2D.h"
#include "ICamera.h"
#include "DrawContext.h"
#include "BezierEffect.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char*)nullptr + (i))


/* CONSTRUCTOR - DESTRUCTOR */
Canvas2DRendererGL::Canvas2DRendererGL(Canvas2DBuffer* canvasBuffer, GLContext* glContext) :
    m_CanvasBuffer(canvasBuffer),
    m_Color(Color(1.0f,1.0f,1.0f)),
    m_Size(vec2(0,0)),
    m_Context(glContext),
    m_DynamicFontMesh(nullptr),
    m_TextureQuad(nullptr),
    m_DynamicShapeMesh(nullptr),
    m_TextureEffectQuad(nullptr),
    m_ColorEffectShape(nullptr),
    m_FontEffectDynFont(nullptr),
    m_NinePatchEffectQuad(nullptr),
    m_BezierEffect(nullptr),
    m_Bezier(nullptr)
{
}

Canvas2DRendererGL::~Canvas2DRendererGL()
{
    m_TextureQuad->release();
    m_DynamicFontMesh->release();
    m_Bezier->release();
    HEDelete(m_DynamicShapeMesh);

    HEDelete(m_TextureEffectQuad);
    HEDelete(m_ColorEffectShape);
    HEDelete(m_FontEffectDynFont);
    HEDelete(m_NinePatchEffectQuad);
    HEDelete(m_BezierEffect);
}

/* GENERAL */
void Canvas2DRendererGL::resize()
{
    m_Size = m_CanvasBuffer->m_Size;
    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_Size.x, 0.0f, m_Size.y, 0.0f, 1.0f);
}

/* GETTERS */

/* SETTERS */
void Canvas2DRendererGL::setColor(const Color& col)
{
    if (m_Color != col)
    {
        m_Color = col;
    }
}

void Canvas2DRendererGL::setClipRect( const he::RectI& rect )
{
    GL::heScissorEnabled(true);
    GL::heScissorRect(rect);
}

void Canvas2DRendererGL::resetClipRect()
{
    GL::heScissorEnabled(false);
}

int atoix(char chr)
{
    int result(chr - 48);
    if (chr >= 97) chr -= 32;
    if (chr >= 65) result -= 7;
    return result;
}
void Canvas2DRendererGL::fillText(const gui::Text& text, const vec2& pos)
{
    if (text.getTextSize() > 0)
    {
        HIERARCHICAL_PROFILE(__HE_FUNCTION__);
        HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

        HE_ASSERT(text.getFont() != nullptr, "Text has no font set!");
        HE_ASSERT(text.getFont()->isPreCached() == true, "Font needs to be precached!");

        const Texture2D* const tex2D(text.getFont()->getTextureAtlas());

        vec2 linePos(0, 0);
        bool hasBounds(text.hasBounds());

        m_CharVertexBuffer.clear();
        m_CharIndexBuffer.clear();

        const he::gui::Font* const font(text.getFont());
        const uint32 lineSpacing(font->getLineSpacing());
        const gui::Text::HAlignment h = text.getHorizontalAlignment();

        const char* fullText(text.getText());
        const size_t size(text.getTextSize());
        const vec2 bounds(hasBounds? text.getBounds() : vec2(0, 0));
        m_CharVertexBuffer.reserve(size * 4);
        m_CharIndexBuffer.reserve(size * 6);

        Color textColor(1.0f, 1.0f, 1.0f, 1.0f);

        size_t lineCharStart(0);
        size_t lineCounter(0);
        for (uint32 i(0); i < size; ++i)
        {
            const char character(fullText[i]);

            if (character == '\n')
            {
                const size_t lineSize(i - lineCharStart);
                if (lineSize > 0)
                    addTextToTextBuffer(fullText + lineCharStart, lineSize, linePos, h, bounds.x, font, textColor);
                lineCharStart = i + 1;
                linePos.y += lineSpacing;
                linePos.x = 0;
                ++lineCounter;
            }
            else if (character == '&' && i + 3 < size)
            {
                if (i + 3 < size)
                {
                    const size_t lineSize(i - lineCharStart);
                    if (lineSize > 0)
                        linePos.x = addTextToTextBuffer(fullText + lineCharStart, lineSize, linePos, h, bounds.x, font, textColor);

                    char cR(fullText[i + 1]);
                    char cG(fullText[i + 2]);
                    char cB(fullText[i + 3]);

                    int r = atoix(cR);
                    int g = atoix(cG);
                    int b = atoix(cB);

                    textColor = Color(r / 15.0f, g / 15.0f, b / 15.0f, 1.0f);

                    i += 3;
                    lineCharStart = i + 1;
                }
            }
        }
        if (lineCharStart < size)
        {
            const size_t lineSize(size - lineCharStart);
            if (lineSize > 0)
            {
                addTextToTextBuffer(fullText + lineCharStart, lineSize, linePos, h, bounds.x, font, textColor);
                ++lineCounter;
            }
        }

        gui::Text::VAlignment v = text.getVerticalAlignment();

        vec3 offset(pos.x, pos.y, 0);
        switch (v)
        {
        case gui::Text::VAlignment_Top:
            break;
        case gui::Text::VAlignment_Center:
            offset.y += (bounds.y / 2.0f) - ((lineSpacing * lineCounter) / 2.0f);
            break;
        case gui::Text::VAlignment_Bottom:
            offset.y += bounds.y - (lineSpacing * lineCounter);
            break;
        }

        m_DynamicFontMesh->setVertices(&m_CharVertexBuffer[0], m_CharVertexBuffer.size(), MeshUsage_Stream, false);
        m_DynamicFontMesh->setIndices(&m_CharIndexBuffer[0], m_CharIndexBuffer.size(), IndexStride_UInt, MeshUsage_Stream);

        m_FontEffectDynFont->setDiffuseMap(tex2D);
        m_FontEffectDynFont->setBlendColor(m_Color);
        m_FontEffectDynFont->setWorldMatrix(m_OrthographicMatrix * mat44::createTranslation(offset));

        GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

        DrawContext context;
        context.m_VBO = m_DynamicFontMesh->getVBO();
        context.m_IBO = m_DynamicFontMesh->getIBO();
        m_FontEffectDynFont->apply(context);
        m_DynamicFontMesh->draw();
    }
}
float Canvas2DRendererGL::addTextToTextBuffer( const char* const buffer, const size_t count, const vec2& pos, 
    const gui::Text::HAlignment alignment, const float maxWidth, const he::gui::Font* const font, const Color& color )
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    const Texture2D* const tex2D(font->getTextureAtlas());
    const uint32 texWidth(tex2D->getWidth());
    const uint32 texHeight(tex2D->getHeight());

    vec2 glyphPos(pos);
    float hoffset(0.0f);
    switch (alignment)
    {
    case gui::Text::HAlignment_Left:
        break;
    case gui::Text::HAlignment_Center:
        hoffset = (maxWidth / 2.0f - font->getStringWidth(buffer, count) / 2.0f);
        break;
    case gui::Text::HAlignment_Right:
        hoffset = (maxWidth - font->getStringWidth(buffer, count));
        break;
    }
    glyphPos.x += hoffset;

    const size_t charCount(m_CharVertexBuffer.size() / 4);
    m_CharVertexBuffer.resize((charCount + count) * 4);
    m_CharIndexBuffer.resize((charCount + count) * 6);
    for (uint32 i(0); i < count; ++i)
    {
        const char character(buffer[i]);
        const gui::Font::CharData& charData = font->getCharTextureData(character);
        const RectF& regionToDraw = charData.textureRegion;
        const float glyphOffset(charData.offset);

        const vec2 tcScale(regionToDraw.width / texWidth, regionToDraw.height / texHeight);
        const vec2 tcOffset(regionToDraw.x / texWidth, ((texHeight - regionToDraw.y) / texHeight) - tcScale.y);

        const vec2 size(regionToDraw.width, regionToDraw.height);

        const size_t offset(charCount + i);
        VertexText& vertex0(m_CharVertexBuffer[offset * 4 + 0]);
        VertexText& vertex1(m_CharVertexBuffer[offset * 4 + 1]);
        VertexText& vertex2(m_CharVertexBuffer[offset * 4 + 2]);
        VertexText& vertex3(m_CharVertexBuffer[offset * 4 + 3]);

        const vec4 rgba(color.rgba());
        vertex0.position = glyphPos + vec2(0, size.y) + vec2(glyphOffset, 0.f);
        vertex0.textureCoord = tcOffset;
        vertex0.color = rgba;
        vertex1.position = glyphPos + vec2(size.x, size.y) + vec2(glyphOffset, 0.f);
        vertex1.textureCoord = tcOffset + vec2(tcScale.x, 0);
        vertex1.color = rgba;
        vertex2.position = glyphPos+ vec2(glyphOffset,0.f);
        vertex2.textureCoord = tcOffset + vec2(0, tcScale.y);
        vertex2.color = rgba;
        vertex3.position = glyphPos + vec2(size.x, 0) + vec2(glyphOffset, 0.f);
        vertex3.textureCoord = tcOffset + tcScale;
        vertex3.color = rgba;

        m_CharIndexBuffer[offset * 6 + 0] = offset * 4 + 0;
        m_CharIndexBuffer[offset * 6 + 1] = offset * 4 + 1;
        m_CharIndexBuffer[offset * 6 + 2] = offset * 4 + 2;

        m_CharIndexBuffer[offset * 6 + 3] = offset * 4 + 1;
        m_CharIndexBuffer[offset * 6 + 4] = offset * 4 + 3;
        m_CharIndexBuffer[offset * 6 + 5] = offset * 4 + 2;

        glyphPos.x += charData.advance.x;
    }
    return glyphPos.x;
}


void Canvas2DRendererGL::drawImage( const Texture2D* tex2D, const vec2& pos,
                                    const vec2& newDimensions,
                                    const RectI regionToDraw)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (regionToDraw != RectI(0,0,0,0))
    {
        tcScale.x = static_cast<float>(regionToDraw.width / tex2D->getWidth());
        tcScale.y = static_cast<float>(regionToDraw.height / tex2D->getHeight());

        tcOffset.x = static_cast<float>(regionToDraw.x / tex2D->getWidth());
        tcOffset.y = static_cast<float>(1 - (regionToDraw.y / tex2D->getHeight()) - tcScale.y);
    }

    if (newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(newDimensions.x), abs(newDimensions.y));

        if (newDimensions.x < 0) tcScale.x *= -1.0f;
        if (newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {
        size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
    }

    mat44 world(mat44::createTranslation(vec3(pos.x + size.x/2, pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

    m_TextureEffectQuad->setWorldMatrix(m_OrthographicMatrix * world);
    m_TextureEffectQuad->setDiffuseMap(tex2D);
    m_TextureEffectQuad->setAlpha(m_Color.a());
    m_TextureEffectQuad->setTCOffset(tcOffset);
    m_TextureEffectQuad->setTCScale(tcScale);
    m_TextureEffectQuad->setDepth(0.5f);
    m_TextureEffectQuad->setBlended(true);
    
    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    DrawContext context;
    context.m_VBO = m_TextureQuad->getVBO();
    context.m_IBO = m_TextureQuad->getIBO();
    m_TextureEffectQuad->apply(context);
    m_TextureQuad->draw();
}
void Canvas2DRendererGL::drawSprite(const gui::Sprite* sprite, const vec2& pos,
                                    const vec2& size)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    if (sprite->getFlags() & gui::Sprite::UNIFORM_SCALE)
    {
        drawImage(sprite->getRenderTexture(), pos, size);
    }
    else
    {
        vec2 s(sprite->getSize());
        RectF c(sprite->getCenter());

        if (size != vec2(0.0f,0.0f))
        {
            //vec2 scale(size.x / s.x, size.y / s.y);
            c.width = size.x - (s.x - c.width);
            c.height = size.y - (s.y - c.height);
            s = size;
        }

        drawNinePatch(sprite->getRenderTexture(), pos, c, s);
    }
}

void Canvas2DRendererGL::drawNinePatch( const gfx::Texture2D* tex2D, const vec2& pos, const he::RectF& centerBlock, const vec2& size )
{
    mat44 world(mat44::createTranslation(vec3(pos.x + size.x/2, pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

    m_NinePatchEffectQuad->setWorldMatrix(m_OrthographicMatrix * world);
    m_NinePatchEffectQuad->setCenter(centerBlock);
    m_NinePatchEffectQuad->setSize(size);
    m_NinePatchEffectQuad->setOriginalSize(he::vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight())));
    m_NinePatchEffectQuad->setDiffuseMap(tex2D);
    m_NinePatchEffectQuad->setDepth(0.5f);
    m_NinePatchEffectQuad->setBlendColor(m_Color);

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);
    DrawContext context;
    context.m_VBO = m_TextureQuad->getVBO();
    context.m_IBO = m_TextureQuad->getIBO();
    m_NinePatchEffectQuad->apply(context);
    m_TextureQuad->draw();
}

void Canvas2DRendererGL::blitImage( const Texture2D* tex2D, const vec2& pos,
                                    bool useBlending,
                                    const vec2& newDimensions,
                                    const RectI regionToDraw)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (regionToDraw != RectI(0,0,0,0))
    {
        tcScale.x = static_cast<float>(regionToDraw.width) / tex2D->getWidth();
        tcScale.y = static_cast<float>(regionToDraw.height) / tex2D->getHeight();

        tcOffset.x = static_cast<float>(regionToDraw.x) / tex2D->getWidth();
        tcOffset.y = 1.0f - (static_cast<float>(regionToDraw.y) / tex2D->getHeight()) - tcScale.y;
    }

    if (newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(newDimensions.x), abs(newDimensions.y));

        if (newDimensions.x < 0) tcScale.x *= -1.0f;
        if (newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {
        size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
    }

    mat44 world(mat44::createTranslation(vec3(pos.x + size.x/2, pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));
    
    m_TextureEffectQuad->setWorldMatrix(m_OrthographicMatrix * world);
    m_TextureEffectQuad->setDiffuseMap(tex2D);
    m_TextureEffectQuad->setAlpha(1.0f);
    m_TextureEffectQuad->setTCOffset(tcOffset);
    m_TextureEffectQuad->setTCScale(tcScale);
    m_TextureEffectQuad->setDepth(0.5f);
    m_TextureEffectQuad->setBlended(useBlending);

    DrawContext context;
    context.m_VBO = m_TextureQuad->getVBO();
    context.m_IBO = m_TextureQuad->getIBO();
    m_TextureEffectQuad->apply(context);
    m_TextureQuad->draw();
}

void Canvas2DRendererGL::blitImage3D( const Texture2D* tex2D, const gfx::ICamera* const camera )
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    vec2 tcOffset(0.0f, 0.0f);
    vec2 tcScale(1.0f, -1.0f);
    float aspect(static_cast<float>(tex2D->getWidth() / static_cast<float>(tex2D->getHeight())));

    const vec3 position(camera->getPosition() + camera->getLook() * 6.0f); // 6 meters in front of camera
    const mat44 world(mat44::createTranslation(position) * mat44::createBillboard(camera));

    m_TextureEffectQuad->setWorldMatrix(camera->getViewProjection() * mat44::createTranslation(position) * mat44::createBillboard(camera) * mat44::createScale(vec3(8.0f, 8.0f / aspect, 1.0f)));
    m_TextureEffectQuad->setDiffuseMap(tex2D);
    m_TextureEffectQuad->setAlpha(1.0f);
    m_TextureEffectQuad->setTCOffset(tcOffset);
    m_TextureEffectQuad->setTCScale(tcScale);
    m_TextureEffectQuad->setDepth(0.5f);
    m_TextureEffectQuad->setBlended(true);

    DrawContext context;
    context.m_VBO = m_TextureQuad->getVBO();
    context.m_IBO = m_TextureQuad->getIBO();
    m_TextureEffectQuad->apply(context);

    GL::heSetCullFace(true); // must be nicer...

    m_TextureQuad->draw();

    GL::heSetCullFace(false);
}

void Canvas2DRendererGL::strokeShape(Mesh2D* const shape)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    m_ColorEffectShape->setColor(m_Color);
    m_ColorEffectShape->setWorldMatrix(m_OrthographicMatrix);
    m_ColorEffectShape->setDepth(0.5f);

    if (!shape->hasBuffer())
        shape->createBuffer(true);

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    DrawContext context;
    context.m_VBO = shape->getVBOID();
    context.m_IBO = shape->getVBOIndexID();
    m_ColorEffectShape->apply(context);
    shape->draw();
}

void Canvas2DRendererGL::fillShape(Mesh2D* const shape)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    m_ColorEffectShape->setColor(m_Color);
    m_ColorEffectShape->setWorldMatrix(m_OrthographicMatrix);
    m_ColorEffectShape->setDepth(0.5f);

    if (!shape->hasBuffer())
        shape->createBuffer(false);

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    DrawContext context;
    context.m_VBO = shape->getVBOID();
    context.m_IBO = shape->getVBOIndexID();
    m_ColorEffectShape->apply(context);
    shape->draw();
}

void Canvas2DRendererGL::strokeRect(const RectI& rect)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    m_ColorEffectShape->setColor(m_Color);
    m_ColorEffectShape->setWorldMatrix(m_OrthographicMatrix);
    m_ColorEffectShape->setDepth(0.5f);

    m_DynamicShapeMesh->clear();
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x), static_cast<float>(rect.y)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x) + rect.width, static_cast<float>(rect.y)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x) + rect.width, static_cast<float>(rect.y + rect.height)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x), static_cast<float>(rect.y + rect.height)));
    m_DynamicShapeMesh->createBuffer(true);

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    DrawContext context;
    context.m_VBO = m_DynamicShapeMesh->getVBOID();
    context.m_IBO = m_DynamicShapeMesh->getVBOIndexID();
    m_ColorEffectShape->apply(context);
    m_DynamicShapeMesh->draw();
}

void Canvas2DRendererGL::fillRect(const RectI& rect)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    m_ColorEffectShape->setColor(m_Color);
    m_ColorEffectShape->setWorldMatrix(m_OrthographicMatrix);
    m_ColorEffectShape->setDepth(0.5f);

    m_DynamicShapeMesh->clear();
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x), static_cast<float>(rect.y)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x) + rect.width, static_cast<float>(rect.y)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x) + rect.width, static_cast<float>(rect.y + rect.height)));
    m_DynamicShapeMesh->addVertex(vec2(static_cast<float>(rect.x), static_cast<float>(rect.y + rect.height)));
    m_DynamicShapeMesh->createBuffer(false);

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    DrawContext context;
    context.m_VBO = m_DynamicShapeMesh->getVBOID();
    context.m_IBO = m_DynamicShapeMesh->getVBOIndexID();
    m_ColorEffectShape->apply(context);
    m_DynamicShapeMesh->draw();
}

void Canvas2DRendererGL::drawLine(const vec2& p1, const vec2& p2)
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    m_ColorEffectShape->setColor(m_Color);
    m_ColorEffectShape->setWorldMatrix(m_OrthographicMatrix);
    m_ColorEffectShape->setDepth(0.5f);

    m_DynamicShapeMesh->clear();
    m_DynamicShapeMesh->addVertex(p1);
    m_DynamicShapeMesh->addVertex(p2);
    m_DynamicShapeMesh->createBuffer(true);

    DrawContext context;
    context.m_VBO = m_DynamicShapeMesh->getVBOID();
    context.m_IBO = m_DynamicShapeMesh->getVBOIndexID();
    m_ColorEffectShape->apply(context);
    m_DynamicShapeMesh->draw();
}

void Canvas2DRendererGL::fillCurve( const vec2& p1, const vec2& tan1, const vec2& tan2, const vec2& p2, const float radius )
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    GL::heBindFbo(m_CanvasBuffer->m_FrameBufferId);

    m_BezierEffect->setColor(m_Color);
    m_BezierEffect->setRadius(radius);

    m_BezierEffect->setCurve(p1, p1 + tan1, p2 + tan2, p2);
    m_BezierEffect->setWVPMatrix(m_OrthographicMatrix);

    DrawContext context;
    context.m_VBO = m_Bezier->getVBO();
    context.m_IBO = m_Bezier->getIBO();
    m_BezierEffect->apply(context);
    m_Bezier->draw();
}

/* INTERNAL */
void Canvas2DRendererGL::init()
{
    HE_ASSERT(m_CanvasBuffer->m_GlContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    m_Size = m_CanvasBuffer->m_Size;

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_Size.x, 0.0f, m_Size.y, 0.0f, 1.0f);

    ResourceFactory<ModelMesh>* const modelMeshFactory(ResourceFactory<ModelMesh>::getInstance());
    ObjectHandle modelHandle(modelMeshFactory->create());
    m_TextureQuad = modelMeshFactory->get(modelHandle);
    {
        VertexLayout vLayout;
        vLayout.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 0));
        vLayout.addElement(VertexElement(eShaderAttribute_TextureCoordiante, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 8));

        he::ObjectList<VertexPosTex2D> vertices(4);
        vertices.add(
            VertexPosTex2D(vec2(-0.5f, 0.5f),
            vec2(0, 0)));

        vertices.add(
            VertexPosTex2D(vec2(0.5f, 0.5f),
            vec2(1, 0)));

        vertices.add(
            VertexPosTex2D(vec2(-0.5f, -0.5f),
            vec2(0, 1)));

        vertices.add(
            VertexPosTex2D(vec2(0.5f, -0.5f),
            vec2(1, 1)));

        he::PrimitiveList<uint16> indices(6);
        indices.add(0); indices.add(1); indices.add(2);
        indices.add(1); indices.add(3); indices.add(2);

        m_TextureQuad->init(vLayout, MeshDrawMode_Triangles);
        m_TextureQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static, false);
        m_TextureQuad->setIndices(&indices[0], 6, IndexStride_UShort, gfx::MeshUsage_Static);
        m_TextureQuad->setName("Canvas2DRendererGL-TextureQuad");

        m_TextureQuad->setLoaded(eLoadResult_Success);
    }

    m_DynamicFontMesh = modelMeshFactory->get(modelMeshFactory->create());
    {
        VertexLayout vLayout;
        vLayout.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 0));
        vLayout.addElement(VertexElement(eShaderAttribute_TextureCoordiante, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 8));
        vLayout.addElement(VertexElement(eShaderAttribute_Color, eShaderAttributeType_Float, eShaderAttributeTypeComponents_4, 16));

        m_DynamicFontMesh->init(vLayout, MeshDrawMode_Triangles);
        m_DynamicFontMesh->setVertices(nullptr, 0, MeshUsage_Dynamic, false);
        m_DynamicFontMesh->setIndices(nullptr, 0, IndexStride_UInt, MeshUsage_Dynamic);
        m_DynamicFontMesh->setName("Canvas2DRendererGL-FontMesh");
        m_DynamicFontMesh->setLoaded(eLoadResult_Success);
    }

    m_DynamicShapeMesh = HENew(Mesh2D)(false);


    m_Bezier = modelMeshFactory->get(modelMeshFactory->create());
    {
        VertexLayout vertexLayoutBezier;
        vertexLayoutBezier.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 0));

        m_Bezier->init(vertexLayoutBezier, gfx::MeshDrawMode_Points);
        he::vec2 bezierVert(0, 0);
        int16 bezierInd(0);
        m_Bezier->setVertices(&bezierVert, 1, gfx::MeshUsage_Static, false);
        m_Bezier->setIndices(&bezierInd, 1, IndexStride_UShort, gfx::MeshUsage_Static);
        m_Bezier->setName("Canvas2DRendererGL-Bezier");
        m_Bezier->setLoaded(eLoadResult_Success);
    }


    if (m_ColorEffectShape == nullptr)
    {
        m_ColorEffectShape = HENew(Simple2DEffect)();
        m_ColorEffectShape->init(m_DynamicShapeMesh->getVertexLayout());
    }
    if (m_TextureEffectQuad == nullptr)
    {
        m_TextureEffectQuad = HENew(Simple2DTextureEffect)();
        m_TextureEffectQuad->init(m_TextureQuad->getVertexLayout());
    }
    if (m_FontEffectDynFont == nullptr)
    {
        m_FontEffectDynFont = HENew(Simple2DFontEffect)();
        m_FontEffectDynFont->init(m_DynamicFontMesh->getVertexLayout());
    }
    if (m_NinePatchEffectQuad == nullptr)
    {
        m_NinePatchEffectQuad = HENew(NinePatchEffect)();
        m_NinePatchEffectQuad->init(m_TextureQuad->getVertexLayout());
    }
    if (m_BezierEffect == nullptr)
    {
        m_BezierEffect = HENew(BezierEffect)();
        m_BezierEffect->init(m_Bezier->getVertexLayout());
    }
}


} } //end namespace
