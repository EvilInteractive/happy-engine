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
#include "MathFunctions.h"

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

HappyCooker::HappyCooker(): m_pPhysicsEngine(new he::px::PhysicsEngine())
{
}


HappyCooker::~HappyCooker()
{
    delete m_pPhysicsEngine;
}

bool HappyCooker::cookObjToConvex(const char* input, const char* output)
{
    using namespace he;

    std::cout << "\nhappycooker cooking: " << input << " to " << output << "\n";

    ct::models::ObjLoader objLoader;

    gfx::VertexLayout layout;
    layout.addElement(gfx::VertexElement(0, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Position,
        sizeof(vec3), 0));

    try { objLoader.load(input, layout, false); }
    catch (err::FileNotFoundException e)
    {
        std::wcout << "err while trying to read obj: " << e.getMsg();
        return false;
    }
    std::cout << "starting cooking... \n";
    PxCooking* cooking(PxCreateCooking(PX_PHYSICS_VERSION, &m_pPhysicsEngine->getSDK()->getFoundation(), PxCookingParams()));

    if (cooking == nullptr)
    {
        std::cout << "cooking failed to start\n";
        return false;
    }
    io::BinaryStream stream(output, io::BinaryStream::Write);

    byte numMeshes(static_cast<byte>(min<uint>(objLoader.getNumMeshes(), 255)));
    stream.storeByte(numMeshes);
    bool succes(true);
    for (uint i = 0; i < numMeshes; ++i)
    {
        std::cout << "   cooking " << objLoader.getMeshName(i) << " - " << objLoader.getNumVertices(i) << " vertices and " << objLoader.getNumIndices(i) << " indices";    

        PxConvexMeshDesc desc;   
        switch (objLoader.getIndexStride(i))
        {
            case gfx::IndexStride_Byte: ASSERT("byte indices are not supported"); break;
            case gfx::IndexStride_UShort: desc.flags = PxConvexFlag::e16_BIT_INDICES; break;
            case gfx::IndexStride_UInt: break;
            default: ASSERT("unkown indexType"); break;
        }
 
        desc.points.count = objLoader.getNumVertices(i);
        desc.points.data = objLoader.getVertices(i);
        desc.points.stride = sizeof(VertexPos);
        desc.triangles.count = objLoader.getNumIndices(i) / 3;
        desc.triangles.data = objLoader.getIndices(i);
        desc.triangles.stride = objLoader.getIndexStride(i) * 3; //stride of triangle = 3 indices

        if (cooking->cookConvexMesh(desc, stream))
        {
            std::cout << "     DONE!\n";
        }
        else
        {
            std::cout << "     FAILED!\n";
            succes = false;
        }        
    }
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
    using namespace he;

    std::cout << "\nhappycooker cooking: " << input << " to " << output << "\n";

    ct::models::ObjLoader objLoader;

    gfx::VertexLayout layout;
    layout.addElement(gfx::VertexElement(0, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Position, sizeof(vec3), 0));
    layout.addElement(gfx::VertexElement(1, gfx::VertexElement::Type_Vector2, gfx::VertexElement::Usage_TextureCoordinate, sizeof(vec2), 12));
    layout.addElement(gfx::VertexElement(2, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Normal, sizeof(vec3), 20));
    layout.addElement(gfx::VertexElement(3, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Tangent, sizeof(vec3), 32));

    try { objLoader.load(input, layout, true); }
    catch (err::FileNotFoundException e)
    {
        std::wcout << "err while trying to read obj: " << e.getMsg();
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
    using namespace he;

    std::cout << "happycooker cooking: " << input << " to " << output << "\n";

    ct::lines::ObjLineLoader objLoader;

    try { objLoader.load(input); }
    catch (err::FileNotFoundException e)
    {
        std::wcout << "err while trying to read obj: " << e.getMsg();
        return false;
    }
    
    std::cout << "read " << objLoader.getPoints().size() << " vertices and " << objLoader.getIndices().size() << " indices\n";

    std::cout << "starting cooking... \n";

    io::BinaryStream stream(output, io::BinaryStream::Write);
    stream.storeDword(objLoader.getPoints().size());
    stream.storeBuffer(&objLoader.getPoints()[0], sizeof(vec3) * objLoader.getPoints().size());
    stream.storeDword(objLoader.getIndices().size());
    stream.storeBuffer(&objLoader.getIndices()[0], sizeof(ushort) * objLoader.getIndices().size());
    std::cout << "cooking successful! :)\n";
    return true;
}

} //end namespace