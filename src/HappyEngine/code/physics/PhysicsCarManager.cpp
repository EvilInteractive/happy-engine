////HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
////
////This file is part of HappyEngine.
////
////    HappyEngine is free software: you can redistribute it and/or modify
////    it under the terms of the GNU Lesser General Public License as published by
////    the Free Software Foundation, either version 3 of the License, or
////    (at your option) any later version.
////
////    HappyEngine is distributed in the hope that it will be useful,
////    but WITHOUT ANY WARRANTY; without even the implied warranty of
////    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
////    GNU Lesser General Public License for more details.
////
////    You should have received a copy of the GNU Lesser General Public License
////    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
////
////Author:  Bastian Damman
////Created: 20/11/2011
//#include "HappyPCH.h"
//
//#include "PhysicsCarManager.h"
//#include "HappyNew.h"
//
//#include "PhysicsCar.h"
//
//namespace he {
//namespace px {
//
//PhysicsCarManager::PhysicsCarManager(): m_SqWheelRaycastSceneQuery(nullptr)
//{
//}
//
//
//PhysicsCarManager::~PhysicsCarManager()
//{
//    std::for_each(m_CarList.cbegin(), m_CarList.cend(), [](const PhysicsCar* pCar)
//    {
//        delete pCar;
//    });
//}
//
//void PhysicsCarManager::init( const DriveableSurfaceTyreFrictionTable* pFrictionTable )
//{
//    m_pSurfaceTyreFrictionTable = pFrictionTable;
//    m_SqWheelRaycastSceneQuery = physx::PxVehicle4WSetUpSceneQuery(PHYSICS->getScene(), m_SqData);
//}
//const DriveableSurfaceTyreFrictionTable* PhysicsCarManager::getFrictionTable() const
//{
//    return m_pSurfaceTyreFrictionTable;
//}
//
//PhysicsCar* PhysicsCarManager::createCar()
//{
//    PhysicsCar* pCar(NEW PhysicsCar());
//
//    return pCar;
//}
//void PhysicsCarManager::startCarSimulation( PhysicsCar* pCar )
//{
//    m_Mutex.lock();
//    m_CarList.push_back(pCar);
//    m_InternalCarList.push_back(&pCar->m_Vehicle);
//    m_Mutex.unlock();
//}
//
//void PhysicsCarManager::removeCar( PhysicsCar* pCar )
//{
//    m_Mutex.lock();
//    m_CarList.erase(std::remove(m_CarList.begin(), m_CarList.end(), pCar), m_CarList.end());
//    m_InternalCarList.erase(std::remove(m_InternalCarList.begin(), m_InternalCarList.end(), &pCar->m_Vehicle), m_InternalCarList.end());
//    delete pCar;
//    m_Mutex.unlock();
//}
//
//void PhysicsCarManager::tick( float dTime )
//{
//    m_Mutex.lock();
//    //Raycasts.
//    if (m_InternalCarList.size() > 0)
//        physx::PxVehicle4WSuspensionRaycasts(m_SqWheelRaycastSceneQuery, m_InternalCarList.size(), m_SqData.mSqResults, &m_InternalCarList[0]);
//
//    //Update Cars
//    std::for_each(m_CarList.cbegin(), m_CarList.cend(), [&](PhysicsCar* pCar)
//    {
//        pCar->tick(dTime);
//    });
//
//    if (m_InternalCarList.size() > 0)
//    {
//        physx::PxVehicleDrivableSurfaceToTyreFrictionPairs surfaceTyrePairs;
//        surfaceTyrePairs.mPairs= PHYSICS->getCarManager()->getFrictionTable()->getFrictionPairs();
//        surfaceTyrePairs.mNumSurfaceTypes= PHYSICS->getCarManager()->getFrictionTable()->getNumSurfaces();
//        surfaceTyrePairs.mNumTyreTypes= PHYSICS->getCarManager()->getFrictionTable()->getNumTyreTypes();
//        PxVehicle4WUpdate(dTime, PHYSICS->getScene()->getGravity(), surfaceTyrePairs, 1, &m_InternalCarList[0]);
//    }
//
//    m_Mutex.unlock();
//}
//
//DriveableSurfaceTyreFrictionTable::DriveableSurfaceTyreFrictionTable( byte drivableSurfaces, byte tyreTypes ):
//        m_DrivableSurfaces(drivableSurfaces), m_TyreTypes(tyreTypes)
//{
//    HE_ASSERT((drivableSurfaces * tyreTypes) % 4 == 0, "Error drivableSurfaces * tyreTypes must be multiple of 4");
//
//    m_TyreFrictionPair = (float*)he_aligned_malloc(drivableSurfaces * tyreTypes * sizeof(float), 16);
//    m_VehicleDrivableSurfaceTypes = (physx::PxVehicleDrivableSurfaceType*)he_aligned_malloc(sizeof(physx::PxVehicleDrivableSurfaceType) * m_DrivableSurfaces, 16);
//
//    m_DrivableMaterials.resize(drivableSurfaces);
//    m_PxDrivableMaterials.resize(drivableSurfaces);
//}
//
//DriveableSurfaceTyreFrictionTable::~DriveableSurfaceTyreFrictionTable()
//{
//    he_aligned_free(m_TyreFrictionPair);
//    he_aligned_free(m_VehicleDrivableSurfaceTypes);
//}
//
//void DriveableSurfaceTyreFrictionTable::setMaterial(byte id, const PhysicsMaterial& material)
//{
//    HE_ASSERT(id < m_DrivableSurfaces, "id >= max driveable surfaces");
//    m_DrivableMaterials[id] = material;
//    m_PxDrivableMaterials[id] = material.getInternalMaterial();
//
//    m_VehicleDrivableSurfaceTypes[id].mType = id;
//    void* pUserData = &m_VehicleDrivableSurfaceTypes[id];
//    m_PxDrivableMaterials[id]->userData = pUserData;
//}
//void DriveableSurfaceTyreFrictionTable::setTyreType(byte id, float globalFrictionMult)
//{
//    HE_ASSERT(id < m_TyreTypes, "id >= max tyre type");
//    for (int groundID(0); groundID < m_DrivableSurfaces; ++groundID)
//    {
//        m_TyreFrictionPair[groundID * m_TyreTypes + id] = m_DrivableMaterials[groundID].getInternalMaterial()->getStaticFriction() * globalFrictionMult;
//    }
//}
//void DriveableSurfaceTyreFrictionTable::setTyreMaterialMult(byte tyreId, byte materialId, float mult)
//{
//    HE_ASSERT(tyreId < m_TyreTypes, "tyreId >= max tyre type");
//    HE_ASSERT(materialId < m_DrivableSurfaces, "materialId >= max driveable surfaces");
//    m_TyreFrictionPair[materialId * m_TyreTypes + tyreId] = m_DrivableMaterials[materialId].getInternalMaterial()->getStaticFriction() * mult;
//}
//
//float* DriveableSurfaceTyreFrictionTable::getFrictionPairs() const
//{
//    return m_TyreFrictionPair;
//}
//
//byte DriveableSurfaceTyreFrictionTable::getNumSurfaces() const
//{
//    return m_DrivableSurfaces;
//}
//
//byte DriveableSurfaceTyreFrictionTable::getNumTyreTypes() const
//{
//    return m_TyreTypes;
//}
//
//const PhysicsMaterial& DriveableSurfaceTyreFrictionTable::getMaterial( byte materialId ) const
//{
//    HE_ASSERT(materialId < m_DrivableSurfaces, "materialId >= max driveable surfaces");
//    return m_DrivableMaterials[materialId];
//}
//
//} } //end namespace
