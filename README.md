# mosaic-generation
Monocular video based Mosaic Generation System for mobile robots implemented in OpenCV 3.2.

## Requirements
OpenCV 3.0

## How to compile?
Provided with this repo is a CMakeLists.txt file, which you can use to directly compile the code as follows:
```bash
mkdir build
cd build
cmake ..
make
```
## How to run?
After compilation, run the program with the following sintax:
```bash
./mosaic [IMPUT DATA] --DETECTOR -MATCHER
```
Type the following command to see correct usage:
```bash
./mosaic -h
```
### Usage examples:
```bash
./mosaic -i image1.jpg -i image2.jpg --sift -f -o
```
Detect and compute the features between two input images with Sift detector and flann matcher, aditionally shows the matches points.

```bash
./mosaic -v video.mp4 --sift -b
```
Detect and compute the features between all the pairs of images generated from a video file, the selection method is fixed to one frame per second. Sift detector and BruteForce matcher are used.
