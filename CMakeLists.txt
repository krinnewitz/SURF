cmake_minimum_required (VERSION 2.6)
project (SURF) 

FIND_PACKAGE( OpenCV REQUIRED )

add_executable (surf Main.cpp) 

TARGET_LINK_LIBRARIES( surf ${OpenCV_LIBS} )

