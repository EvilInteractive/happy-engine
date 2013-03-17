include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYJSON_SRCS src ../../../HappyJson/src/HappyJson/
            json_batchallocator.h
            json_internalarray.inl
            json_internalmap.inl
            json_reader.cpp
            json_tool.h
            json_value.cpp
            json_valueiterator.inl
            json_writer.cpp
        )
addFilter(HAPPYJSON_SRCS include ../../../HappyJson/include/json/
            autolink.h
            config.h
            features.h
            forwards.h
            json.h
            reader.h
            value.h
            writer.h
)
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyJson/include)

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# Create HappyJson library
add_library (HappyJson ${HAPPYJSON_SRCS})

#set output dir
SET_TARGET_PROPERTIES(HappyJson PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyJson/bin)                     
                            