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
#include "ObjLineLoader.h"

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

    std::cout << "\nhappycooker cooking: " << input << " to " << output << "\n";

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
    
    std::cout << "read " << objLoader.getNumVertices(0) << " vertices and " << objLoader.getNumIndices(0) << " indices\n";

    std::cout << "starting cooking... \n";
    PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), PxCookingParams()));

    if (cooking == nullptr)
    {
        std::cout << "cooking failed to start\n";
        return false;
    }
    
    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeByte(1);

    PxConvexMeshDesc desc;   
    switch (objLoader.getIndexStride(0))
    {
        case graphics::IndexStride_Byte: ASSERT("byte indices are not supported"); break;
        case graphics::IndexStride_UShort: desc.flags = PxConvexFlag::e16_BIT_INDICES; break;
        case graphics::IndexStride_UInt: break;
        default: ASSERT("unkown indexType"); break;
    }
 
    desc.points.count = objLoader.getNumVertices(0);
    desc.points.data = objLoader.getVertices(0);
    desc.points.stride = sizeof(VertexPos);
    desc.triangles.count = objLoader.getNumIndices(0) / 3;
    desc.triangles.data = objLoader.getIndices(0);
    desc.triangles.stride = objLoader.getIndexStride(0) * 3; //stride of triangle = 3 indices

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

    std::cout << "\nhappycooker cooking: " << input << " to " << output << "\n";

    content::models::ObjLoader objLoader;

    graphics::VertexLayout layout;
    layout.addElement(graphics::VertexElement(0, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Position, sizeof(math::Vector3), 0, ""));
    layout.addElement(graphics::VertexElement(1, graphics::VertexElement::Type_Vector2, graphics::VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, ""));
    layout.addElement(graphics::VertexElement(2, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Normal, sizeof(math::Vector3), 20, ""));
    layout.addElement(graphics::VertexElement(3, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Tangent, sizeof(math::Vector3), 32, ""));

    try { objLoader.load(input, layout, true); }
    catch (error::FileNotFoundException e)
    {
        std::wcout << "error while trying to read obj: " << e.getMsg();
        return false;
    }
    
    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(objLoader.getNumMeshes());
    std::cout << "starting cooking " << objLoader.getNumMeshes() << "meshes...\n";
    for (uint i = 0; i < objLoader.getNumMeshes(); ++i)
    {
        std::cout << "    cooking: " << objLoader.getMeshName(i) << " - " << objLoader.getNumVertices(i) << " vertices and " << objLoader.getNumIndices(i) << " indices";
        
        stream.storeString(objLoader.getMeshName(i));
        stream.storeDword(objLoader.getNumVertices(i));
        stream.storeBuffer(objLoader.getVertices(i), 44 * objLoader.getNumVertices(i));
        stream.storeDword(objLoader.getNumIndices(i));
        stream.storeByte(static_cast<byte>(objLoader.getIndexStride(i)));
        stream.storeBuffer(objLoader.getIndices(i), objLoader.getIndexStride(i) * objLoader.getNumIndices(i));
        std::cout << "    DONE!\n";
    }
 
    return true;
}

bool HappyCooker::cookObjLineToBinObj(const char* input, const char* output)
{
    using namespace happyengine;

    std::cout << "happycooker cooking: " << input << " to " << output << "\n";

    content::lines::ObjLineLoader objLoader;

    try { objLoader.load(input); }
    catch (error::FileNotFoundException e)
    {
        std::wcout << "error while trying to read obj: " << e.getMsg();
        return false;
    }
    
    std::cout << "read " << objLoader.getPoints().size() << " vertices and " << objLoader.getIndices().size() << " indices\n";

    std::cout << "starting cooking... \n";

    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(objLoader.getPoints().size());
    stream.storeBuffer(&objLoader.getPoints()[0], sizeof(math::Vector3) * objLoader.getPoints().size());
    stream.storeDword(objLoader.getIndices().size());
    stream.storeBuffer(&objLoader.getIndices()[0], sizeof(ushort) * objLoader.getIndices().size());
    std::cout << "cooking successful! :)\n";
    return true;
}

} //end namespace