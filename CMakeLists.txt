cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

project(DirectX11Learning)

include_directories(common)

#
# ImGui(modified)
#
add_subdirectory("ImGui")
set_target_properties(ImGui PROPERTIES FOLDER "ImGui")

add_compile_definitions(UNICODE _UNICODE)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(chapter04)
add_subdirectory(chapter06)
add_subdirectory(chapter07)
add_subdirectory(common)


