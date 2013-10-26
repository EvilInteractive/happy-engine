
if (WIN32)
set(THREAD_CPP Thread_Win32.cpp)
set(MUTEX_CPP Mutex_Win32.cpp)
elseif (APPLE)
set(THREAD_CPP Thread_Mac.cpp)
set(MUTEX_CPP Mutex_Mac.cpp)
else ()
set(THREAD_CPP Thread_Linux.cpp)
set(MUTEX_CPP Mutex_Linux.cpp)
endif()

addFilter(CODE_SRCS src/thread code/thread/
            ${THREAD_CPP}          
            Thread.h
            ThreadSafety.h
            ThreadTypes.h
            ${MUTEX_CPP}
            Mutex.h
        )
