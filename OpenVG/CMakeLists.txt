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
	VGScene.h
	VGRender.h 
	VGBuilder.h 
	VGFactory.h
	Private/VgContext.cpp
	Private/VGElement.cpp
	Private/VGText.cpp
	Private/VGPicture.cpp
	Private/VGShape.cpp
)
target_include_directories(OpenVG PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})