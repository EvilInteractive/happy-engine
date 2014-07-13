cmake_minimum_required (VERSION 2.8.11)

add_definitions(-DQT_NO_OPENGL_ES_2)

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/CMakeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/)
set (HAPPYSANDBOX_SRCS ${HAPPYSANDBOX_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# extra includes
if (WIN32)
	set(QT_PATH "" CACHE STRING "Path to the qt sdk directory")
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} {QT_PATH})
else()
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local/opt/qt5)
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${Qt5OpenGL_INCLUDE_DIRS})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC OFF) # needs to be off for pch support
set(CMAKE_AUTOMOC_MOC_OPTIONS "-bHappySandBoxPCH.h" "-fHappySandBoxPCH.h")
#set(CMAKE_AUTOUIC ON) # only from cmake 3.0 :(
#set(CMAKE_AUTORCC ON) # only from cmake 3.0 :(

# include + link 3rdParty
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# wrap qt
STRING(REGEX MATCHALL [^;]+\\.h HAPPYSANDBOX_MOC_SRCS "${HAPPYSANDBOX_SRCS}")
STRING(REGEX MATCHALL [^;]+\\.ui HAPPYSANDBOX_UIC_SRCS "${HAPPYSANDBOX_SRCS}")
STRING(REGEX MATCHALL [^;]+\\.qrc HAPPYSANDBOX_RCC_SRCS "${HAPPYSANDBOX_SRCS}")

qt5_wrap_cpp(HAPPYSANDBOX_MOC_LINK ${HAPPYSANDBOX_MOC_SRCS})
qt5_wrap_ui(HAPPYSANDBOX_UI_LINK ${HAPPYSANDBOX_UIC_SRCS})
qt5_add_resources(HAPPYSANDBOX_RCC_LINK ${HAPPYSANDBOX_RCC_SRCS})

addFilter(HAPPYSANDBOX_SRCS moc "" ${HAPPYSANDBOX_MOC_LINK})
addFilter(HAPPYSANDBOX_SRCS ui "" ${HAPPYSANDBOX_UI_LINK})
addFilter(HAPPYSANDBOX_SRCS rcc "" ${HAPPYSANDBOX_RCC_LINK})

# add HappySandbox executable
add_executable (HappySandBox ${HAPPYSANDBOX_SRCS})

include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/CopyHappyRuntime.cmake)
copyHappyRuntime(HappySandBox)

# link thirdparty
LinkThirdParty(HappySandBox)

# exra links
target_link_libraries(HappySandBox Qt5::Widgets)
target_link_libraries(HappySandBox Qt5::OpenGL)
target_link_libraries(HappySandBox Qt5::Gui)

# link HappyEngine
target_link_libraries(HappySandBox HappyEngine)

# link qt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Gui REQUIRED)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappySandBox code/ HappySandBoxPCH.h HappySandBoxPCH.cpp)

if(TARGET HappySandBoxPCH)
    target_link_libraries(HappySandBoxPCH Qt5::Widgets)
    target_link_libraries(HappySandBoxPCH Qt5::OpenGL)
    target_link_libraries(HappySandBoxPCH Qt5::Gui)
else()
    # Force PCH for the generated MOC files
    foreach (src_file ${HAPPYSANDBOX_MOC_LINK})
        set_source_files_properties (${src_file} PROPERTIES COMPILE_FLAGS "/YuHappySandBoxPCH.h /FIHappySandBoxPCH.h")
    endforeach()
    foreach (src_file ${HAPPYSANDBOX_RCC_LINK})
        set_source_files_properties (${src_file} PROPERTIES COMPILE_FLAGS "/YuHappySandBoxPCH.h /FIHappySandBoxPCH.h")
    endforeach()
endif()
    
# Copy qt dll's
if (WIN32)
    SET(QT_DLL_LIST Qt5Core Qt5OpenGL Qt5Widgets Qt5Gui)
    foreach (f ${QT_DLL_LIST})
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}d.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/Debug )
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/MinSizeRel)
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/Release)
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}d.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/RelWithDebInfo)
    endforeach()
    
    SET(QTDEP_DLL_LIST icuin52 icuuc52 icudt52)
    foreach (f ${QTDEP_DLL_LIST})
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/Debug )
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/MinSizeRel)
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/Release)
        file(INSTALL ${_qt5Widgets_install_prefix}/bin/${f}.dll DESTINATION ${HappyEngine_SOURCE_DIR}/bin/RelWithDebInfo)
    endforeach()
endif()
# set output dir
SET_TARGET_PROPERTIES(HappySandBox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)   
SET_TARGET_PROPERTIES(HappySandBox PROPERTIES FOLDER "HappyTools")
