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
#include "View.h"
#include "Window.h"
#include "Texture2D.h"
#include "Mesh2D.h"
#include "ModelMesh.h"
#include "Simple2DEffect.h"
#include "Simple2DFontEffect.h"
#include "Simple2DTextureEffect.h"
#include "Text.h"
#include "Font.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

/* STATIC */
Canvas2D::Data* Canvas2D::create(GLContext* context, const vec2& size)
{
    GRAPHICS->setActiveContext(context);
    Canvas2D::Data* pData = NEW Canvas2D::Data();

    pData->context = context;
    pData->renderTextureHnd = ResourceFactory<Texture2D>::getInstance()->create();
    Texture2D* pTexture = ResourceFactory<Texture2D>::getInstance()->get(pData->renderTextureHnd);

    pTexture->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Linear, gfx::Texture2D::TextureFormat_RGBA8, false);
    pTexture->setData((uint)size.x, (uint)size.y, 0, gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Byte, 0);   
    pTexture->setLoadFinished();

    // create final FBO & RB
    glGenRenderbuffers(1, &pData->resolvedRbufferID);
    glGenFramebuffers(1, &pData->resolvedFbufferID);
    GL::heBindFbo(pData->resolvedFbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, pData->resolvedRbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (uint)size.x, (uint)size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pData->resolvedRbufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->getID(), 0);
    he::err::checkFboStatus("Canvas2D: resolvedFbuffer");

    // create intermediate FBO & RB, MultiSampling
    glGenFramebuffers(1, &pData->fbufferID);
    GL::heBindFbo(pData->fbufferID);

    int samples = GL::getMaxMultiSamples();

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
        HE_ERROR("Failed to create FrameBuffer Canvas2D!");
        ResourceFactory<gfx::Texture2D>::getInstance()->release(pData->renderTextureHnd);
        delete pData;
        return nullptr;
    }

    return pData;
}
void Canvas2D::resizeData( Data* data, const vec2& size )
{
    Texture2D* texture(ResourceFactory<Texture2D>::getInstance()->get(data->renderTextureHnd));
    texture->setData((uint)size.x, (uint)size.y, 0, gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Byte, 0);   

    int samples = GL::getMaxMultiSamples();

    glBindRenderbuffer(GL_RENDERBUFFER, data->colorRbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)samples, GL_RGBA8, (GLsizei)size.x, (GLsizei)size.y);

    glBindRenderbuffer(GL_RENDERBUFFER, data->depthRbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)samples, GL_DEPTH_COMPONENT16, (GLsizei)size.x, (GLsizei)size.y);
}


/* CONSTRUCTOR - DESTRUCTOR */
#pragma warning(disable:4355) // this pointer in member initialize list
Canvas2D::Canvas2D(const RectI& absoluteViewport) :     
    m_pBufferData(nullptr),
    m_pBufferMesh(NEW Mesh2D()),
    m_pColorEffect(NEW Simple2DEffect()),
    m_pRenderTexture(ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd)),
    m_StackDepth(0),
    m_pTextureQuad(nullptr),
    m_pTextureEffect(NEW Simple2DTextureEffect()),
    m_GlobalAlpha(1.0f),
    m_pFontEffect(NEW Simple2DFontEffect()),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_PixelDepth(9999.99f),
    m_AutoClear(true),
    m_RelativeViewport(0, 0, 0, 0),
    m_Position(static_cast<float>(absoluteViewport.x), static_cast<float>(absoluteViewport.y)),
    m_CanvasSize(static_cast<float>(absoluteViewport.width), static_cast<float>(absoluteViewport.height)),
    m_View(nullptr)
{
    init();
}

Canvas2D::Canvas2D( View* view, const RectF& relativeViewport ) :     
    m_pBufferData(nullptr),
    m_pBufferMesh(NEW Mesh2D()),
    m_pColorEffect(NEW Simple2DEffect()),
    m_pRenderTexture(nullptr),
    m_StackDepth(0),
    m_pTextureQuad(nullptr),
    m_pTextureEffect(NEW Simple2DTextureEffect()),
    m_GlobalAlpha(1.0f),
    m_pFontEffect(NEW Simple2DFontEffect()),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_PixelDepth(9999.99f),
    m_AutoClear(true),
    m_RelativeViewport(relativeViewport),
    m_Position(view->getViewport().x * relativeViewport.x, view->getViewport().y * relativeViewport.y),
    m_CanvasSize(view->getViewport().width * relativeViewport.width, view->getViewport().height * relativeViewport.height),
    m_View(view),
    m_ViewResizedHandler(boost::bind(&Canvas2D::viewResized, this))
{
    m_View->ViewportSizeChanged += m_ViewResizedHandler;
    init();
}
#pragma warning(default:4355) 

Canvas2D::~Canvas2D()
{
    cleanup();

    delete m_pBufferData;
    delete m_pColorEffect;
    delete m_pTextureEffect;
    delete m_pBufferMesh;
    delete m_pFontEffect;

    m_View->ViewportSizeChanged -= m_ViewResizedHandler;

}

/* EXTRA */
void Canvas2D::init()
{
    m_pBufferData = Canvas2D::create(m_View->getWindow()->getContext(), m_CanvasSize);
    HE_ASSERT(m_pBufferData != nullptr, "Failed to create Canvas2D::data! - fatal");
    m_pRenderTexture = ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd);

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
    m_pTextureQuad->init(vLayout, gfx::MeshDrawMode_Triangles);

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

    m_pTextureQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
    m_pTextureQuad->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
    m_pTextureQuad->setLoaded();
}
void Canvas2D::resize( const vec2& newSize )
{
    if (m_CanvasSize != newSize)
    {
        resizeData(m_pBufferData, newSize);

        m_CanvasSize = newSize;
        m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 10000.0f);
    }
}

void Canvas2D::cleanup()
{
    GRAPHICS->setActiveContext(m_pBufferData->context);
    glDeleteFramebuffers(1, &m_pBufferData->fbufferID);
    glDeleteFramebuffers(1, &m_pBufferData->resolvedFbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->colorRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->depthRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->resolvedRbufferID);

    m_pRenderTexture->release();
    m_pTextBuffer->release();
    m_pTextureQuad->release();
}

float Canvas2D::getNewDepth()
{
    m_PixelDepth -= 0.01f;

    return m_PixelDepth;
}

/* GENERAL */
void Canvas2D::translate(const vec2& translation)
{
    m_TransformationStack[m_StackDepth] = mat33::createTranslation2D(translation) * m_TransformationStack[m_StackDepth];
}

void Canvas2D::rotate(float rotation)
{
    m_TransformationStack[m_StackDepth] = mat33::createRotation2D(rotation) * m_TransformationStack[m_StackDepth];
}

void Canvas2D::scale(const vec2& scale)
{
    m_TransformationStack[m_StackDepth] = mat33::createScale2D(scale) * m_TransformationStack[m_StackDepth];
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
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    GL::heBindFbo(m_pBufferData->fbufferID);
    GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL::heBindFbo(m_pBufferData->resolvedFbufferID);
    GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Canvas2D::draw2D(Renderer2D* renderer)
{
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    // blit MS FBO to normal FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pBufferData->fbufferID);
    GL::heBindFbo(m_pBufferData->resolvedFbufferID);

    glBlitFramebuffer(  0, 0, (GLint)m_CanvasSize.x, (GLint)m_CanvasSize.y,
                        0, 0, (GLint)m_CanvasSize.x, (GLint)m_CanvasSize.y,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    Texture2D* tex = ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd);
    renderer->drawTexture2DToScreen(tex, m_Position);

    if (m_AutoClear)
    {
        clear();
    }
}

void Canvas2D::strokeRect(const vec2& pos, const vec2& size)
{
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
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
    m_pColorEffect->setDepth(getNewDepth());

    GL::heBindVao(m_pBufferMesh->getBufferID());
    glDrawElements(GL_LINE_LOOP, (GLsizei)m_pBufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::fillRect(const vec2& pos, const vec2& size)
{
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
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
    m_pColorEffect->setDepth(getNewDepth());

    GL::heBindVao(m_pBufferMesh->getBufferID());
    glDrawElements(GL_TRIANGLES, (GLsizei)m_pBufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::fillText(const gui::Text& txt, const vec2& pos)
{
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    PROFILER_BEGIN("Canvas2D::fillText");

    vec2 linePos = pos;

    HE_ASSERT(txt.getFont()->isPreCached() == true, "Font needs to be precached!");

    m_pFontEffect->begin();
    Texture2D* tex2D = txt.getFont()->getTextureAtlas();
    m_pFontEffect->setDiffuseMap(tex2D);
    m_pFontEffect->setFontColor(m_FillColor);
    m_pFontEffect->setDepth(getNewDepth());

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
            tcOffset.y = ((tex2D->getHeight() - regionToDraw.y) / tex2D->getHeight()) - tcScale.y;

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

    PROFILER_END();
}

void Canvas2D::drawImage(	const Texture2D* tex2D, const vec2& pos,
                            const vec2& newDimensions,
                            const RectF& regionToDraw)
{
    HE_ASSERT(m_pBufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
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
    m_pTextureEffect->setDepth(getNewDepth());

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

void Canvas2D::viewResized()
{
    const RectI& viewport(m_View->getViewport());
    vec2 newSize(viewport.width * m_RelativeViewport.width, viewport.height * m_RelativeViewport.height);
    resize(newSize);
    m_Position.x = viewport.x * m_RelativeViewport.x;
    m_Position.y = viewport.y * m_RelativeViewport.y;
}


} }//end namespace