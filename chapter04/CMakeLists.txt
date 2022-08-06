set(target chapter04)

set(chapter04_SOURCES
        InitDirect3D.cpp
        )

add_executable( ${target} WIN32 ${chapter04_SOURCES} )

target_link_libraries( ${target}
		PRIVATE
		common
		)