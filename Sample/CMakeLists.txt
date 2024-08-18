cmake_minimum_required (VERSION 3.8)

include_directories(${CMAKE_SOURCE_DIR}/External/OpenUI/External)
include_directories(${CMAKE_SOURCE_DIR}/External/OpenUI/External/stb)
include_directories(${CMAKE_SOURCE_DIR}/External/OpenUI/External/x64-windows/include)
if(CMAKE_BUILD_TYPE MATCHES Debug)
link_directories(${CMAKE_SOURCE_DIR}/External/OpenUI/External/x64-windows/debug/lib)
else()
link_directories(${CMAKE_SOURCE_DIR}/External/OpenUI/External/x64-windows/lib)
endif()

add_executable (Sample "main.cpp" "OpenUIRender.h" "OpenUIPainter.h" "OpenUIRender.cpp" "OpenUIPainter.cpp" "OpenVGRender.h" "OpenVGRender.cpp")

target_include_directories(Sample PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(Sample PRIVATE ${CMAKE_SOURCE_DIR}/External/OpenUI)

target_link_libraries(Sample 
	debug SDL3
	debug GlU32 
	debug glew32d
	debug OpenGL32 
	debug OpenUI
	debug OpenVG
)

target_link_libraries(Sample 
	optimized SDL3
	optimized GlU32 
	optimized glew32
	optimized OpenGL32
	optimized OpenUI
	optimized OpenVG
)

add_custom_command(  
    TARGET Sample POST_BUILD  
    COMMAND "${CMAKE_SOURCE_DIR}/INSTALL.bat"
    COMMENT "Copying dll to output directory"  
)