cmake_minimum_required(VERSION 2.8)
project( mosaic-opencv )
find_package( OpenCV 3.0 REQUIRED xfeatures2d)

include_directories( ${OpenCV_INCLUDE_DIRS} )
file(GLOB mosaic-cv/src
    "*.h"
    "*.hxx"
    "*.cpp"
)
add_executable( fcomp ${mosaic-cv} )
target_compile_options(fcomp PUBLIC -std=c++11)
target_link_libraries( fcomp ${OpenCV_LIBS} )

