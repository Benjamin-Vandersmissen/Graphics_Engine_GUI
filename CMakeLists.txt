cmake_minimum_required(VERSION 3.6)
project(Graphics_Engine_GUI)

set(CMAKE_CXX_STANDARD 11)

include_directories(FLTK)
link_directories(usr/local/lib)

set(SOURCE_FILES main.cpp GUI.cpp GUI.h)
add_executable(Graphics_Engine_GUI ${SOURCE_FILES})
target_link_libraries(Graphics_Engine_GUI fltk_images fltk X11 Xext pthread Xfixes dl m)