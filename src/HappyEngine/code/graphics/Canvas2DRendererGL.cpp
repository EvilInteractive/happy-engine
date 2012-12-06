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

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char*)nullptr + (i))

/* STATIC */
Simple2DEffect* Canvas2DRendererGL::s_ColorEffect = nullptr;
Simple2DTextureEffect* Canvas2DRendererGL::s_TextureEffect = nullptr;
Simple2DFontEffect* Canvas2DRendererGL::s_FontEffect = nullptr;
NinePatchEffect* Canvas2DRendererGL::s_NinePatchEffect = nullptr;
ObjectHandle Canvas2DRendererGL::s_TextureQuadHandle = ObjectHandle::unassigned;
uint16 Canvas2DRendererGL::s_Renderers = 0;

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2DRendererGL::Canvas2DRendererGL(Canvas2DBuffer* canvasBuffer, GLContext* glContext) :
    m_CanvasBuffer(canvasBuffer),
    m_Color(Color(1.0f,1.0f,1.0f)),
    m_SurfaceDirty(true),
    m_Size(vec2(0,0)),
    m_Context(glContext),
    m_DynamicFontMesh(nullptr)
{
    ++s_Renderers;
}

Canvas2DRendererGL::~Canvas2DRendererGL()
{
    --s_Renderers;

    m_TextureQuad->release();
    m_DynamicFontMesh->release();

    if (s_Renderers == 0)
    {
        delete s_TextureEffect;
        delete s_ColorEffect;
        delete s_FontEffect;
        delete s_NinePatchEffect;
    }

}

/* GENERAL */

/* GETTERS */

/* SETTERS */
void Canvas2DRendererGL::setColor(const Color& col)
{
    if (m_Color != col)
    {
        m_Color = col;
    }
}

void Canvas2DRendererGL::fillText(const gui::Text& text, const vec2& pos)
{
    HE_ASSERT(m_CanvasBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    HE_ASSERT(text.getFont() != nullptr, "Text has no font set!");
    HE_ASSERT(text.getFont()->isPreCached() == true, "Font needs to be precached!");

    Texture2D* tex2D = text.getFont()->getTextureAtlas();

    vec2 linePos = pos;
    bool hasBounds(text.hasBounds());

    m_CharVertexBuffer.clear();
    m_CharIndexBuffer.clear();
    const he::gfx::Font* const font(text.getFont());
    const uint32 lineSpacing(font->getLineSpacing());
    const gui::Text::HAlignment h = text.getHorizontalAlignment();
    const he::ObjectList<std::string>& textLines(text.getText());
    const size_t textLineCount(textLines.size());
    const vec2 bounds(text.getBounds());
    const uint32 texWidth(tex2D->getWidth());
    const uint32 texHeight(tex2D->getHeight());
    size_t size(0);
    for (uint32 i(0); i < textLineCount; ++i)
        size += textLines[i].size();
    m_CharVertexBuffer.resize(size * 4);
    m_CharIndexBuffer.resize(size * 6);
    size_t charCounter(0);
    for (uint32 i(0); i < textLineCount; ++i)
    {
        const std::string& line(textLines[i]);
        linePos.y = pos.y + (lineSpacing * i);

        // If there is set a boundingbox for text
        if (hasBounds)
        {
            vec2 offset(0, 0);

            switch (h)
            {
                case gui::Text::HAlignment_Left:
                    break;
                case gui::Text::HAlignment_Center:
                    offset.x += (bounds.x / 2.0f - font->getStringWidth(line) / 2.0f);
                    break;
                case gui::Text::HAlignment_Right:
                    offset.x += (bounds.x - font->getStringWidth(line));
                    break;
            }

            gui::Text::VAlignment v = text.getVerticalAlignment();

            switch (v)
            {
                case gui::Text::VAlignment_Top:
                    break;
                case gui::Text::VAlignment_Center:
                    offset.y += bounds.y / 2.0f - (lineSpacing * textLineCount) / 2.0f;
                    break;
                case gui::Text::VAlignment_Bottom:
                    offset.y += bounds.y - (lineSpacing * textLineCount);
                    break;
            }

            linePos += offset;
        }

        vec2 glyphPos = linePos;

        const size_t charCount(line.size());
        for (size_t i2(0); i2 < charCount; ++i2)
        {
            const char character(line[i2]);

            const Font::CharData& charData = font->getCharTextureData(character);
            const RectF& regionToDraw = charData.textureRegion;

            const vec2 tcScale(regionToDraw.width / texWidth, regionToDraw.height / texHeight);
            const vec2 tcOffset(regionToDraw.x / texWidth, ((texHeight - regionToDraw.y) / texHeight) - tcScale.y);

            const vec2 size(regionToDraw.width, regionToDraw.height);

            VertexPosTex2D& vertex0(m_CharVertexBuffer[charCounter * 4 + 0]);
            VertexPosTex2D& vertex1(m_CharVertexBuffer[charCounter * 4 + 1]);
            VertexPosTex2D& vertex2(m_CharVertexBuffer[charCounter * 4 + 2]);
            VertexPosTex2D& vertex3(m_CharVertexBuffer[charCounter * 4 + 3]);

            vertex0.position = glyphPos + vec2(0, size.y);
            vertex0.textureCoord = tcOffset;
            vertex1.position = glyphPos + vec2(size.x, size.y);
            vertex1.textureCoord = tcOffset + vec2(tcScale.x, 0);
            vertex2.position = glyphPos;
            vertex2.textureCoord = tcOffset + vec2(0, tcScale.y);
            vertex3.position = glyphPos + vec2( size.x, 0);
            vertex3.textureCoord = tcOffset + tcScale;

            m_CharIndexBuffer[charCounter * 6 + 0] = charCounter * 4 + 0;
            m_CharIndexBuffer[charCounter * 6 + 1] = charCounter * 4 + 1;
            m_CharIndexBuffer[charCounter * 6 + 2] = charCounter * 4 + 2;

            m_CharIndexBuffer[charCounter * 6 + 3] = charCounter * 4 + 1;
            m_CharIndexBuffer[charCounter * 6 + 4] = charCounter * 4 + 3;
            m_CharIndexBuffer[charCounter * 6 + 5] = charCounter * 4 + 2;
            
            glyphPos.x += charData.advance.x;

            ++charCounter;
            if (i2 < charCount - 1)
            {
                glyphPos.x += font->getKerning(character, line[i2 + 1]);
            }

        }
    }
    HE_ASSERT(charCounter == size, "Incompatible charCounter and size");

    m_DynamicFontMesh->setVertices(&m_CharVertexBuffer[0], size * 4, MeshUsage_Stream, false);
    m_DynamicFontMesh->setIndices(&m_CharIndexBuffer[0], size * 6, IndexStride_UInt, MeshUsage_Stream);
    
    s_FontEffect->begin();
    s_FontEffect->setDiffuseMap(tex2D);
    s_FontEffect->setFontColor(m_Color);
    s_FontEffect->setDepth(0.5f);
    s_FontEffect->setWorldMatrix(m_OrthographicMatrix);
    GL::heBlendEnabled(true);
    GL::heBlendEquation(BlendEquation_Add);
    // reduce text quality loss by alpha reduction
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    
    GL::heBindFbo(m_CanvasBuffer->frameBufferId);
    GL::heBindVao(m_DynamicFontMesh->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_DynamicFontMesh->getNumIndices(), m_DynamicFontMesh->getIndexType(), BUFFER_OFFSET(0));
}
void Canvas2DRendererGL::drawImage( const Texture2D* tex2D, const vec2& pos,
                                    const vec2& newDimensions,
                                    const RectI regionToDraw)
{
    HE_ASSERT(m_CanvasBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

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
    
    s_TextureEffect->begin();
    s_TextureEffect->setWorldMatrix(m_OrthographicMatrix * world);
    s_TextureEffect->setDiffuseMap(tex2D);
    s_TextureEffect->setAlpha(1.0f);
    s_TextureEffect->setTCOffset(tcOffset);
    s_TextureEffect->setTCScale(tcScale);
    s_TextureEffect->setDepth(0.5f);

    GL::heBlendEnabled(true);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);

    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    
    GL::heBindFbo(m_CanvasBuffer->frameBufferId);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}
void Canvas2DRendererGL::drawSprite(const gui::Sprite* sprite, const vec2& pos,
                                    const vec2& size)
{
    HE_ASSERT(m_CanvasBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    vec2 s(sprite->getSize());
    RectF c(sprite->getCenter());

    if (size != vec2(0.0f,0.0f))
    {
        //vec2 scale(size.x / s.x, size.y / s.y);
        c.width = size.x - (s.x - c.width);
        c.height = size.y - (s.y - c.height);

        s = size;
    }

    mat44 world(mat44::createTranslation(vec3(pos.x + s.x/2, pos.y + s.y/2, 0.0f)) * mat44::createScale(s.x, s.y, 1.0f));
    
    s_NinePatchEffect->begin();
    s_NinePatchEffect->setWorldMatrix(m_OrthographicMatrix * world);
    s_NinePatchEffect->setCenter(c);
    s_NinePatchEffect->setSize(s);
    s_NinePatchEffect->setOriginalSize(sprite->getSize());
    s_NinePatchEffect->setDiffuseMap(sprite->getRenderTexture());
    s_NinePatchEffect->setDepth(0.5f);

    GL::heBlendEnabled(true);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);

    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    
    GL::heBindFbo(m_CanvasBuffer->frameBufferId);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

/* INTERNAL */
void Canvas2DRendererGL::init()
{
    HE_ASSERT(m_CanvasBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    m_Size = m_CanvasBuffer->size;

    if (s_ColorEffect == nullptr)
    {
        s_ColorEffect = NEW Simple2DEffect();
        s_ColorEffect->load();
    }
    if (s_TextureEffect == nullptr)
    {
        s_TextureEffect = NEW Simple2DTextureEffect();
        s_TextureEffect->load();
    }
    if (s_FontEffect == nullptr)
    {
        s_FontEffect = NEW Simple2DFontEffect();
        s_FontEffect->load();
    }
    if (s_NinePatchEffect == nullptr)
    {
        s_NinePatchEffect = NEW NinePatchEffect();
        s_NinePatchEffect->load();
    }

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_Size.x, 0.0f, m_Size.y, 0.0f, 1.0f);

    ResourceFactory<ModelMesh>* const modelMeshFactory(ResourceFactory<ModelMesh>::getInstance());
    if (s_TextureQuadHandle == ObjectHandle::unassigned)
    {
        s_TextureQuadHandle = modelMeshFactory->create();
        ModelMesh* const mesh(modelMeshFactory->get(s_TextureQuadHandle));

        BufferLayout vLayout;
        vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
        vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

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

        he::PrimitiveList<uint8> indices(6);
        indices.add(0); indices.add(1); indices.add(2);
        indices.add(1); indices.add(3); indices.add(2);

        mesh->init(vLayout, MeshDrawMode_Triangles);
        mesh->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
        mesh->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
        mesh->setLoaded();

        m_TextureQuad = mesh;
    }
    else
    {
        modelMeshFactory->instantiate(s_TextureQuadHandle);
        m_TextureQuad = modelMeshFactory->get(s_TextureQuadHandle);
    }

    m_DynamicFontMesh = modelMeshFactory->get(modelMeshFactory->create());

    BufferLayout vLayout;
    vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
    vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

    m_DynamicFontMesh->init(vLayout, MeshDrawMode_Triangles);
    m_DynamicFontMesh->setVertices(nullptr, 0, MeshUsage_Stream, false);
    m_DynamicFontMesh->setIndices(nullptr, 0, IndexStride_UInt, MeshUsage_Stream);
    m_DynamicFontMesh->setLoaded();
}

} } //end namespace
