//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 07/12/2011
#include "HappyPCH.h" 

#include "InstancingController.h"
#include "HappyEngine.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "OpenGL.h"
#include "I3DObject.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

InstancingController::InstancingController(bool dynamic, const ModelMesh::pointer& mesh, const Material& material):
    m_Dynamic(dynamic), m_pModelMesh(mesh), m_Material(material), m_NeedsUpdate(false), m_MatrixBufferCapacity(32)
{
    m_pModelMesh->callbackIfLoaded(boost::bind(&InstancingController::init, this));
}


InstancingController::~InstancingController()
{
    glDeleteVertexArrays(1, &m_Vao);
    glDeleteBuffers(1, &m_MatrixBuffer);
}


void InstancingController::init()
{

    //////////////////////////////////////////////////////////////////////////
    ///  Regular Draw
    //////////////////////////////////////////////////////////////////////////

        glGenVertexArrays(1, &m_Vao);
        GL::heBindVao(m_Vao);
        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOID());
        VertexLayout::layout elements(m_pModelMesh->getVertexLayout().getElements());
        std::for_each(elements.cbegin(), elements.cend(), [&](const VertexElement& e)
        {
            GLint components = 1;
            GLenum type = 0;
            switch (e.getType())
            {
                case VertexElement::Type_Vec2: type = GL_FLOAT; components = 2; break;
                case VertexElement::Type_Vec3: type = GL_FLOAT; components = 3; break;
                case VertexElement::Type_Vec4: type = GL_FLOAT; components = 4; break;
                case VertexElement::Type_Float: type = GL_FLOAT; break;

                case VertexElement::Type_Int: type = GL_INT; break;
                case VertexElement::Type_IVec4: type = GL_INT; components = 4; break;
                case VertexElement::Type_UInt: type = GL_UNSIGNED_INT; break;

                #pragma warning(disable:4127)
                default: ASSERT(false, "unknown/unsupported attribute type for instancing"); break;
                #pragma warning(default:4127)
            }
            glVertexAttribPointer(e.getElementIndex(), components, type, 
                GL_FALSE, m_pModelMesh->getVertexLayout().getVertexSize(), 
                BUFFER_OFFSET(e.getByteOffset())); 
            glEnableVertexAttribArray(e.getElementIndex());
        });

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModelMesh->getVBOIndexID());

        //////////////////////////////////////////////////////////////////////////
        ///  Instance Buffer
        //////////////////////////////////////////////////////////////////////////
        glGenBuffers(1, &m_MatrixBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_MatrixBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_MatrixBufferCapacity * sizeof(mat44), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);
        glEnableVertexAttribArray(elements.size() + 0);
        glEnableVertexAttribArray(elements.size() + 1);
        glEnableVertexAttribArray(elements.size() + 2);
        glEnableVertexAttribArray(elements.size() + 3);
        glVertexAttribPointer(elements.size() + 0, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 0)); 
        glVertexAttribPointer(elements.size() + 1, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 1)); 
        glVertexAttribPointer(elements.size() + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 2)); 
        glVertexAttribPointer(elements.size() + 3, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 3)); 
        glVertexAttribDivisor(elements.size() + 0, 1);
        glVertexAttribDivisor(elements.size() + 1, 1);
        glVertexAttribDivisor(elements.size() + 2, 1);
        glVertexAttribDivisor(elements.size() + 3, 1);

    //////////////////////////////////////////////////////////////////////////
    ///  Shadow Draw
    //////////////////////////////////////////////////////////////////////////
        glGenVertexArrays(1, &m_ShadowVao);
        GL::heBindVao(m_ShadowVao);

        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOShadowID());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), BUFFER_OFFSET(0)); 
        glEnableVertexAttribArray(0);

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModelMesh->getVBOIndexID());

        //////////////////////////////////////////////////////////////////////////
        ///  Instance Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_MatrixBuffer);
        glEnableVertexAttribArray(1 + 0);
        glEnableVertexAttribArray(1 + 1);
        glEnableVertexAttribArray(1 + 2);
        glEnableVertexAttribArray(1 + 3);
        glVertexAttribPointer(1 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 0)); 
        glVertexAttribPointer(1 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 1)); 
        glVertexAttribPointer(1 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 2)); 
        glVertexAttribPointer(1 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 3)); 
        glVertexAttribDivisor(1 + 0, 1);
        glVertexAttribDivisor(1 + 1, 1);
        glVertexAttribDivisor(1 + 2, 1);
        glVertexAttribDivisor(1 + 3, 1);


    GL::heBindVao(0);

    GRAPHICS->addToDrawList(this);
}

void InstancingController::updateBuffer()
{
    // only need to update once a frame
    boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - m_PrevUpdateTime);

    if ((m_Dynamic || m_NeedsUpdate) && elapsedTime.count() / static_cast<float>(boost::nano::den) * 1000 > 12.0f)
    {
        std::vector<mat44> matrixBuffer;
        matrixBuffer.reserve(m_Instances.size());

        he::for_each(m_Instances.cbegin(), m_Instances.cend(), [&matrixBuffer](const I3DObject* pObj)
        {
            matrixBuffer.push_back(pObj->getWorldMatrix());
        });
        m_InstancesInBuffer = matrixBuffer.size();

        glBindBuffer(GL_ARRAY_BUFFER, m_MatrixBuffer);

        if (m_MatrixBufferCapacity < matrixBuffer.size())
        {
            while(m_MatrixBufferCapacity < matrixBuffer.size())
                m_MatrixBufferCapacity *= 2;
            glBufferData(GL_ARRAY_BUFFER, m_MatrixBufferCapacity * sizeof(mat44), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);
            std::cout << "Increasing instancing controller's capacity to " << m_MatrixBufferCapacity << "\n";
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, matrixBuffer.size()*sizeof(mat44), matrixBuffer.size() > 0 ? &matrixBuffer[0] : 0);

        m_NeedsUpdate = false;
        m_PrevUpdateTime = boost::chrono::high_resolution_clock::now();
    }
}

void InstancingController::draw()
{
    updateBuffer();

    GL::heBindVao(m_Vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_InstancesInBuffer);
}
void InstancingController::drawShadow()
{
    updateBuffer();

    GL::heBindVao(m_ShadowVao);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_InstancesInBuffer);
}



uint InstancingController::addInstance(const I3DObject* pObj)
{
    m_NeedsUpdate = true;
    return m_Instances.insert(pObj);
}
void InstancingController::removeInstance( uint id )
{
    ASSERT(m_Dynamic == true, "use dynamic buffer if you want to remove instances");
    m_Instances.remove(id);
}

const Material& InstancingController::getMaterial() const
{
    return m_Material;
}
const ModelMesh::pointer& InstancingController::getModelMesh() const
{
    return m_pModelMesh;
}

void InstancingController::applyMaterial(const ICamera* pCamera) const
{
    m_Material.apply(this, pCamera);
}
void InstancingController::applyMaterial( const Material& customMaterial, const ICamera* pCamera ) const
{
    customMaterial.apply(this, pCamera);
}

bool InstancingController::getCastsShadow() const
{
    return m_CastShadows;
}

void InstancingController::setCastsShadow( bool castShadow )
{
    m_CastShadows = castShadow;
}

bool InstancingController::isVisible() const
{
    return m_IsVisible;
}

void InstancingController::setVisible( bool visible )
{
    m_IsVisible = visible;
}

uint InstancingController::getCount() const
{
    return m_InstancesInBuffer;
}

} } //end namespace