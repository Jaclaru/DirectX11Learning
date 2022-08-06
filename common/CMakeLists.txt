set(target common)

set(common_SOURCES
        D3DApp.h
		D3DApp.cpp
		WinMin.h
		CpuTimer.h
		CpuTimer.cpp
		D3DUtil.h
		D3DUtil.cpp
		DXTrace.h
		DXTrace.cpp
)

add_library(${target} STATIC ${common_SOURCES})
