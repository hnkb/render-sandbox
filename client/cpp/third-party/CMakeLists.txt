add_library (delabella STATIC "delabella/delabella.cpp")
target_include_directories(delabella PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/delabella")

file(GLOB LIBTESS2_SOURCES "libtess2-1.0.2/Source/*.c")
add_library (libtess2 STATIC ${LIBTESS2_SOURCES})
target_include_directories(libtess2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libtess2-1.0.2/Include")
