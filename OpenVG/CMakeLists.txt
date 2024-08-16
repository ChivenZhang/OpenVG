cmake_minimum_required (VERSION 3.8)

add_library (OpenVG SHARED 
	Private/VgContext.cpp
	VG.h
	VGContext.h
)
target_include_directories(OpenVG PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})