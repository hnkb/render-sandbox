
set(TARGET_NAME cpp-core)

add_executable(${TARGET_NAME}
	text/Font.cpp
	text/Layout.cpp
	utils/Compression.cpp
	main.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

# Use -Ofast instead of -O3 or -O2 as we don't need floating point standard-compliance
string (REGEX REPLACE "([\\/\\-]O)[0-3]" "\\1fast" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string (REGEX REPLACE "([\\/\\-]O)[0-3]" "\\1fast" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if (DEFINED EMSCRIPTEN)
	message("Configuring for Emscripten build ${CMAKE_BUILD_TYPE}")

	# workaround for VSCode bug calling CMake with empty build type
	target_compile_options(${TARGET_NAME} PRIVATE
		$<$<CONFIG:>:-Ofast>
		-sUSE_HARFBUZZ
	)

	target_link_options(${TARGET_NAME} PRIVATE
		-sENVIRONMENT=web
		-sEXPORT_ES6
		-sMODULARIZE
		-sUSE_HARFBUZZ

		-sINITIAL_MEMORY=536870912
		-sMAX_WEBGL_VERSION=2
		-lembind
		--emit-tsd ${TARGET_NAME}.d.ts
		--embed-file "${CMAKE_CURRENT_SOURCE_DIR}/assets@."
	)

	set_target_properties(${TARGET_NAME}
		PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/client/app/bin"
	)
endif()


add_subdirectory("third-party")
target_link_libraries(${TARGET_NAME} PRIVATE
	brotlidec brotlicommon
	woff2dec woff2common
)
target_include_directories(${TARGET_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/third-party/woff2/include"
)
