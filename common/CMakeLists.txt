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
		MathHelper.h
		MathHelper.cpp
		GeometryGenerator.h
		GeometryGenerator.cpp
		LightHelper.h
		Geometry.h
		Vertex.h
		Vertex.cpp
		Mouse.h
		Mouse.cpp
		Keyboard.h
		Keyboard.cpp
		WICTextureLoader11.h
		WICTextureLoader11.cpp
		DDSTextureLoader11.h
		DDSTextureLoader11.cpp
		Transform.h
		Transform.cpp
		Camera.h
		Camera.cpp
		RenderStates.h
		RenderStates.cpp
		EffectHelper.h
		Effects.h
		BasicEffect.cpp
)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(${target} STATIC ${common_SOURCES})

target_link_libraries(${target} d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib winmm.lib d2d1.lib dwrite.lib)

# ImGui
target_link_libraries(${target} ImGui)
