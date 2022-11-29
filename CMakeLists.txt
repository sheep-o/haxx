cmake_minimum_required(VERSION 3.23)
project(hax)

set(CMAKE_CXX_STANDARD 23)

include_directories($ENV{DXSDK_DIR}/Include)

file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.cpp" "imgui/*.h" "imgui/*.cpp")

add_library(hax SHARED ${SRC} IClientEntityList.h Entity.cpp Entity.h)
target_link_libraries(hax $ENV{DXSDK_DIR}/Lib/x86/d3d9.lib)
target_link_libraries(hax $ENV{DXSDK_DIR}/Lib/x86/d3dx9.lib)