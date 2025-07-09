cmake_minimum_required(VERSION 3.15)
project(RGS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(HEADERS
	"src/Application.h"
 "src/Window.h" "src/WindowsWindow.h" "src/Base.h" "src/InputCodes.h" "src/Framebuffer.h" "src/Math.h")

set(SOURCES
	"src/Main.cpp"	
	"src/Application.cpp"
 "src/Window.cpp" "src/WindowsWindow.cpp" "src/Framebuffer.cpp" "src/Math.cpp")

set (TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})