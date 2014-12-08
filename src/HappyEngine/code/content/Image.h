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
//Created: 2014/12/04

#ifndef Image_h__
#define Image_h__

#include "Texture.h"

namespace he {
namespace ct {

class HAPPY_ENTRY Image
{
public:
    struct MipData
    {
        MipData();
        ~MipData() {}
        uint32 width;
        uint32 height;
        uint8* data;
        uint32 bufferSize;
        uint8 mipLevel;
        bool isCompressed : 1;
        gfx::TextureBufferLayout format;
        gfx::TextureBufferType type;
    };
    struct Face
    {
        Face() {}
        ~Face() {}

        Face(Face&& other);
        Face& operator=(Face&& other);

        he::ObjectList<MipData> m_Mips;
    };

    Image();
    ~Image();

    Image(Image&& other);
    Image& operator=(Image&& other);

    bool load(const he::Path& path);
    bool create(const he::Color& color);
    void destroy();

    const he::ObjectList<Face>& getFaces() const { return m_Faces; }
    gfx::TextureFormat getFormat() const { return m_Format; }

private:
    he::ObjectList<Face> m_Faces;
    gfx::TextureFormat m_Format;

    Image(const Image&);
    Image& operator=(const Image&);
};

} }

#endif // Image_h__
