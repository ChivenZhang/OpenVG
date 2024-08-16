cmake_minimum_required (VERSION 3.8)

add_executable (Sample "main.cpp")
add_dependencies(Sample OpenVG)
