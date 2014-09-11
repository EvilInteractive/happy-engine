
addFilter(CODE_SRCS src/tools code/tools/
            FPSGraph.cpp                 FPSGraph.h
            Grid.cpp                     Grid.h
            Logger.cpp                   Logger.h
            Path.cpp                     Path.h
            Profiler.cpp                 Profiler.h
            StopWatch.cpp                StopWatch.h
            SystemStats.cpp				 SystemStats.h
        )

addFilter(CODE_SRCS src/tools/undoRedo code/tools/
            Command.cpp                  Command.h
            CommandStack.cpp             CommandStack.h
            CommandTransaction.cpp       CommandTransaction.h
        )
        
addFilter(CODE_SRCS src/tools/console code/tools/
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

