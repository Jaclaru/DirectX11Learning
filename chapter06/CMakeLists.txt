set(target chapter06)

set(chapter06_SOURCES
        test.cpp
        )

file(GLOB HLSL_FILES HLSL/*.hlsl HLSL/*.hlsli)
foreach(HLSL_FILE ${HLSL_FILES})

    get_filename_component(HLSL_FDIR ${HLSL_FILE} DIRECTORY)
    get_filename_component(HLSL_FBASENAME_WE ${HLSL_FILE} NAME_WE)
    string(CONCAT HLSL_FNAME_WE ${HLSL_FDIR} / ${HLSL_FBASENAME_WE})

    string(LENGTH ${HLSL_FBASENAME_WE} LEN_FNAME_WE)
    
    math(EXPR LEN_FNAME_WE "${LEN_FNAME_WE}-2")
    string(SUBSTRING ${HLSL_FBASENAME_WE} ${LEN_FNAME_WE} 2 ENTRY_POINT)
    string(TOLOWER ${ENTRY_POINT} SHADER_TYPE)

    if ("${SHADER_TYPE}" STREQUAL "vs") 
        set(SHADER_TYPE "Vertex")
    elseif("${SHADER_TYPE}" STREQUAL "hs") 
        set(SHADER_TYPE "Hull")
    elseif("${SHADER_TYPE}" STREQUAL "ds") 
        set(SHADER_TYPE "Domain")
    elseif("${SHADER_TYPE}" STREQUAL "gs") 
        set(SHADER_TYPE "Geometry")
    elseif("${SHADER_TYPE}" STREQUAL "ps")
        set(SHADER_TYPE "Pixel")
    elseif("${SHADER_TYPE}" STREQUAL "cs")
        set(SHADER_TYPE "Compute")
    endif()
    set_source_files_properties(${HLSL_FILE} PROPERTIES
                                VS_SHADER_OBJECT_FILE_NAME HLSL/${HLSL_FBASENAME_WE}.cso
                                VS_SHADER_TYPE ${SHADER_TYPE}
                                VS_SHADER_MODEL 5.0
                                VS_SHADER_ENTRYPOINT ${ENTRY_POINT}
                                VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:true>
                                VS_SHADER_ENABLE_DEBUG $<$<CONFIG:Debug>:true>)
endforeach()

add_executable( ${target} WIN32 ${chapter06_SOURCES} ${HLSL_FILES})
source_group("HLSL Files" FILES ${HLSL_FILES})

target_link_libraries( ${target}
		PRIVATE
		common
		)