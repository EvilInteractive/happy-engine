MACRO(addFilter codeSRCS filter pathprefix)  

set(CODE_FILES)
set(CODE_FILES_FILTER_SRCS)

foreach(f ${ARGN}) 
    LIST(APPEND CODE_FILES_FILTER_SRCS ${pathprefix}${f})
endforeach(f) 

STRING(REPLACE "/" "\\\\" correctFilter ${filter})
    
source_group(${correctFilter} FILES ${CODE_FILES_FILTER_SRCS})
LIST(APPEND ${codeSRCS} ${CODE_FILES_FILTER_SRCS})

ENDMACRO(addFilter codeSRCS filter pathprefix)