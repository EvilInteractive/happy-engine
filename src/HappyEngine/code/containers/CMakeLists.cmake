set (CODE_CONTAINERS_FILES  
							Creator.h
							List.h
							NodeGraph.inl			NodeGraph.h
							NodeGraphError.h
							NodeGraphNode.inl		NodeGraphNode.h
							ObjectFactory.h
                            Resource.h
                            ResourceFactory.h
                            SlotPContainer.h)
                        
foreach(f ${CODE_CONTAINERS_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_CONTAINERS_SRCS code/containers/${f})
endforeach(f) 

source_group (src\\containers FILES ${HAPPYENGINE_CODE_CONTAINERS_SRCS})
