#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include <dirent.h>
#include <iostream>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

void sift_detector(cv::Mat* img, cv::Mat* descriptors, std::vector<cv::KeyPoint>* keypoints);

void surf_detector(cv::Mat* img, cv::Mat* descriptors, std::vector<cv::KeyPoint>* keypoints);

void orb_detector(cv::Mat* img, cv::Mat* descriptors, std::vector<cv::KeyPoint>* keypoints);

void kaze_detector(cv::Mat* img, cv::Mat* descriptors, std::vector<cv::KeyPoint>* keypoints);

std::vector<cv::DMatch> getGoodMatches(int n_matches, std::vector<cv::DMatch> matches);

std::vector<std::string>read_filenames(std::string dir_ent);

#endif