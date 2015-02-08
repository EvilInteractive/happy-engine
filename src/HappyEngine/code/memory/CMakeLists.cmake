
addFilter(CODE_SRCS src/memory code/memory/
            MemoryManager.h						MemoryManager.cpp
            HappyAllocater.h 					HappyAllocater.cpp
            HappyMemory.h
            HappyNew.h
            HappyNewOverride.h
        )


addFilter(CODE_SRCS src/memory/nedmalloc code/memory/nedmalloc/
            malloc.c.h
            nedmalloc.c 		nedmalloc.h
        )

