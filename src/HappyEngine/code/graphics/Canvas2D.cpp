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
#include "RenderTarget.h"
#include "MathFunctions.h"

namespace he {
namespace gfx {

/* STATIC */
Canvas2D::Data* Canvas2D::create(GLContext* context, const vec2& size)
{
    GRAPHICS->setActiveContext(context);
    Canvas2D::Data* data = NEW Canvas2D::Data();

    data->m_Context = context;
    data->m_RenderTextureHnd = ResourceFactory<Texture2D>::getInstance()->create();
    Texture2D* texture = ResourceFactory<Texture2D>::getInstance()->get(data->m_RenderTextureHnd);

    texture->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest, gfx::TextureFormat_RGBA8, false);
    texture->setData((uint32)size.x, (uint32)size.y, 0, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Byte, 0);   
    texture->setLoadFinished();
    
    // create intermediate FBO & RB, MultiSampling
    glGenFramebuffers(1, &data->m_FBufferID);
    GL::heBindFbo(data->m_FBufferID);
        
    // Color
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

    // Depth
    glGenRenderbuffers(1, &data->m_DepthRbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, data->m_DepthRbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)size.x, (GLsizei)size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, data->m_DepthRbufferID);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        HE_ERROR("Failed to create FrameBuffer Canvas2D!");
        ResourceFactory<gfx::Texture2D>::getInstance()->release(data->m_RenderTextureHnd);
        delete data;
        return nullptr;
    }

    return data;
}

void Canvas2D::resizeData(Data* data, const vec2& size)
{
    Texture2D* texture(ResourceFactory<Texture2D>::getInstance()->get(data->m_RenderTextureHnd));
    texture->setData((uint32)size.x, (uint32)size.y, 0, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Byte, 0);   
    
    glBindRenderbuffer(GL_RENDERBUFFER, data->m_DepthRbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)size.x, (GLsizei)size.y);
}

/* CONSTRUCTOR - DESTRUCTOR */
#pragma warning(disable:4355) // this pointer in member initialize list
Canvas2D::Canvas2D(const RectI& absoluteViewport) :     
    m_BufferData(nullptr),
    m_BufferMesh(NEW Mesh2D()),
    m_ColorEffect(NEW Simple2DEffect()),
    m_RenderTexture(nullptr),
    m_StackDepth(0),
    m_TextureQuad(nullptr),
    m_TextureEffect(NEW Simple2DTextureEffect()),
    m_GlobalAlpha(1.0f),
    m_FontEffect(NEW Simple2DFontEffect()),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_PixelDepth(4999.99f),
    m_AutoClear(true),
    m_RelativeViewport(0, 0, 0, 0),
    m_Position(static_cast<float>(absoluteViewport.x), static_cast<float>(absoluteViewport.y)),
    m_CanvasSize(static_cast<float>(absoluteViewport.width), static_cast<float>(absoluteViewport.height)),
    m_View(nullptr),
    m_ExtraPixelDepth(0),
    m_BlendStyle(BlendStyle_Alpha),
    m_Renderer2D(nullptr)
{
    init();
}

Canvas2D::Canvas2D( Renderer2D* parent, const RectF& relativeViewport ) :     
    m_BufferData(nullptr),
    m_BufferMesh(NEW Mesh2D()),
    m_ColorEffect(NEW Simple2DEffect()),
    m_RenderTexture(nullptr),
    m_StackDepth(0),
    m_TextureQuad(nullptr),
    m_TextureEffect(NEW Simple2DTextureEffect()),
    m_GlobalAlpha(1.0f),
    m_FontEffect(NEW Simple2DFontEffect()),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_PixelDepth(4999.99f),
    m_AutoClear(true),
    m_RelativeViewport(relativeViewport),
    m_Position(parent->getView()->getViewport().x * relativeViewport.x, parent->getView()->getViewport().y * relativeViewport.y),
    m_CanvasSize(parent->getView()->getViewport().width * relativeViewport.width, parent->getView()->getViewport().height * relativeViewport.height),
    m_View(parent->getView()),
    m_ViewResizedHandler(boost::bind(&Canvas2D::viewResized, this)),
    m_ExtraPixelDepth(0),
    m_BlendStyle(BlendStyle_Alpha),
    m_Renderer2D(parent)
{
    m_View->ViewportSizeChanged += m_ViewResizedHandler;
    init();
}
#pragma warning(default:4355) 

Canvas2D::~Canvas2D()
{
    cleanup();

    delete m_BufferData;
    delete m_ColorEffect;
    delete m_TextureEffect;
    delete m_BufferMesh;
    delete m_FontEffect;

    if (m_View != nullptr)
        m_View->ViewportSizeChanged -= m_ViewResizedHandler;
}

/* EXTRA */
void Canvas2D::init()
{
    m_BufferData = Canvas2D::create(m_View->getWindow()->getContext(), m_CanvasSize);
    HE_ASSERT(m_BufferData != nullptr, "Failed to create Canvas2D::data! - fatal");
    m_RenderTexture = ResourceFactory<Texture2D>::getInstance()->get(m_BufferData->m_RenderTextureHnd);

    m_ColorEffect->load();
    m_TextureEffect->load();
    m_FontEffect->load();

    ObjectHandle handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_TextBuffer = ResourceFactory<Texture2D>::getInstance()->get(handle);

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.01f, 10000.0f);
    m_TransformationStack.add(mat33::Identity);

    BufferLayout vLayout;
    vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
    vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

    // model texturequad
    m_TextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_TextureQuad->init(vLayout, gfx::MeshDrawMode_Triangles);

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

    m_TextureQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
    m_TextureQuad->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
    m_TextureQuad->setLoaded();
}
void Canvas2D::resize(const vec2& newSize)
{
    if (m_CanvasSize != newSize)
    {
        resizeData(m_BufferData, newSize);

        m_CanvasSize = newSize;
        m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 10000.0f);
    }
}

void Canvas2D::cleanup()
{
    GRAPHICS->setActiveContext(m_BufferData->m_Context);
    glDeleteFramebuffers(1, &m_BufferData->m_FBufferID);
    glDeleteRenderbuffers(1, &m_BufferData->m_DepthRbufferID);

    m_RenderTexture->release();
    m_TextBuffer->release();
    m_TextureQuad->release();
}

float Canvas2D::getNewDepth()
{
    m_PixelDepth -= 0.01f;

    return m_PixelDepth + m_ExtraPixelDepth;
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
    m_TransformationStack.add(mat33::Identity);
}

void Canvas2D::restore()
{
    if (m_StackDepth >= 0)
    {
        --m_StackDepth;
        m_TransformationStack.removeAt(m_TransformationStack.size() - 1);
    }
}

mat44 Canvas2D::getTransformation()
{
    if (m_StackDepth == 0)
        return m_TransformationStack[0].getMat44();

    mat33 result = mat33::Identity;

    for (uint16 i(0); i < m_StackDepth; ++i)
    {
        result = m_TransformationStack[i] * result;
    }

    return result.getMat44();
}

void Canvas2D::restoreDepth()
{
    m_ExtraPixelDepth = 0;
}

/* GETTERS */
Canvas2D::Data* Canvas2D::getData() const
{
    return m_BufferData;
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

void Canvas2D::setDepth(short depth)
{
    m_ExtraPixelDepth = clamp<short>(depth, -4000, 4000);
}

/* DRAW METHODS */
void Canvas2D::clear()
{
    HE_ASSERT(m_BufferData->m_Context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    GL::heBindFbo(m_BufferData->m_FBufferID);
    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    GL::heSetDepthWrite(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Canvas2D::draw()
{
    HE_ASSERT(m_BufferData->m_Context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");                 

    Texture2D* tex = ResourceFactory<Texture2D>::getInstance()->get(m_BufferData->m_RenderTextureHnd);
    m_Renderer2D->drawTexture2DToScreen(tex, m_Position);
}

void Canvas2D::strokeRect(const vec2& pos, const vec2& size)
{
    HE_ASSERT(m_BufferData->m_Context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    
    applyBlend();

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    
    m_BufferMesh->clear();
    m_BufferMesh->addVertex(pos);
    m_BufferMesh->addVertex(pos + vec2(size.x, 0));
    m_BufferMesh->addVertex(pos + size);
    m_BufferMesh->addVertex(pos + vec2(0, size.y));
    m_BufferMesh->createBuffer(true);

    m_ColorEffect->begin();
    m_ColorEffect->setColor(m_StrokeColor);
    m_ColorEffect->setWorldMatrix(m_OrthographicMatrix * getTransformation());
    m_ColorEffect->setDepth(getNewDepth());

    GL::heBindVao(m_BufferMesh->getBufferID());
    glDrawElements(GL_LINE_LOOP, (GLsizei)m_BufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::fillRect(const vec2& pos, const vec2& size)
{
    HE_ASSERT(m_BufferData->m_Context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    applyBlend();

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    
    m_BufferMesh->clear();
    m_BufferMesh->addVertex(pos);
    m_BufferMesh->addVertex(pos + vec2(size.x, 0));
    m_BufferMesh->addVertex(pos + size);
    m_BufferMesh->addVertex(pos + vec2(0, size.y));
    m_BufferMesh->createBuffer();
    
    m_ColorEffect->begin();
    m_ColorEffect->setColor(m_FillColor);
    m_ColorEffect->setWorldMatrix(m_OrthographicMatrix * getTransformation());
    m_ColorEffect->setDepth(getNewDepth());

    GL::heBindVao(m_BufferMesh->getBufferID());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Canvas2D::fillText(const he::gui::Text& /*txt*/, const vec2& /*pos*/)
{
    return;
    /*
    HE_ASSERT(m_BufferData->context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");
    PROFILER_BEGIN("Canvas2D::fillText");

    vec2 linePos = pos;

    HE_ASSERT(txt.getFont() != nullptr, "Text has no font set!");
    HE_ASSERT(txt.getFont()->isPreCached() == true, "Font needs to be precached!");

    Texture2D* tex2D = txt.getFont()->getTextureAtlas();

    m_FontEffect->begin();
    
    m_FontEffect->setDiffuseMap(tex2D);
    m_FontEffect->setFontColor(m_FillColor);

    float dp(getNewDepth());

    m_FontEffect->setDepth(dp);

    setBlendStyle(BlendStyle_Alpha);
    applyBlend();

    GL::heBlendFuncSeperate(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha, BlendFunc_One, BlendFunc_One);

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(false);

    GL::heBindFbo(m_BufferData->fbufferID);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());

    bool hasBounds(txt.hasBounds());

    for (uint32 i(0); i < txt.getText().size(); ++i)
    {
        linePos.y = pos.y + (txt.getFont()->getLineSpacing() * i);

        // If there is set a boundingbox for text
        if (hasBounds)
        {
            vec2 offset(0, 0);

            gui::Text::HAlignment h = txt.getHorizontalAlignment();

            switch (h)
            {
                case gui::Text::HAlignment_Left:
                    break;
                case gui::Text::HAlignment_Center:
                    offset.x += (txt.getBounds().x / 2.0f - txt.getFont()->getStringWidth(txt.getText()[i]) / 2.0f);
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
                    offset.y += (txt.getBounds().y / 2.0f - (txt.getFont()->getLineSpacing() * txt.getText().size()) / 2.0f);
                    break;
                case gui::Text::VAlignment_Bottom:
                    offset.y += (txt.getBounds().y - (txt.getFont()->getLineSpacing() * txt.getText().size()));
                    break;
            }

            linePos += offset;
        }

        vec2 glyphPos = linePos;

        for (uint32 i2(0); i2 < txt.getText()[i].size(); ++i2)
        {
            vec2 tcOffset(0.0f,0.0f);
            vec2 tcScale(1.0f,1.0f);
            vec2 size;

            const Font::CharData* cData = txt.getFont()->getCharTextureData(txt.getText()[i][i2]);
            // skip char if chardata is empty
            if (cData == nullptr)
                continue;

            const RectF& regionToDraw = cData->textureRegion;

            tcScale.x = regionToDraw.width / tex2D->getWidth();
            tcScale.y = regionToDraw.height / tex2D->getHeight();

            tcOffset.x = regionToDraw.x / tex2D->getWidth();
            tcOffset.y = ((tex2D->getHeight() - regionToDraw.y) / tex2D->getHeight()) - tcScale.y;

            size = vec2(regionToDraw.width, regionToDraw.height);

            if (i2 > 0)
            {
               glyphPos.x += txt.getFont()->getKerning(txt.getText()[i][i2 - 1], txt.getText()[i][i2]);
            }

            mat44 world(mat44::createTranslation(vec3(glyphPos.x + (size.x/2), glyphPos.y + (size.y/2), 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));

            glyphPos.x += cData->advance.x;
              
            m_FontEffect->setWorldMatrix(m_OrthographicMatrix * world);
            m_FontEffect->setTCOffset(tcOffset);
            m_FontEffect->setTCScale(tcScale);

            glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
        }
    }

    PROFILER_END();*/
}

void Canvas2D::drawImage(	const Texture2D* tex2D, const vec2& pos,
                            const vec2& newDimensions,
                            const RectF& regionToDraw)
{
    HE_ASSERT(m_BufferData->m_Context == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

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
    
    m_TextureEffect->begin();
    m_TextureEffect->setWorldMatrix(m_OrthographicMatrix * world);
    m_TextureEffect->setDiffuseMap(tex2D);
    m_TextureEffect->setAlpha(m_GlobalAlpha);
    m_TextureEffect->setTCOffset(tcOffset);
    m_TextureEffect->setTCScale(tcScale);
    m_TextureEffect->setDepth(getNewDepth());

    m_BlendStyle = BlendStyle_Alpha;
    applyBlend();

    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    GL::heBindFbo(m_BufferData->m_FBufferID);
    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

void Canvas2D::viewResized()
{
    const RectI& viewport(m_View->getViewport());
    vec2 newSize(viewport.width * m_RelativeViewport.width, viewport.height * m_RelativeViewport.height);
    resize(newSize);
    m_Position.x = viewport.x * m_RelativeViewport.x;
    m_Position.y = viewport.y * m_RelativeViewport.y;
}

void Canvas2D::applyBlend()
{
    switch (m_BlendStyle)
    {
    case BlendStyle_Opac:
        GL::heBlendEnabled(false);
        break;
    case BlendStyle_Alpha:
        GL::heBlendEnabled(true);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        break;
    case BlendStyle_Add:
        GL::heBlendEnabled(true);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendFunc(BlendFunc_One, BlendFunc_One);
        break;
    }
}

} }//end namespace
