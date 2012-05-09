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
#include "Happy2DRenderer.h"

namespace he {
namespace gfx {

/* STATIC */
Canvas2D::Data* Canvas2D::create(const vec2& size)
{
    Canvas2D::Data* pData = NEW Canvas2D::Data();

    pData->renderTextureHnd = ResourceFactory<Texture2D>::getInstance()->create();
    Texture2D* pTexture = ResourceFactory<Texture2D>::getInstance()->get(pData->renderTextureHnd);

    pTexture->setData(size.x, size.y, 
        gfx::Texture2D::TextureFormat_RGBA8, 0, 
        gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Byte,
        gfx::Texture2D::WrapType_Repeat,  gfx::Texture2D::FilterType_Linear, false, false);
    
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
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, pData->colorRbufferID);

    glGenRenderbuffers(1, &pData->depthRbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, pData->depthRbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, size.x, size.y);
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
                                                        m_CanvasSize(size)
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
    delete m_pBufferMesh;

    cleanup();
}

/* EXTRA */
void Canvas2D::init()
{
    m_pColorEffect->load();

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 100.0f);
    m_TransformationStack.push_back(mat33::Identity);
}

void Canvas2D::cleanup()
{
    glDeleteFramebuffers(1, &m_pBufferData->fbufferID);
    glDeleteFramebuffers(1, &m_pBufferData->resolvedFbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->colorRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->depthRbufferID);
    glDeleteRenderbuffers(1, &m_pBufferData->resolvedRbufferID);

    m_pRenderTexture->release();
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

/* DRAW METHODS */
void Canvas2D::draw(const vec2& pos)
{
    // blit MS FBO to normal FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pBufferData->fbufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pBufferData->resolvedFbufferID);

    glBlitFramebuffer(  0, 0, m_CanvasSize.x, m_CanvasSize.y,
                        0, 0, m_CanvasSize.x, m_CanvasSize.y,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    GUI->drawTexture2D(ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd), pos);

    if (m_FullScreen)
    {
        vec2 dim((float)GRAPHICS->getScreenWidth(), (float)GRAPHICS->getScreenHeight());

        if (m_CanvasSize != dim)
        {
            cleanup();

            m_pBufferData = Canvas2D::create(dim);
            m_pRenderTexture = ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd);

            m_CanvasSize = dim;
            m_OrthographicMatrix = mat44::createOrthoLH(0.0f, m_CanvasSize.x, 0.0f, m_CanvasSize.y, 0.0f, 100.0f);
        }
    }
}

void Canvas2D::strokeRect(const vec2& pos, const vec2& size)
{

}

void Canvas2D::fillRect(const vec2& pos, const vec2& size)
{
    /************************************************************************/
    /* TEMP TEST STUFF                                                      */
    /************************************************************************/

    glEnable(GL_MULTISAMPLE);
    GL::heBindFbo(m_pBufferData->fbufferID);

    m_pBufferMesh->clear();
    m_pBufferMesh->addVertex(pos);
    m_pBufferMesh->addVertex(pos + vec2(size.x, size.y / 20));
    m_pBufferMesh->addVertex(pos + size - vec2(20,50));
    m_pBufferMesh->addVertex(pos + vec2(0, size.y));
    m_pBufferMesh->createBuffer();

    m_pColorEffect->begin();
    m_pColorEffect->setColor(m_FillColor);
    m_pColorEffect->setWorldMatrix(m_OrthographicMatrix * getTransformation());
    //m_pColorEffect->setDepth(0.5f);

    GL::heBindVao(m_pBufferMesh->getBufferID());
    glDrawElements(GL_TRIANGLES, m_pBufferMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void Canvas2D::strokeArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void Canvas2D::fillArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void Canvas2D::beginPath()
{

}

void Canvas2D::closePath()
{

}

void Canvas2D::moveTo(const vec2& pos)
{

}

void Canvas2D::lineTo(const vec2& pos)
{

}

void Canvas2D::arcTo(float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void Canvas2D::quadraticCurveTo(const vec2& cp, const vec2& pos)
{

}

void Canvas2D::bezierCurveTo(const vec2& cp1, const vec2& cp2, const vec2& pos)
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

}

void Canvas2D::drawImage(	const Texture2D* tex2D, const vec2& pos,
				            const vec2& newDimensions,
				            const RectF& regionToDraw)
{

}

} }//end namespace