
include (FetchContent)

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/brotli")
	message ("Downloading brotli source...")
	FetchContent_Declare (brotli
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/brotli"
		URL "https://github.com/google/brotli/archive/refs/tags/v1.1.0.tar.gz")
	FetchContent_MakeAvailable (brotli)
endif()

add_subdirectory ("brotli" EXCLUDE_FROM_ALL)



if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/woff2")
	message ("Downloading woff2 source...")
	FetchContent_Declare (woff2
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/woff2"
		URL "https://github.com/google/woff2/archive/refs/tags/v1.0.2.tar.gz")
	FetchContent_MakeAvailable (woff2)
endif()

# Pass these variables to WOFF2
if (TARGET brotlidec)
    get_target_property(BROTLI_INCLUDE_DIRS brotlidec INTERFACE_INCLUDE_DIRECTORIES)
    set(BROTLIDEC_INCLUDE_DIRS ${BROTLI_INCLUDE_DIRS} CACHE STRING "Brotli include directories")
    set(BROTLIDEC_LIBRARIES brotlidec CACHE STRING "Brotli libraries")
endif()
if (TARGET brotlienc)
    get_target_property(BROTLI_INCLUDE_DIRS brotlienc INTERFACE_INCLUDE_DIRECTORIES)
    set(BROTLIENC_INCLUDE_DIRS ${BROTLI_INCLUDE_DIRS} CACHE STRING "Brotli include directories")
    set(BROTLIENC_LIBRARIES brotlienc CACHE STRING "Brotli libraries")
endif()

# Force woff2 to build static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

add_subdirectory ("woff2" EXCLUDE_FROM_ALL)
