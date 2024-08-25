cmake_minimum_required (VERSION 3.8)

add_definitions(
	-DOPENVG_SHARED_LIBRARY
)

add_library (OpenVG SHARED 
	VG.h
	VGContext.h
	VGElement.h
	VGShape.h
	VGPicture.h
	VGText.h 
	VGRender.h 
	VGBuilder.h 
	VGFactory.h
	VGPainter.h
	VGGradient.h
	VGStroke.h
	Private/VGContext.cpp
	Private/VGElement.cpp
	Private/VGShape.cpp
	Private/VGText.cpp
	Private/VGPicture.cpp
	Private/VGPainter.cpp
	Private/VGGradient.cpp
	Private/VGStroke.cpp
	Private/VGTessellate.cpp
	Private/VGTessellate.h
	Private/VGTrueType.cpp
	Private/VGTrueType.h
 "Private/VGPath.h" "Private/VGPath.cpp")

target_include_directories(OpenVG PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(OpenVG libtess2 micro-tess)