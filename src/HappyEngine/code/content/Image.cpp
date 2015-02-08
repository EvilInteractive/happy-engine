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
#include "HappyPCH.h"
#include "Image.h"

#undef UNICODE
#undef _UNICODE
#include "IL/il.h"
#include "IL/ilu.h"

namespace he {
namespace ct {

Image::MipData::MipData()
: width(0)
, height(0)
, data(nullptr)
, bufferSize(0)
, mipLevel(0)
, isCompressed(false)
, format(gfx::TextureBufferLayout_Invalid)
, type(gfx::TextureBufferType_Byte)
{

}

Image::Face::Face( Face&& other )
    : m_Mips(std::move(other.m_Mips))
{

}

Image::Face& Image::Face::operator=( Face&& other )
{
    m_Mips = std::move(other.m_Mips);
    return *this;
}

Image::Image()
    : m_Format(gfx::TextureFormat_Invalid)
{

}

Image::~Image()
{

}

Image::Image( Image&& other )
    : m_Faces(std::move(other.m_Faces))
    , m_Format(other.m_Format)
{

}

void Image::destroy()
{
    m_Faces.forEach([](const Face& face)
    {
        face.m_Mips.forEach([](const MipData& mip)
        {
            he_aligned_free(mip.data);
        });
    });
}

Image& Image::operator=( Image&& other )
{
    m_Faces = std::move(other.m_Faces);
    m_Format = other.m_Format;
    return *this;
}

inline void handleILError(const he::String& file)
{
    ILenum err = ilGetError();
    while (err != IL_NO_ERROR)
    {
        const char* errorstr(iluErrorString(err));
        HE_ERROR("%s: %s", errorstr, file.c_str());
        err = ilGetError();
    }
}

bool Image::load( const he::Path& path )
{ 
    bool result(false);
    const ILuint ilImage(ilGenImage());
    ilBindImage(ilImage);
    m_Format = gfx::TextureFormat_Invalid;
    if (ilLoadImage(path.str().c_str()))
    {
        ILuint dxtcFormat(ilGetInteger(IL_DXTC_DATA_FORMAT));
        uint32 channels(ilGetInteger(IL_IMAGE_CHANNELS));
        bool isCompressed(true);
        if (dxtcFormat == IL_DXT_NO_COMP)
        {
            m_Format = gfx::TextureFormat_RGBA8;
            isCompressed = false;
        }
        else if (dxtcFormat == IL_DXT1)
        {
            if (channels == 4)
                m_Format = gfx::TextureFormat_Compressed_RGBA8_DXT1;
            else if (channels == 3)
                m_Format = gfx::TextureFormat_Compressed_RGB8_DXT1;
            else
            {
                HE_ERROR("Unsupported channel count (%d) for DXT1 compression - %s", channels, path.str().c_str());
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT3)
        {
            m_Format = gfx::TextureFormat_Compressed_RGBA8_DXT3;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT3 compression - %s", channels, path.str().c_str());
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT5)
        {
            m_Format = gfx::TextureFormat_Compressed_RGBA8_DXT5;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT5 compression - %s", channels, path.str().c_str());
                return false;
            }
        }
        else
        {
            HE_ERROR("Unsupported compression type: %d", dxtcFormat);
            return false;
        }

        ILuint numMipMaps(ilGetInteger(IL_NUM_MIPMAPS));
        ILuint numFaces(ilGetInteger(IL_NUM_FACES) + 1);
        for (uint8 f(0); f < numFaces; ++f)
        {
            Face face;
            for (uint8 m(0); m <= numMipMaps; ++m) // <= : 0 = normal, 1+ are mipmaps
            {
                ilBindImage(ilImage); // reset image else mip counter is relative to previous
                ilActiveFace(f);
                ilActiveMipmap(m);

                MipData mipData;

                mipData.mipLevel = m;
                mipData.isCompressed = isCompressed;
                mipData.width = ilGetInteger(IL_IMAGE_WIDTH);
                mipData.height = ilGetInteger(IL_IMAGE_HEIGHT);

                if (isCompressed)
                {
                    mipData.bufferSize = ilGetDXTCData(nullptr, 0, dxtcFormat);
                    mipData.data = static_cast<uint8*>(he_aligned_malloc("Image::load()::mipdata.data", mipData.bufferSize, 16));
                    if (ilGetDXTCData(mipData.data, mipData.bufferSize, dxtcFormat) == 0)
                    {
                        HE_ERROR("Could not compress texture - %s", path.str().c_str());
                        he_aligned_free(mipData.data);
                        mipData.data = nullptr;
                        handleILError(path.str());
                        return false;
                    }
                }
                else
                {
                    if (ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE))
                    {
                        mipData.bufferSize = 4 * sizeof(uint8) * mipData.width * mipData.height;
                        mipData.data = static_cast<uint8*>(he_aligned_malloc("Image::load()::mipdata.data", mipData.bufferSize, 16));
                        mipData.format = gfx::TextureBufferLayout_BGRA;
                        mipData.type = gfx::TextureBufferType_Byte;
                        he_memcpy(mipData.data, ilGetData(), mipData.bufferSize);
                    }
                    else
                    {
                        handleILError(path.str());
                        return false;
                    }
                }

                face.m_Mips.add(mipData);
            }
            m_Faces.add(std::move(face));
        }
        result = true;
    }
    else
    {
        handleILError(path.str().c_str());
    }

    ilDeleteImage(ilImage);
    return result;
}

bool Image::create( const he::Color& color )
{
    m_Format = gfx::TextureFormat_RGBA8;

    MipData mip;
    mip.bufferSize = 4 * sizeof(uint8);
    mip.data = static_cast<uint8*>(he_aligned_malloc("Image::create()::mip.data", mip.bufferSize, 16));
    mip.width = 1;
    mip.height = 1;
    mip.format = gfx::TextureBufferLayout_BGRA;
    mip.type = gfx::TextureBufferType_Byte;
    mip.data[0] = color.bByte();
    mip.data[1] = color.gByte();
    mip.data[2] = color.rByte();
    mip.data[3] = color.aByte();
    mip.isCompressed = false;
    mip.mipLevel = 0;

    Face face;
    face.m_Mips.add(std::move(mip));

    m_Faces.add(std::move(face));

    return true;
}

} }

