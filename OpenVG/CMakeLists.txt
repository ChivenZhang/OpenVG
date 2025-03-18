cmake_minimum_required (VERSION 3.8)

add_definitions(
		# -DOPENVG_SHARED_LIBRARY
)

add_library (OpenVG STATIC
		Private/VGContext.cpp
		Private/VGElement.cpp
		Private/VGGradient.cpp
		Private/VGPainter.cpp
		Private/VGPicture.cpp
		Private/VGShape.cpp
		Private/VGTessellate.cpp
		Private/VGText.cpp
		Private/VGTrueType.cpp
		VG.h
		VGBuilder.h
		VGContext.h
		VGElement.h
		VGFactory.h
		VGGradient.h
		VGPainter.h
		VGPicture.h
		VGRender.h
		VGShape.h
		VGText.h
		VGTrueType.h
)

target_include_directories(OpenVG PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(OpenVG PUBLIC ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_directories(OpenVG PUBLIC ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib)
else()
	target_link_directories(OpenVG PUBLIC ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
endif()

find_package(Fontconfig REQUIRED) # since CMake 3.14
target_link_libraries(OpenVG PRIVATE Fontconfig::Fontconfig)

find_package(Freetype REQUIRED)
target_link_libraries(OpenVG PRIVATE Freetype::Freetype) # since CMake 3.10

find_package(harfbuzz CONFIG REQUIRED)
# HarfBuzz font subsetter
target_link_libraries(OpenVG PRIVATE harfbuzz::harfbuzz-subset)
# HarfBuzz text shaping library
target_link_libraries(OpenVG PRIVATE harfbuzz::harfbuzz)

target_link_libraries(OpenVG PRIVATE raqm fribidi icudt icuin icuio icuuc)

find_package(unofficial-skia CONFIG REQUIRED)
target_link_libraries(OpenVG PRIVATE unofficial::skia::skia)

# Similar for optional components, e.g. unofficial::skia::modules::skparagraph

# Clipper2
find_package(Clipper2 CONFIG REQUIRED)
target_link_libraries(OpenVG PRIVATE Clipper2::Clipper2)

# Clipper2Z
find_package(Clipper2 CONFIG REQUIRED)
target_link_libraries(OpenVG PRIVATE Clipper2::Clipper2Z)