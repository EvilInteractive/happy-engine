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
//Created: 07/05/2012

#include "HappyPCH.h" 
#include "FontLoader.h"

#include "ResourceFactory.h"
#include "BinObjLoader.h"
#include "MeshEnums.h"

#include <sys/stat.h>

#define FACTORY ResourceFactory<gui::Font>::getInstance()

namespace he {
namespace ct {

/* CONSTRUCTOR - DESTRUCTOR*/
FontLoader::FontLoader()
{
    const FT_Error error(FT_Init_FreeType(&m_FTLibrary));
    HE_ASSERT(error == false,"Error creating freetype library!");;
}

FontLoader::~FontLoader()
{
    ResourceFactory<gui::Font>::getInstance()->garbageCollect();
    FT_Done_FreeType(m_FTLibrary);
}

/* GENERAL */
gui::Font* FontLoader::load(const he::String& path, uint16 size, uint8 options)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << path << size;
    he::String assetName(m_TempStream.str());

    ResourceFactory<gui::Font>* const fontFactory(FACTORY);

    if (m_AssetContainer.isAssetPresent(assetName) && fontFactory->isAlive(m_AssetContainer.getAsset(assetName)))
    {
        const ObjectHandle handle(m_AssetContainer.getAsset(assetName));
        fontFactory->instantiate(handle);       
        return fontFactory->get(handle);
    }
    else
    {
        const ObjectHandle handle(fontFactory->create());

        FT_Face face;
        FT_Error error(FT_New_Face(m_FTLibrary, path.c_str(), 0, &face));

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error loading font: %s", path.c_str());
            return nullptr;
        }

        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error loading font charmap: %s", path.c_str());
            return nullptr;
        }

        // font size in 1/64 pixel
        // 96 DPI -> should change later to adapt to screen DPI
        error = FT_Set_Char_Size(face, 0, size * 64, 96, 96);

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error setting font size: %s", path.c_str());
            return nullptr;
        }
        
        gui::Font::VectorFont vectorFont;
        // Check for vector
        {
            he::String modelPath(path);
            size_t index(modelPath.rfind("."));
            if (index != he::String::npos)
            {
                modelPath.resize(index + 1);
                modelPath += "binobj";

                struct stat buffer;   
                if (stat(modelPath.c_str(), &buffer) == 0)
                {
                    models::BinObjLoader modelLoader;
                    if (modelLoader.load(modelPath))
                    {
                        char rangeStart(CHAR_MAX);
                        char rangeEnd(0);
                        const size_t meshes(modelLoader.getNumMeshes());
                        for (size_t i(0); i < meshes; ++i)
                        {
                            const char chr(modelLoader.getMeshName(i)[0]);
                            rangeStart = std::min(rangeStart, chr);
                            rangeEnd = std::max(rangeEnd, chr);
                        }
                        rangeEnd += 1; // not inclusive end

                        const gfx::VertexLayout& layout(modelLoader.getVertexLayout());
                        size_t posIndex(0);
                        if (layout.getElements().find_if([](const gfx::VertexElement& el)
                        {
                            return el.getAttribute() == gfx::eShaderAttribute_Position;
                        }, posIndex))
                        {
                            const gfx::VertexElement& el(layout.getElements()[posIndex]);
                            const char range(rangeEnd - rangeStart);
                            void** vertices = static_cast<void**>(he_malloc("FontLoader::load()::vertices", sizeof(void*) * range));
                            he_memset(vertices, 0, sizeof(void*) * range);
                            void** indices = static_cast<void**>(he_malloc("FontLoader::load()::indices", sizeof(void*) * range));
                            he_memset(indices, 0, sizeof(void*) * range);

                            uint16* vertexCountBuffer(static_cast<uint16*>(he_malloc("FontLoader::load()::vertexCountBuffer", sizeof(uint16) * range)));
                            he_memset(vertexCountBuffer, 0, sizeof(uint16) * range);
                            uint16* indexCountBuffer(static_cast<uint16*>(he_malloc("FontLoader::load()::indexCountBuffer", sizeof(uint16) * range)));
                            he_memset(indexCountBuffer, 0, sizeof(uint16) * range);

                            vectorFont.m_RangeStart = rangeStart;
                            vectorFont.m_RangeEnd = rangeEnd;
                            vectorFont.m_Vertices = vertices;
                            vectorFont.m_Indices = indices;
                            vectorFont.m_VertexCount = vertexCountBuffer;
                            vectorFont.m_IndexCount = indexCountBuffer;
                        
                            for (size_t i(0); i < meshes; ++i)
                            {
                                if (modelLoader.getIndexStride(i) == gfx::IndexStride_UShort)
                                {
                                    const he::String& name(modelLoader.getMeshName(i));
                                    char chr(name[0]);
                                    if (chr == '_')
                                    {
                                        if (name.size() == 2) // some things are prefixed _
                                            chr = name[1];
                                    }
                                    const char index(chr - rangeStart);

                                    {
                                        const char* meshVertices(static_cast<const char*>(modelLoader.getVertices(i)));
                                        const size_t vertexCount(modelLoader.getNumVertices(i));
                                        float* buffer(static_cast<float*>(he_malloc("FontLoader::load()::buffer", vertexCount * sizeof(float) * 2)));
                                        for (size_t v(0); v < vertexCount; ++v)
                                        {
                                            const float* pos(reinterpret_cast<const float*>(meshVertices + v * layout.getSize() + el.getByteOffset()));
                                            buffer[v * 2] = pos[0];
                                            buffer[v * 2 + 1] = pos[2];
                                        }
                                        HE_ASSERT(vertices[index] == nullptr, "Already mesh @index %d --leak", index);
                                        vertices[index] = buffer;
                                        vertexCountBuffer[index] = checked_numcast<uint16>(vertexCount);
                                    }
                                    {
                                        const uint16* meshIndices(static_cast<const uint16*>(modelLoader.getIndices(i)));
                                        const size_t indexCount(modelLoader.getNumIndices(i));
                                        uint16* buffer(static_cast<uint16*>(he_malloc("FontLoader::load()::buffer", indexCount * sizeof(uint16))));
                                        for (size_t v(0); v < indexCount; ++v)
                                        {
                                            const uint16* ind(meshIndices + v);
                                            buffer[v] = *ind;
                                        }
                                        HE_ASSERT(indices[index] == nullptr, "Already mesh @index %d --leak", index);
                                        indices[index] = buffer;
                                        indexCountBuffer[index] = checked_numcast<uint16>(indexCount);
                                    }
                                }
                                else
                                {
                                    HE_ERROR("VectorFont with index stride != IndexStride_UShort! : %s (%s)", modelPath.c_str(), modelLoader.getMeshName(i).c_str());
                                }
                            }
                        }
                    }
                }
            }
        }

        gui::Font* const font(fontFactory->get(handle));
        font->setName(assetName);
        font->init(m_FTLibrary, face, size, options);
        font->setVectorFont(vectorFont);
        font->setLoaded(eLoadResult_Success);

        m_AssetContainer.addAsset(assetName, handle);

        return font;
    }
}

} } //end namespace
