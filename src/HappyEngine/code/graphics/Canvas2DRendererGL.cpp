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
#include "ModelMesh.h"
#include "Vertex.h"
#include "Text.h"
#include "Font.h"

namespace he {
namespace gfx {

/* STATIC */
Simple2DEffect* Canvas2DRendererGL::s_ColorEffect = nullptr;
Simple2DTextureEffect* Canvas2DRendererGL::s_TextureEffect = nullptr;
Simple2DFontEffect* Canvas2DRendererGL::s_FontEffect = nullptr;
ObjectHandle Canvas2DRendererGL::s_TextureQuadHandle = ObjectHandle::unassigned;
uint16 Canvas2DRendererGL::s_Renderers = 0;

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2DRendererGL::Canvas2DRendererGL(Canvas2DBuffer* canvasBuffer, GLContext* glContext) :
    m_CanvasBuffer(canvasBuffer),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_SurfaceDirty(true),
    m_Size(vec2(0,0)),
    m_RenderBuffer(NEW Canvas2DBuffer()),
    m_Context(glContext)
{
    ++s_Renderers;
}

Canvas2DRendererGL::~Canvas2DRendererGL()
{
    --s_Renderers;

    m_TextureQuad->release();

    if (s_Renderers == 0)
    {
        delete s_TextureEffect;
        delete s_ColorEffect;
        delete s_FontEffect;
    }

    delete m_RenderBuffer;
}

/* GENERAL */

/* GETTERS */
const Texture2D* Canvas2DRendererGL::getRenderTexture() const
{
    return ResourceFactory<Texture2D>::getInstance()->get(m_RenderBuffer->renderTextureHandle);
}

bool Canvas2DRendererGL::isSurfaceDirty() const
{
    return m_SurfaceDirty;
}

/* SETTERS */
void Canvas2DRendererGL::setFillColor(const Color& col)
{
    if (m_FillColor != col)
    {
        m_FillColor = col;
    }
}

/* DRAW */
void Canvas2DRendererGL::clear()
{
    m_RenderBuffer->clear();
}
void Canvas2DRendererGL::fillText(const gui::Text& text, const vec2& pos)
{
    HE_ASSERT(m_RenderBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    HE_ASSERT(text.getFont() != nullptr, "Text has no font set!");
    HE_ASSERT(text.getFont()->isPreCached() == true, "Font needs to be precached!");

    Texture2D* tex2D = text.getFont()->getTextureAtlas();

    s_FontEffect->begin();
    
    s_FontEffect->setDiffuseMap(tex2D);
    s_FontEffect->setFontColor(m_FillColor);
    s_FontEffect->setDepth(0.5f);

    GL::heBlendEnabled(true);
    GL::heBlendEquation(BlendEquation_Add);
    // reduce text quality loss by alpha reduction
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    GL::heBindFbo(m_RenderBuffer->frameBufferId);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());

    vec2 linePos = pos;
    bool hasBounds(text.hasBounds());

    for (uint32 i(0); i < text.getText().size(); ++i)
    {
        linePos.y = pos.y + (text.getFont()->getLineSpacing() * i);

        // If there is set a boundingbox for text
        if (hasBounds)
        {
            vec2 offset(0, 0);

            gui::Text::HAlignment h = text.getHorizontalAlignment();

            switch (h)
            {
                case gui::Text::HAlignment_Left:
                    break;
                case gui::Text::HAlignment_Center:
                    offset.x += (text.getBounds().x / 2.0f - text.getFont()->getStringWidth(text.getText()[i]) / 2.0f);
                    break;
                case gui::Text::HAlignment_Right:
                    offset.x += (text.getBounds().x - text.getFont()->getStringWidth(text.getText()[i]));
                    break;
            }

            gui::Text::VAlignment v = text.getVerticalAlignment();

            switch (v)
            {
                case gui::Text::VAlignment_Top:
                    break;
                case gui::Text::VAlignment_Center:
                    offset.y += (text.getBounds().y / 2.0f - (text.getFont()->getLineSpacing() * text.getText().size()) / 2.0f);
                    break;
                case gui::Text::VAlignment_Bottom:
                    offset.y += (text.getBounds().y - (text.getFont()->getLineSpacing() * text.getText().size()));
                    break;
            }

            linePos += offset;
        }

        vec2 glyphPos = linePos;

        for (uint32 i2(0); i2 < text.getText()[i].size(); ++i2)
        {
            vec2 tcOffset(0.0f,0.0f);
            vec2 tcScale(1.0f,1.0f);
            vec2 size;

            const Font::CharData* cData = text.getFont()->getCharTextureData(text.getText()[i][i2]);
            // skip char if chardata is empty
            if (cData == nullptr)
                continue;

            const RectF& regionToDraw = cData->textureRegion;

            tcScale.x = regionToDraw.width / tex2D->getWidth();
            tcScale.y = regionToDraw.height / tex2D->getHeight();

            tcOffset.x = regionToDraw.x / tex2D->getWidth();
            tcOffset.y = ((tex2D->getHeight() - regionToDraw.y) / tex2D->getHeight()) - tcScale.y;

            size = vec2(regionToDraw.width, regionToDraw.height);

            mat44 world(mat44::createTranslation(vec3(glyphPos.x + size.x/2, glyphPos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

            glyphPos.x += cData->advance.x;

            if (i2 < text.getText()[i].size() - 1)
            {
                glyphPos.x += text.getFont()->getKerning(text.getText()[i][i2], text.getText()[i][i2 + 1]);
            }
              
            s_FontEffect->setWorldMatrix(m_OrthographicMatrix * world);
            s_FontEffect->setTCOffset(tcOffset);
            s_FontEffect->setTCScale(tcScale);

            glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
        }
    }
}
void Canvas2DRendererGL::drawImage( const Texture2D* tex2D, const vec2& pos,
                                    const vec2& newDimensions,
                                    const RectI regionToDraw)
{
    HE_ASSERT(m_RenderBuffer->glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

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
    
    GL::heBindFbo(m_RenderBuffer->frameBufferId);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

/* INTERNAL */
void Canvas2DRendererGL::init()
{
    m_Size = m_CanvasBuffer->size;

    m_RenderBuffer->init(m_Context, m_Size);

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

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_Size.x, 0.0f, m_Size.y, 0.0f, 1.0f);

    if (s_TextureQuadHandle == ObjectHandle::unassigned)
    {
        s_TextureQuadHandle = ResourceFactory<ModelMesh>::getInstance()->create();
        ModelMesh* mesh(ResourceFactory<ModelMesh>::getInstance()->get(s_TextureQuadHandle));

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

        mesh->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
        mesh->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
        mesh->setLoaded();

        ResourceFactory<ModelMesh>::getInstance()->instantiate(s_TextureQuadHandle);
        m_TextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(s_TextureQuadHandle);
        mesh->release();
    }
    else
    {
        ResourceFactory<ModelMesh>::getInstance()->instantiate(s_TextureQuadHandle);
        m_TextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(s_TextureQuadHandle);
    }
}

} } //end namespace