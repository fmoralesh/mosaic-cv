/**
 * @file stitch.h
 * @brief Independet Functions
 * @version 1.0
 * @date 20/01/2018
 * @author Victor Garcia
 */

#ifndef STITCH_H
#define STITCH_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define TARGET_WIDTH	640   
#define TARGET_HEIGHT	480

cv::Rect getBound(cv::Mat H, int width, int height);

cv::Mat translateImg(cv::Mat &img, int offsetx, int offsety);

#endif