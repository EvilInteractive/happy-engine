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
//Created: 29/10/2011

#ifndef _HE_MODEL_COMPONENT_H_
#define _HE_MODEL_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "Drawable.h"

namespace he {
namespace gfx {
    class Drawable;
}

namespace ge {
 
struct EntityComponentDesc;   
class HAPPY_ENTRY ModelComponent : public EntityComponent
{
    struct LoadDesc
    {
        LoadDesc(): m_Mesh(nullptr), m_Material(nullptr) {}
        gfx::ModelMesh* m_Mesh;
        gfx::Material* m_Material;
    };

public:
    ModelComponent();
    virtual ~ModelComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    void init(Entity* parent);  // override, final

    void visit(he::io::BinaryVisitor* const /*visitor*/) {} // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strModelComponent; } // override, final

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    bool setProperty(const Property* const inProperty); // override, final
    bool getProperty(Property* const inOutProperty); // override, final
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
          
    const gfx::Drawable* getDrawable() const { return m_Drawable; }
    void loadModelMeshAndMaterial(const he::String& materialAsset, const he::String& modelAsset, const he::String& meshName);
    void unloadModelMeshAndMaterial();

    he::event0<void> OnModelMeshLoaded;

protected:
    Entity* m_Parent;

private:
    gfx::Drawable* m_Drawable;

    LoadDesc* m_LoadingDesc;
    
    he::String m_MaterialAsset;
    he::String m_ModelAsset;

    //Disable default copy constructor and default assignment operator
    ModelComponent(const ModelComponent&);
    ModelComponent& operator=(const ModelComponent&);
};

} } //end namespace

#endif
