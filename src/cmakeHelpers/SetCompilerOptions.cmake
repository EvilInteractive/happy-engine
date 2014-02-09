
 if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)") 
    add_definitions(-W4 -MP)                     #Warning 4, multiprocess compilation
    add_definitions(-fp:precise -arch:SSE2 )     #floating points, SSE2 support
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
    
    # Release specific
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ot -GS- -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0 -GR-")
    
    # Min Size Release specific
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0 -GR-")
    
    # Rel With Deb Info specific
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -GR")
    
    # Debug specific
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -WX -GR")
      
    set (linkTypes EXE SHARED)
    set (configs DEBUG RELWITHDEBINFO)
    foreach (l ${linkTypes})
    foreach (c ${configs})
        SET(link "CMAKE_${l}_LINKER_FLAGS_${c}")    
        STRING(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${${link}})
        SET(${link} "/INCREMENTAL:NO ${replacementFlags}" )       
    endforeach()
    endforeach()
else()
    add_definitions(-Wall -MP)                     #Warning 4, multiprocess compilation
    add_definitions(-fcxx-exceptions)              #c++ exceptions untill we can get rid of it
    
    # Release specific
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ot -GS- -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0")
    
    # Min Size Release specific
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0")
    
    # Rel With Deb Info specific
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    
    # Debug specific
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# Release specific
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DHE_RELEASE")

# Min Size Release specific
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DHE_RELEASE")

# Rel With Deb Info specific
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DHE_RELEASE")

# Debug specific
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DHE_DEBUG")


