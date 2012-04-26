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

Canvas2D::Canvas2D(Data* pData) :   m_pBufferData(pData),
                                    m_pBufferMesh(NEW Mesh2D()),
                                    m_pColorEffect(NEW Simple2DEffect()),
                                    m_pRenderTexture(ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd))
{
    init();
}

Canvas2D::~Canvas2D()
{
    delete m_pBufferData;
    delete m_pColorEffect;
    delete m_pBufferMesh;

    m_pRenderTexture->release();
}

/* EXTRA */
void Canvas2D::init()
{
    m_pColorEffect->load();

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, (float)GRAPHICS->getScreenWidth(), 0.0f, (float)GRAPHICS->getScreenHeight(), 0.0f, 100.0f);
}

/* GENERAL */
void Canvas2D::translate(const vec2& translation)
{

}

void Canvas2D::rotate(float rotation)
{

}

void Canvas2D::scale(const vec2& scale)
{

}

void Canvas2D::save()
{

}

void Canvas2D::restore()
{

}

/* GETTERS */
Canvas2D::Data* Canvas2D::getData() const
{
    return m_pBufferData;
}

/* SETTERS */
void Canvas2D::setStrokeColor(const Color& newColor)
{

}

void Canvas2D::setFillColor(const Color& newColor)
{
    m_FillColor = newColor;
}

void Canvas2D::setLineWidth(float width)
{

}

void Canvas2D::setGlobalAlpha(float alpha)
{

}

/* DRAW METHODS */
void Canvas2D::draw()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pBufferData->fbufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pBufferData->resolvedFbufferID);

    glBlitFramebuffer(  0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight(),
                        0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight(),
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    GUI->drawTexture2D(ResourceFactory<Texture2D>::getInstance()->get(m_pBufferData->renderTextureHnd), vec2(0,0));
}

void Canvas2D::strokeRect(const vec2& pos, const vec2& size)
{

}

void Canvas2D::fillRect(const vec2& pos, const vec2& size)
{
    /************************************************************************/
    /* TEMP TEST STUFF                                                      */
    /************************************************************************/

    GL::heBindFbo(m_pBufferData->fbufferID);

    m_OrthographicMatrix = mat44::createOrthoLH(0.0f, (float)GRAPHICS->getScreenWidth(), 0.0f, (float)GRAPHICS->getScreenHeight(), 0.0f, 100.0f);

    m_pBufferMesh->clear();
    m_pBufferMesh->addVertex(pos);
    m_pBufferMesh->addVertex(pos + vec2(size.x, size.y / 20));
    m_pBufferMesh->addVertex(pos + size - vec2(20,50));
    m_pBufferMesh->addVertex(pos + vec2(0, size.y));
    m_pBufferMesh->createBuffer();

    m_pColorEffect->begin();
    m_pColorEffect->setColor(m_FillColor);
    m_pColorEffect->setWorldMatrix(m_OrthographicMatrix);
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