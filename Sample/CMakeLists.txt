cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (Sample "main.cpp" "OpenUIRender.h" "OpenUIPainter.h" "OpenUIRender.cpp" "OpenUIPainter.cpp" "OpenVGRender.h" "OpenVGRender.cpp" "SDL3InputEnum.h" "SDL3InputEnum.cpp" "OpenVGPainter.h" "OpenVGPainter.cpp")
add_dependencies(Sample OpenUI OpenVG)

target_include_directories(Sample PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(Sample PRIVATE ${OPENUI_SOURCE_DIR})

find_package(GLEW REQUIRED)
target_link_libraries(Sample PRIVATE GLEW::GLEW)
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(Sample PRIVATE SDL3::SDL3)
find_package(Stb REQUIRED)
target_include_directories(Sample PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(Sample PRIVATE OpenUI OpenVG)