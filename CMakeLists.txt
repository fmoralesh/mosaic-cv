cmake_minimum_required(VERSION 2.8)
project( mosaic-opencv )
find_package( OpenCV 3.0 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB mosaic-cv
    "*.h"
    "*.hxx"
    "*.cpp"
)
add_executable( mosaic ${mosaic-cv} )
target_compile_options(mosaic PUBLIC -std=c++11)
target_link_libraries( mosaic ${OpenCV_LIBS} )

