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

if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(OpenVG PRIVATE
		bentleyottmann
		brotlicommon
		brotlidec
		brotlienc
		bz2d
		charset
		fontconfig
		freetyped
		fribidi
		getopt
		GlU32
		harfbuzz
		harfbuzz-subset
		iconv
		icudtd
		icuind
		icuiod
		icuucd
		intl
		jpeg
		libexpatdMD
		libpng16d
		libsharpyuv
		libwebp
		libwebpdecoder
		libwebpdemux
		libwebpmux
		OpenGL32
		pthreadVC3d
		pthreadVCE3d
		pthreadVSE3d
		raqm
		skia
		skottie
		skparagraph
		sksg
		skshaper
		skunicode_core
		skunicode_icu
		svg
		turbojpeg
		zlibd
	)
else()
        target_link_libraries(OpenVG PRIVATE
		bentleyottmann
		brotlicommon
		brotlidec
		brotlienc
		bz2
		charset
		fontconfig
		freetype
		fribidi
		getopt
		GlU32
		harfbuzz
		harfbuzz-subset
		iconv
		icudt
		icuin
		icuio
		icuuc
		intl
		jpeg
		libexpatMD
		libpng16
		libsharpyuv
		libwebp
		libwebpdecoder
		libwebpdemux
		libwebpmux
		OpenGL32
		pthreadVC3
		pthreadVCE3
		pthreadVSE3
		raqm
		skia
		skottie
		skparagraph
		sksg
		skshaper
		skunicode_core
		skunicode_icu
		svg
		turbojpeg
		zlib
	)
endif()

