set (CODE_ERROR_FILES   ExternalError.cpp           ExternalError.h
                        HappyInfo.cpp               HappyInfo.h
                        HeAssert.cpp                HeAssert.h)
                        
foreach(f ${CODE_ERROR_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_ERROR_SRCS code/error/${f})
endforeach(f) 

source_group (src\\error FILES ${HAPPYENGINE_CODE_ERROR_SRCS})