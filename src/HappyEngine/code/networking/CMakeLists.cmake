set (CODE_NETWORKING_FILES  
        NetworkConnection.cpp                NetworkConnection.h
        NetworkManager.cpp                   NetworkManager.h
        NetworkObject.cpp                    NetworkObject.h
        NetworkObjectBase.cpp                NetworkObjectBase.h
        NetworkObjectFactory.cpp             NetworkObjectFactory.h
        NetworkObjectFactoryManager.cpp      NetworkObjectFactoryManager.h
        NetworkReplicaConnection.cpp         NetworkReplicaConnection.h
        NetworkSerializer.cpp                NetworkSerializer.h
        INetworkSerializable.h
        )
                        
foreach(f ${CODE_NETWORKING_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_NETWORKING_SRCS code/networking/${f})
endforeach(f) 

source_group (src\\networking FILES ${HAPPYENGINE_CODE_NETWORKING_SRCS})

