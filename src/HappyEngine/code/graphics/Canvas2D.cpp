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

#include "HappyPCH.h" 

#include "Canvas2D.h"
#include "GraphicsEngine.h"
#include "Renderer2D.h"
#include "Vertex.h"

namespace he {
namespace gfx {

/* STATIC */
Canvas2D::Data* Canvas2D::create(const vec2& size)
{
    Canvas2D::Data* pData = NEW Canvas2D::Data();

    pData->renderTextureHnd = ResourceFactory<Texture2D>::getInstance()->create();
    Texture2D* pTexture = ResourceFactory<Texture2D>::getInstance()->get(pData->renderTextureHnd);

    pTexture->setData((uint)size.x, (uint)size.y, 
        gfx::Texture2D::TextureFormat_RGBA8, 0, 
        gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Byte,
        gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Linear, false, false);
    
    // create final FBO & RB
    glGenFramebuffers(1, &pData->resolvedFbufferID);
    GL::heBindFbo(pData->resolvedFbufferID);
    glGenRenderbuffers(1, &pData->resolvedRbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, pData->resolvedRbufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->getID(), 0);

    // create intermediate FBO & RB, MultiSampling
    glGenFramebuffers(1, &pData->fbufferID);
    GL::heBindFbo(pData->fbufferID);

    int samples;
    glGetIntegerv(GL_MAX_SAMPLES, &samples);

    glGenRenderbuffers(1, &pData->colorRbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, pData->colorRbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)samples, GL_RGBA8, (GLsizei)size.x, (GLsizei)size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, pData->colorRbufferID);

    glGenRenderbuffers(1, &pData->depthRbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, pData->depthRbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)samples, GL_DEPTH_COMPONENT16, (GLsizei)size.x, (GLsizei)size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pData->depthRbufferID);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        he::HE_ERROR("Failed to create FrameBuffer Canvas2D!");

        delete pData;
        return nullptr;
    }

    return pData;
}

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2D::Canvas2D(Data* pData, const vec2& size) :     m_pBufferData(pData),
                                                        m_pBufferMesh(NEW Mesh2D()),
                                                        m_pColorEffect(NEW Simple2DEffect()),
                                                        m_pRenderTexture(ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd)),
                                                        m_StackDepth(0),
                                                        m_CanvasSize(size),
                                                        m_pTextureQuad(nullptr),
                                                        m_pTextureEffect(NEW Simple2DTextureEffect()),
                                                        m_GlobalAlpha(1.0f),
                                                        m_pFontEffect(NEW Simple2DFontEffect()),
                                                        m_FillColor(Color(1.0f,1.0f,1.0f)),
                                                        m_StrokeColor(Color(1.0f,1.0f,1.0f)),
                                                        m_PixelDepth(9999.99f),
                                                        m_AutoClear(true)
{
    init();

    if (m_CanvasSize.x == (float)GRAPHICS->getScreenWidth() &&
        m_CanvasSize.y == (float)GRAPHICS->getScreenHeight())
    {
        m_FullScreen = true;
    }
    else
    {
        m_FullScreen = false;
    }
}

Canvas2D::~Canvas2D()
{
    delete m_pBufferData;
    delete m_pColorEffect;
    delete m_pTextureEffect;
    delete m_pBufferMesh;
    delete m_pFontEffect;

    cleanup();
}

/* EXTRA */
void Canvas2D::init()
{
    m_pColorEffect->load();
    m_pTextureEffect->load();
    m_pFontEffect->load();

    ObjectHandle handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_pTextBuffer = ResourceFactory<Texture2D>::getInstance()->get(handle);

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 10000.0f);
    m_TransformationStack.push_back(mat33::Identity);

    BufferLayout vLayout;
    vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
    vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

    // model texturequad
    m_pTextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
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

    m_pTextureQuad->setVertices(&vertices[0], 4, vLayout);
    m_pTextureQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

void Canvas2D::cleanup()
{
    glDeleteFramebuffers(1, &m_pBufferData->fbufferID);
    glDeleteFramebuffers(1, &m_pBufferData->resolvedFbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->colorRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->depthRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->resolvedRbufferID);

    m_pRenderTexture->release();
    m_pTextBuffer->release();
    m_pTextureQuad->release();
}

float Canvas2D::getDepth()
{
    m_PixelDepth -= 0.01f;

    return m_PixelDepth;
}

/* GENERAL */
void Canvas2D::translate(const vec2& translation)
{
    m_TransformationStack[m_StackDepth] = mat33::createTranslaton(translation) * m_TransformationStack[m_StackDepth];
}

void Canvas2D::rotate(float rotation)
{
    m_TransformationStack[m_StackDepth] = mat33::createRotation(rotation) * m_TransformationStack[m_StackDepth];
}

void Canvas2D::scale(const vec2& scale)
{
    m_TransformationStack[m_StackDepth] = mat33::createScale(scale) * m_TransformationStack[m_StackDepth];
}

void Canvas2D::save()
{
    if (m_StackDepth >= STACK_DEPTH_LIMIT)
    {
        HE_ERROR("Canvas2D: Transformation stack depth limit (16) reached");
    }

    ++m_StackDepth;
    m_TransformationStack.push_back(mat33::Identity);
}

void Canvas2D::restore()
{
    if (m_StackDepth >= 0)
    {
        --m_StackDepth;
        m_TransformationStack.pop_back();
    }
}

mat44 Canvas2D::getTransformation()
{
    if (m_StackDepth == 0)
        return m_TransformationStack[0].getMat44();

    mat33 result = mat33::Identity;

    for (ushort i(0); i < m_StackDepth; ++i)
    {
        result = m_TransformationStack[i] * result;
    }

    return result.getMat44();
}

/* GETTERS */
Canvas2D::Data* Canvas2D::getData() const
{
    return m_pBufferData;
}

/* SETTERS */
void Canvas2D::setStrokeColor(const Color& newColor)
{
    m_StrokeColor = newColor;
}

void Canvas2D::setFillColor(const Color& newColor)
{
    m_FillColor = newColor;
}

void Canvas2D::setLineWidth(float width)
{
    m_LineWidth = width;
}

void Canvas2D::setGlobalAlpha(float alpha)
{
    m_GlobalAlpha = alpha;
}

void Canvas2D::setAutoClearing(bool clearAfterDraw)
{
    m_AutoClear = clearAfterDraw;
}

/* DRAW METHODS */
void Canvas2D::clear()
{
    GL::heBindFbo(m_pBufferData->fbufferID);
    GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL::heBindFbo(m_pBufferData->resolvedFbufferID);
    GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Canvas2D::draw(const vec2& pos)
{
    // blit MS FBO to normal FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pBufferData->fbufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pBufferData->resolvedFbufferID);

    glBlitFramebuffer(  0, 0, (GLint)m_CanvasSize.x, (GLint)m_CanvasSize.y,
                        0, 0, (GLint)m_CanvasSize.x, (GLint)m_CanvasSize.y,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    Texture2D* tex = ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd);
    GUI->drawTexture2DToScreen(tex, pos);

    if (m_AutoClear)
    {
        clear();
    }

    if (m_FullScreen)
    {
        vec2 dim((float)GRAPHICS->getScreenWidth(), (float)GRAPHICS->getScreenHeight());

        if (m_CanvasSize != dim)
        {
            cleanup();

            m_pBufferData = Canvas2D::create(dim);
            m_pRenderTexture = ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd);

            m_CanvasSize = dim;
            m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 10000.0f);
        }
    }
}

void Canvas2D::strokeRect(const vec2& pos, const vec2& size)
{
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendEnabled(true);

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);

    GL::heBindFbo(m_pBufferData->fbufferID);

    m_pBufferMesh->clear();
    m_pBufferMesh->addVertex(pos);
    m_pBufferMesh->addVertex(pos + vec2(size.x, 0));
    m_pBufferMesh->addVertex(pos + size);
    m_pBufferMesh->addVertex(pos + vec2(0, size.y));
    m_pBufferMesh->createBuffer(true);

    m_pColorEffect->begin();
    m_pColorEffect->setColor(m_StrokeColor);
    m_pColorEffect->setWorldMatrix(m_OrthographicMatrix * getTransformation());
    m_pColorEffect->setDepth(getDepth());

    GL::heBindVao(m_pBufferMesh->getBufferID());
    glDrawElements(GL_LINE_LOOP, (GLsizei)m_pBufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::fillRect(const vec2& pos, const vec2& size)
{
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendEnabled(true);

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);

    GL::heBindFbo(m_pBufferData->fbufferID);

    m_pBufferMesh->clear();
    m_pBufferMesh->addVertex(pos);
    m_pBufferMesh->addVertex(pos + vec2(size.x, 0));
    m_pBufferMesh->addVertex(pos + size);
    m_pBufferMesh->addVertex(pos + vec2(0, size.y));
    m_pBufferMesh->createBuffer();

    m_pColorEffect->begin();
    m_pColorEffect->setColor(m_FillColor);
    m_pColorEffect->setWorldMatrix(m_OrthographicMatrix * getTransformation());
    m_pColorEffect->setDepth(getDepth());

    GL::heBindVao(m_pBufferMesh->getBufferID());
    glDrawElements(GL_TRIANGLES, (GLsizei)m_pBufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::strokeArc(const vec2& /*pos*/, float /*radius*/, float /*startAngle*/, float /*endAngle*/, bool /*antiClockwise*/)
{

}

void Canvas2D::fillArc(const vec2& /*pos*/, float /*radius*/, float /*startAngle*/, float /*endAngle*/, bool /*antiClockwise*/)
{

}

void Canvas2D::beginPath()
{

}

void Canvas2D::closePath()
{

}

void Canvas2D::moveTo(const vec2& /*pos*/)
{

}

void Canvas2D::lineTo(const vec2& /*pos*/)
{

}

void Canvas2D::arcTo(float /*radius*/, float /*startAngle*/, float /*endAngle*/, bool /*antiClockwise*/)
{

}

void Canvas2D::quadraticCurveTo(const vec2& /*cp*/, const vec2& /*pos*/)
{

}

void Canvas2D::bezierCurveTo(const vec2& /*cp1*/, const vec2& /*cp2*/, const vec2& /*pos*/)
{

}

void Canvas2D::fill()
{

}

void Canvas2D::stroke()
{

}

void Canvas2D::fillText(const gui::Text& txt, const vec2& pos)
{
    PROFILER_BEGIN("Canvas2D::fillText");

    vec2 linePos = pos;

    if (txt.getFont()->isPreCached() == true)
    {
        m_pFontEffect->begin();
        Texture2D* tex2D = txt.getFont()->getTextureAtlas();
        m_pFontEffect->setDiffuseMap(tex2D);
        m_pFontEffect->setFontColor(m_FillColor);
        m_pFontEffect->setDepth(getDepth());

        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendEnabled(true);

        GL::heSetDepthFunc(DepthFunc_LessOrEqual);
        GL::heSetDepthRead(true);
        GL::heSetDepthWrite(true);

        GL::heBindFbo(m_pBufferData->fbufferID);
        GL::heBindVao(m_pTextureQuad->getVertexArraysID());

        bool hasBounds(txt.hasBounds());

        for (uint i(0); i < txt.getText().size(); ++i)
        {
            linePos.y = pos.y + (txt.getFont()->getLineSpacing() * i);

            // If there is set a boundingbox for text
            if (hasBounds)
            {
                vec2 offset;

                gui::Text::HAlignment h = txt.getHorizontalAlignment();

                switch (h)
                {
                    case gui::Text::HAlignment_Left:
                        break;
                    case gui::Text::HAlignment_Center:
                        offset.x += (txt.getFont()->getStringWidth(txt.getText()[i]) / 2);
                        break;
                    case gui::Text::HAlignment_Right:
                        offset.x += (txt.getBounds().x - txt.getFont()->getStringWidth(txt.getText()[i]));
                        break;
                }

                gui::Text::VAlignment v = txt.getVerticalAlignment();

                switch (v)
                {
                    case gui::Text::VAlignment_Top:
                        break;
                    case gui::Text::VAlignment_Center:
                        offset.y += ((txt.getFont()->getLineSpacing() * (txt.getText().size() - 1)) / 2);
                        break;
                    case gui::Text::VAlignment_Bottom:
                        offset.y += (txt.getBounds().y - (txt.getFont()->getLineSpacing() * txt.getText().size()));
                        break;
                }

                linePos += offset;
            }

            vec2 glyphPos = linePos;

            for (uint i2(0); i2 < txt.getText()[i].size(); ++i2)
            {
                vec2 tcOffset(0.0f,0.0f);
                vec2 tcScale(1.0f,1.0f);
                vec2 size;

                const Font::CharData* cData = txt.getFont()->getCharTextureData(txt.getText()[i][i2]);
                const RectF& regionToDraw = cData->textureRegion;

                tcScale.x = regionToDraw.width / tex2D->getWidth();
                tcScale.y = regionToDraw.height / tex2D->getHeight();

                tcOffset.x = regionToDraw.x / tex2D->getWidth();
                tcOffset.y = 1 - ((tex2D->getHeight() - regionToDraw.y) / tex2D->getHeight()) - tcScale.y;

                size = vec2(regionToDraw.width, regionToDraw.height);

                mat44 world(mat44::createTranslation(vec3(glyphPos.x + size.x/2, glyphPos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

                glyphPos.x += cData->advance.x;

                if (i2 < txt.getText()[i].size() - 1)
                {
                    glyphPos.x += txt.getFont()->getKerning(txt.getText()[i][i2], txt.getText()[i][i2 + 1]);
                }

                //save();

                ////translate(vec2(linePos.x + (size.x/2), linePos.y + (size.y/2)));
                ////scale(vec2(size.x / 2, size.y / 2));
  
                m_pFontEffect->setWorldMatrix(m_OrthographicMatrix * world);
                m_pFontEffect->setTCOffset(tcOffset);
                m_pFontEffect->setTCScale(tcScale);

                glDrawElements(GL_TRIANGLES, m_pTextureQuad->getNumIndices(), m_pTextureQuad->getIndexType(), 0);

                //restore();
            }
        }
    }
    else
    {
        m_pFontEffect->begin();
        m_pFontEffect->setFontColor(m_FillColor);
        m_pFontEffect->setDepth(getDepth());

        GL::heBlendFunc(BlendFunc_One, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendEnabled(true);

        GL::heSetDepthFunc(DepthFunc_LessOrEqual);
        GL::heSetDepthRead(true);
        GL::heSetDepthWrite(true);

        GL::heBindFbo(m_pBufferData->fbufferID);
        GL::heBindVao(m_pTextureQuad->getVertexArraysID());

        m_pFontEffect->setTCOffset(vec2(0,0));
        m_pFontEffect->setTCScale(vec2(1.0f,1.0f));

        for (uint i(0); i < txt.getText().size(); ++i)
        {
            txt.getFont()->renderText(txt.getText()[i], m_pTextBuffer);            

            vec2 size;
            size = vec2(static_cast<float>(m_pTextBuffer->getWidth()), static_cast<float>(m_pTextBuffer->getHeight()));

            mat44 world(mat44::createTranslation(vec3(linePos.x + size.x/2, linePos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

            save();

            //translate(vec2(linePos.x + (size.x/2), linePos.y + (size.y/2)));
            //scale(vec2(size.x / 2, size.y / 2));

            m_pFontEffect->setWorldMatrix(m_OrthographicMatrix * world);
            m_pFontEffect->setDiffuseMap(m_pTextBuffer);

            glDrawElements(GL_TRIANGLES, m_pTextureQuad->getNumIndices(), m_pTextureQuad->getIndexType(), 0);

            restore();

            linePos.y += txt.getFont()->getLineSpacing();
        }
    }

    PROFILER_END();
}

void Canvas2D::drawImage(	const Texture2D* tex2D, const vec2& pos,
				            const vec2& newDimensions,
				            const RectF& regionToDraw)
{
    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (regionToDraw != RectF(vec2(0.0f,0.0f), vec2(0.0f,0.0f)))
    {
        tcScale.x = regionToDraw.width / tex2D->getWidth();
        tcScale.y = regionToDraw.height / tex2D->getHeight();

        tcOffset.x = regionToDraw.x / tex2D->getWidth();
        tcOffset.y = 1 - (regionToDraw.y / tex2D->getHeight()) - tcScale.y;
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

    save();

    //scale(vec2(size.x, size.y));
    //translate(vec2(pos.x + size.x/2, pos.y + size.y/2));

    m_pTextureEffect->begin();
    m_pTextureEffect->setWorldMatrix(m_OrthographicMatrix * world);
    m_pTextureEffect->setDiffuseMap(tex2D);
    m_pTextureEffect->setAlpha(m_GlobalAlpha);
    m_pTextureEffect->setTCOffset(tcOffset);
    m_pTextureEffect->setTCScale(tcScale);
    m_pTextureEffect->setDepth(getDepth());

    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendEnabled(true);

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);

    GL::heBindFbo(m_pBufferData->fbufferID);

    GL::heBindVao(m_pTextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pTextureQuad->getNumIndices(), m_pTextureQuad->getIndexType(), 0);

    restore();
}

} }//end namespace