cmake_minimum_required(VERSION 3.24)

if (DEFINED ENV{EMSDK})
	set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
endif()

project(RenderSandbox)

add_subdirectory(client/cpp)
