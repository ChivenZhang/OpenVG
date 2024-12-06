cmake_minimum_required (VERSION 3.8)

add_definitions(
	-DOPENVG_SHARED_LIBRARY
)

add_library (OpenVG SHARED 
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
target_include_directories(OpenVG PRIVATE ${OPENVG_SOURCE_DIR}/External/x64-windows-static-md/include)
target_include_directories(OpenVG PRIVATE ${OPENVG_SOURCE_DIR}/External/x64-windows-static-md/include/skia)
if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_directories(OpenVG PRIVATE ${OPENVG_SOURCE_DIR}/External/x64-windows-static-md/debug/lib)
else()
        target_link_directories(OpenVG PRIVATE ${OPENVG_SOURCE_DIR}/External/x64-windows-static-md/lib)
endif()
target_link_libraries(OpenVG skia)