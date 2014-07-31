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
//Created: 30/09/2011

#ifndef _HE_MATERIAL_LOADER_H_
#define _HE_MATERIAL_LOADER_H_
#pragma once

#include "AssetContainer.h"

namespace he {
namespace io {
    class StructuredVisitor;
}
namespace gfx {
    class Material;
}
namespace ct {

struct MaterialDesc
{
    MaterialDesc();

    he::String m_FragmentShader;
    he::String m_VertexShader;
    bool m_IsBlended;
    bool m_NoPost;
    bool m_CullFrontFace;
    bool m_DepthRead;
    bool m_DepthWrite;
    gfx::BlendEquation m_BlendEquation;
    gfx::BlendFunc m_SourceBlend;
    gfx::BlendFunc m_DestBlend;
    he::ObjectList<he::String> m_Defines;
    he::ObjectList<he::String> m_OutputLayout;

    // Default values for uniforms go here as well

    void visit(he::io::StructuredVisitor* const visitor);
};

class MaterialLoader
{
public:
    MaterialLoader();
    ~MaterialLoader();
    
    gfx::Material* load(const he::Path& path);
    
private:
    AssetContainer<ObjectHandle> m_AssetContainer;
    
    //Disable default copy constructor and default assignment operator
    MaterialLoader(const MaterialLoader&);
    MaterialLoader& operator=(const MaterialLoader&);
};

} } //end namespace

#endif
