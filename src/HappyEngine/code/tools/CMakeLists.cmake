set (CODE_TOOL_NO_FILTER
        FPSGraph.cpp                 FPSGraph.h
        Grid.cpp                     Grid.h
        LoadingScreen.cpp            LoadingScreen.h
        Logger.cpp                   Logger.h
        Path.cpp                     Path.h
        Profiler.cpp                 Profiler.h)
        
set (CODE_TOOL_CONSOLE_FILTER
        BoolTypeHandler.h
        Console.cpp                  Console.h
        FloatTypeHandler.h
        IntTypeHandler.h
        ITypeHandler.h
        StringTypeHandler.h
        UIntTypeHandler.h
        Vec2TypeHandler.h
        Vec3TypeHandler.h
        Vec4TypeHandler.h
)
                      

foreach(f ${CODE_TOOL_NO_FILTER}) 
    LIST(APPEND CODE_TOOL_NO_FILTER_SRCS code/tools/${f})
endforeach(f) 

foreach(f ${CODE_TOOL_CONSOLE_FILTER}) 
    LIST(APPEND CODE_TOOL_CONSOLE_FILTER_SRC code/tools/${f})
endforeach(f) 

source_group (src\\tools FILES ${CODE_TOOL_NO_FILTER_SRCS})
source_group (src\\tools\\console FILES ${CODE_TOOL_CONSOLE_FILTER_SRC})

LIST(APPEND HAPPYENGINE_CODE_TOOLS_SRCS ${CODE_TOOL_NO_FILTER_SRCS} ${CODE_TOOL_CONSOLE_FILTER_SRC})

