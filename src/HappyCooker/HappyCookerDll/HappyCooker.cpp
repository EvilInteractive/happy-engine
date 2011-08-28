//HappyCooker Copyright (C) 2011  Bastian Damman
//
//This file is part of HappyCooker.
//
//    HappyCooker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyCooker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyCooker.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 21/08/2011

#include "HappyCooker.h"
#include "cooking/PxCooking.h"
#include "BinaryStream.h"
#include "ObjLoader.h"
#include <vector>
#include "VertexLayout.h"
#include "FileNotFoundException.h"

namespace happycooker {

HappyCooker* HappyCooker::s_pSingleton = nullptr;

HappyCooker* HappyCooker::getInstance()
{
    if (s_pSingleton == nullptr)
        s_pSingleton = new HappyCooker();
    return s_pSingleton;
}
void HappyCooker::dispose()
{
    delete s_pSingleton;
    s_pSingleton = nullptr;
}

HappyCooker::HappyCooker(): m_pPhysicsEngine(new happyengine::physics::PhysicsEngine())
{
}


HappyCooker::~HappyCooker()
{
    delete m_pPhysicsEngine;
}

bool HappyCooker::cookObjToConvex(const char* input, const char* output)
{
    using namespace happyengine;

    std::cout << "happycooker cooking: " << input << " to " << output << "\n";

    content::models::ObjLoader objLoader;

    graphics::VertexLayout layout;
    layout.addElement(graphics::VertexElement(0, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Position,
        sizeof(math::Vector3), 0, ""));

    try { objLoader.load(input, layout, false); }
    catch (error::FileNotFoundException e)
    {
        std::wcout << "error while trying to read obj: " << e.getMsg();
        return false;
    }
    
    std::cout << "read " << objLoader.getNumVertices() << " vertices and " << objLoader.getNumIndices() << " indices\n";

    std::cout << "starting cooking... \n";
    PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), PxCookingParams()));

    if (cooking == nullptr)
    {
        std::cout << "cooking failed to start\n";
        return false;
    }

    PxConvexMeshDesc desc;   
    switch (objLoader.getIndexType())
    {
        case graphics::IndexType_Byte: ASSERT("byte indices are not supported"); break;
        case graphics::IndexType_UShort: desc.flags = PxConvexFlag::e16_BIT_INDICES; break;
        case graphics::IndexType_UInt: break;
        default: ASSERT("unkown indexType"); break;
    }
 
    desc.points.count = objLoader.getNumVertices();
    desc.points.data = objLoader.getVertices();
    desc.points.stride = sizeof(VertexPos);
    desc.triangles.count = objLoader.getNumIndices() / 3;
    desc.triangles.data = objLoader.getIndices();
    desc.triangles.stride = objLoader.getIndexType() * 3; //stride of triangle = 3 indices

    io::BinaryStream stream(output, io::BinaryStream::Write);
    bool succes(cooking->cookConvexMesh(desc, stream));
    cooking->release();

    std::cout << "cooking done! \n";
    if (succes)
        std::cout << "cooking successful! :)\n";
    else
        std::cout << "cooking failed! :(\n";

    return succes;
}

bool HappyCooker::cookObjToBinObj(const char* input, const char* output)
{
    using namespace happyengine;

    std::cout << "happycooker cooking: " << input << " to " << output << "\n";

    content::models::ObjLoader objLoader;

    graphics::VertexLayout layout;
    layout.addElement(graphics::VertexElement(0, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Position, sizeof(math::Vector3), 0, ""));
    layout.addElement(graphics::VertexElement(1, graphics::VertexElement::Type_Vector2, graphics::VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, ""));
    layout.addElement(graphics::VertexElement(2, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Normal, sizeof(math::Vector3), 20, ""));

    try { objLoader.load(input, layout, false); }
    catch (error::FileNotFoundException e)
    {
        std::wcout << "error while trying to read obj: " << e.getMsg();
        return false;
    }
    
    std::cout << "read " << objLoader.getNumVertices() << " vertices and " << objLoader.getNumIndices() << " indices\n";

    std::cout << "starting cooking... \n";

    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(objLoader.getNumVertices());
    stream.storeBuffer(objLoader.getVertices(), 32 * objLoader.getNumVertices());
    stream.storeDword(objLoader.getNumIndices());
    stream.storeByte(static_cast<byte>(objLoader.getIndexType()));
    stream.storeBuffer(objLoader.getIndices(), objLoader.getIndexType() * objLoader.getNumIndices());
    std::cout << "cooking successful! :)\n";
    return true;
}

} //end namespace