cmake_minimum_required (VERSION 3.8)

add_executable (Sample "main.cpp")
add_dependencies(Sample OpenVG)

target_include_directories(Sample PRIVATE ${CMAKE_SOURCE_DIR}/OpenVG)
target_link_libraries(Sample PRIVATE OpenVG)