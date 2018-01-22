/**
 * @file detector.h
 * @brief Independet Functions
 * @version 1.0
 * @date 20/01/2018
 * @author Victor Garcia
 */

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

/**
 * @function getGoodMatches(int n_matches, std::vector<cv::DMatch> matches)
 * @brief Discard the matchs outliers
 * @param n_matches Number of matches
 * @param matches Vector container all the Opencv Matches
 * @return vector<cv::DMatch> Vector container good Opencv Matches
 */
std::vector<cv::DMatch> getGoodMatches(const std::vector<cv::DMatch> matches);

/**
 * @function read_filenames(std::string dir_ent)
 * @brief Get and store the name of files from a directory
 * @param dir_ent Path of the directory to read the file names
 * @return vector<std::string> Vector container the names (sorted alphabetically) of files in the directory 
 */
std::vector<std::string> read_filenames(const std::string dir_ent);

/**
 * @function gridDetector(cv::Mat src, std::vector<KeyPoint> keypoint, cv::Mat Descriptor)
 * @brief 
 * @param src 
 * @param keypoint 
 * @param Descriptor 
 */
void gridDetector(const cv::Mat src[2], cv::Feature2D* detector, std::vector<cv::KeyPoint> keypoints[2], cv::Mat descriptors[2]);

#endif