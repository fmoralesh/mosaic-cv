/********************************************
 * FILE NAME: options.h                     *
 * DESCRIPTION: Contains the main code for  *
 *              feature match comparison    *
 *              images                      *
 * AUTHOR: Victor García                    *
 ********************************************/

#include <iostream>
#include <iostream>
#include <chrono>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/opencv.hpp"
#include "options.h"
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std::chrono;
/*
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )  {

    int tot_matches;
    Mat img_1, img_2;
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    vector< DMatch > matches;
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;
    try{
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help){
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e){
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e){
        std::cerr << "Select at least one Feature Extractor and one Matcher" << std::endl;
        std::cerr << parser;
        return 1;
    }
    if(op_img1){
        img_1 = imread( args::get(op_img1), IMREAD_GRAYSCALE );
        if( !img_1.data){
            cout<< " --(!) Error reading image 1 " << endl; 
            cerr << parser;
            return -1;
        }
    }
    if(op_img2){
        img_2 = imread( args::get(op_img2), IMREAD_GRAYSCALE );
        if( !img_2.data){
            cout<< " --(!) Error reading image 1 " << endl; 
            cerr << parser;
            return -1;
        }
    }
    if(op_vid){
        VideoCapture vid(args::get(op_vid)); 
        if(!vid.isOpened()){
            cout << "Couldn't open Video " << endl;
            return -1;
        }
        double fps = vid.get(CAP_PROP_FPS);
        cout << "Video opened \nFrames per second: "<< fps << endl;
        namedWindow("gray",1);
        for(;;){
            Mat frame, gray;
            vid >> frame; // get a new frame from camera
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            imshow("gray", gray);
            //if(waitKey(30)) break;
        }
        vid.release();
        return 1; 
    }

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    if(op_sift){
        // Detect the keypoints using SIFT Detector, compute the descriptors
        Ptr<SIFT> detector = SIFT::create();
        //detector = SIFT::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    if(op_surf){
        // Detect the keypoints using SUFR Detector, compute the descriptors
        int minHessian = 400;
        Ptr<SURF> detector = SURF::create();
        detector->setHessianThreshold(minHessian);
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    if(op_orb){
        // Detect the keypoints using ORB Detector, compute the descriptors
        Ptr<ORB> detector = ORB::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

        // Flann needs the descriptors to be of type CV_32F
        descriptors_1.convertTo(descriptors_1, CV_32F);
        descriptors_2.convertTo(descriptors_2, CV_32F);
    }
    if(op_kaze){
        // Detect the keypoints using ORB Detector, compute the descriptors
        Ptr<KAZE> detector = KAZE::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    // Step 2: Matching descriptor vectors using FLANN matcher

    if(op_brutef){
        BFMatcher matcher;
        matcher.match( descriptors_1, descriptors_2, matches );
    }
    if(op_flann){
        FlannBasedMatcher matcher;
        matcher.match( descriptors_1, descriptors_2, matches );
    }

    tot_matches = descriptors_1.rows;

    double max_dist = 0; double min_dist = 100;
    // Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ ){
        double dist = matches[i].distance;
        if( dist < min_dist )
            min_dist = dist;
        if( dist > max_dist )
            max_dist = dist;
    }
    // Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    // or a small arbitary value ( 0.02 ) in the event that min_dist is very
    // small)
    vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ ){
        if( matches[i].distance <= max(3*min_dist, 0.02) ){
            good_matches.push_back( matches[i]);
        }
    }
    //-- Draw only "good" matches
    Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    // Show detected matches
    namedWindow("Good Matches", WINDOW_NORMAL);
    imshow( "Good Matches", img_matches );

    int i = (int)good_matches.size();
    cout << "-- Total Features detected ["<< tot_matches <<"]"  << endl;
    cout << "-- Number of Good Matches  ["<< i <<"]"  << endl;
    cout << "-- Accuracy  ["<< i*100/tot_matches <<" %]"  << endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count()/1000;
    cout << "   Execution time: " << duration << " ms" <<endl;
    waitKey(0);
    return 0;
}