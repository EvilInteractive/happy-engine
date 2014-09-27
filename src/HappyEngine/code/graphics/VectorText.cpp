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
//Author:  Bastian Damman
//Created: 2014/09/23
#include "HappyPCH.h" 
#include "VectorText.h"

#include "Model.h"
#include "ModelMesh.h"
#include "Font.h"
#include "Simple2DEffect.h"
#include "VertexLayout.h"
#include "DrawContext.h"
#include "Canvas2D.h"

#include "GraphicsEngine.h"

namespace he {
namespace gui {

VectorText::VectorText()
    : m_Text(nullptr)
    , m_Effect(NEW gfx::Simple2DEffect())
    , m_Scale(1.0f)
{
    gfx::ModelMeshFactory* const modelMeshFactory(gfx::ModelMeshFactory::getInstance());
    m_Text = modelMeshFactory->get(modelMeshFactory->create());

    gfx::VertexLayout layout;
    layout.addElement(gfx::VertexElement(gfx::eShaderAttribute_Position, gfx::eShaderAttributeType_Float, gfx::eShaderAttributeTypeComponents_2, 0));
    m_Effect->init(layout);
    m_Effect->setColor(he::Color(1.0f, 1.0f, 1.0f, 1.0f));
    m_Effect->setDepth(0.5f);

    GRAPHICS->setActiveContext(GRAPHICS->getSharedContext());
    m_Text->init(layout, gfx::MeshDrawMode_Triangles);
}

VectorText::~VectorText()
{

}

void VectorText::create( Font* font, HAlignment halign, VAlignment valign, const he::String& text)
{
    if (!text.empty())
    {
        font->instantiate();
        font->callbackOnceIfLoaded(this, [this, font, halign, valign, text](const ELoadResult result)
        {
            const float scale(static_cast<float>(1));
            const float rescale(scale / static_cast<float>(font->getPixelHeight()));
            if (result == eLoadResult_Success)
            {
                he::vec2 pos(0, 0);
                switch (halign)
                {
                case HAlignment_Left: break;
                case HAlignment_Right: pos.x = -font->getStringWidth(text) * rescale; break;
                case HAlignment_Center: pos.x = -font->getStringWidth(text) * rescale / 2.0f; break;
                }
                switch (valign)
                {
                case VAlignment_Bottom: break;
                case VAlignment_Top: pos.y = font->getPixelHeight() * rescale; break;
                case VAlignment_Center: pos.y = font->getPixelHeight() * rescale / 2.0f; break;
                }

                const gui::Font::VectorFont& vectorFont(font->getVectorFont());

                size_t vBuffSize(0);
                size_t iBuffSize(0);
                const char* chr(text.c_str());
                do 
                {
                    const char glyph(*chr);
                    if (glyph >= vectorFont.m_RangeStart && glyph < vectorFont.m_RangeEnd)
                    {
                        const char index(glyph - vectorFont.m_RangeStart);
                        const uint16 vert(vectorFont.m_VertexCount[index]);
                        const uint16 ind(vectorFont.m_IndexCount[index]);
                        vBuffSize += vert;
                        iBuffSize += ind;
                    }
                } while(*++chr);

                float* vertBuff(static_cast<float*>(he_malloc(sizeof(float) * 2 * vBuffSize)));
                uint16* indBuff(static_cast<uint16*>(he_malloc(sizeof(uint16) * iBuffSize)));

                size_t vIndex(0);
                size_t iIndex(0);
                chr = text.c_str();
                char prev(0);
                do 
                {
                    const char glyph(*chr);
                    if (prev)
                    {
                        pos.x += font->getKerning(prev, glyph) * rescale;
                    }
                    if (glyph >= vectorFont.m_RangeStart && glyph < vectorFont.m_RangeEnd)
                    {
                        const char index(glyph - vectorFont.m_RangeStart);
                        const uint16 vertCount(vectorFont.m_VertexCount[index]);
                        const uint16 indCount(vectorFont.m_IndexCount[index]);
                        const void* vert(vectorFont.m_Vertices[index]);
                        const void* ind(vectorFont.m_Indices[index]);

                        memcpy(vertBuff + vIndex * 2, vert, sizeof(float) * 2 * vertCount);
                        // Move vertices
                        for (size_t i(0); i < vertCount; ++i)
                        {
                            vertBuff[(vIndex + i) * 2] += vertBuff[(vIndex + i) * 2] * scale + pos.x + (font->getWidth(glyph) / 2.0f + font->getOffset(glyph)) * rescale;
                            vertBuff[(vIndex + i) * 2 + 1] += vertBuff[(vIndex + i) * 2 + 1] * scale + pos.y;
                        }
                        memcpy(indBuff + iIndex, ind, sizeof(uint16) * indCount);
                        // Fix indices
                        for (size_t i(0); i < indCount; ++i)
                        {
                            indBuff[iIndex + i] = checked_numcast<uint16>(indBuff[iIndex + i] + vIndex);
                        }

                        vIndex += vertCount;
                        iIndex += indCount;
                    }
                    pos.x += font->getAdvance(glyph) * rescale;
                    prev = glyph;
                } while(*++chr);
                m_Text->setIndices(indBuff, iBuffSize, gfx::IndexStride_UShort, gfx::MeshUsage_Static);
                m_Text->setVertices(vertBuff, vBuffSize, gfx::MeshUsage_Static, true);

                he_free(vertBuff);
                he_free(indBuff);
                font->release();
            }
        });
    }
}

void VectorText::draw2D( he::gui::Canvas2D* canvas, const he::mat33& transform )
{
    gfx::DrawContext context;
    context.m_VBO = m_Text->getVBO();
    context.m_IBO = m_Text->getIBO();
    he::mat44 transform44(transform(0, 0), transform(0, 1), 0, transform(0, 2),
                          transform(1, 0), transform(1, 1), 0, transform(1, 2),
                                        0,               0, 1,              0,
                          transform(2, 0), transform(2, 1), 0, transform(2, 2));
    m_Effect->setWorldMatrix(canvas->getMatrix() * transform44 * mat44::createTranslation(he::vec3(m_Position.x, m_Position.y, 0)) * he::mat44::createScale(he::vec3(m_Scale, m_Scale, 1)));
    m_Effect->apply(context);
    m_Text->draw();
}

he::vec2 VectorText::getSize()
{
    return he::vec2(m_Text->getBound().getAABB().getBottomBackRight().xy() - m_Text->getBound().getAABB().getTopFrontLeft().xy()) * m_Scale;
}

} }