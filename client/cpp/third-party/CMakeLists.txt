
include (FetchContent)

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/brotli")
	message ("Downloading brotli source...")
	FetchContent_Declare (brotli
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/brotli"
		URL "https://github.com/google/brotli/archive/refs/tags/v1.1.0.tar.gz")
	FetchContent_MakeAvailable (brotli)
endif()

add_subdirectory ("brotli" EXCLUDE_FROM_ALL)
