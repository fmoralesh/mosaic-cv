#include <iostream>
#include <iostream>
#include <args.hxx>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

/*
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )  {
    // Argument Parser flags
    args::ArgumentParser parser("Feature Detectors comparison", "Authors: Victor Garcia");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Positional<std::string> img1(parser, "IMAGE_1", "Imagen_1.jpg");
    args::Positional<std::string> img2(parser, "IMAGE_2", "imagen_2.jpg");
    args::Group feature(parser, "Select the Feature Detector and Descriptor:", args::Group::Validators::AtMostOne);
    args::Flag sift(feature, "SIFT", "Use SIFT Detector and Descriptor", {"sift"});
    args::Flag surf(feature, "SURF", "Use SURF Detector and Descriptor", {"surf"});
    args::Flag orb(feature, "ORB", "Use SIFT Detector and Descriptor", {"orb"});
    args::Flag kaze(feature, "KAZE", "Use SURF Detector and Descriptor", {"kaze"});

    parser.LongSeparator(" ");
    Mat img_1, img_2;
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

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
        std::cerr << "Select only one Feature Extractor" << std::endl;
        std::cerr << parser;
        return 1;
    }
    if(img1){
        img_1 = imread( args::get(img1), IMREAD_GRAYSCALE );
        if( !img_1.data){
            cout<< " --(!) Error reading image 1 " << std::endl; 
            std::cerr << parser;
            return -1;
        }
    }
    if(img2){
        img_2 = imread( args::get(img2), IMREAD_GRAYSCALE );
        if( !img_2.data){
            cout<< " --(!) Error reading image 1 " << std::endl; 
            std::cerr << parser;
            return -1;
        }
    }
    if(sift){
        // Detect the keypoints using SIFT Detector, compute the descriptors
        Ptr<SIFT> detector = SIFT::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    if(surf){
        // Detect the keypoints using SUFR Detector, compute the descriptors
        int minHessian = 400;
        Ptr<SURF> detector = SURF::create();
        //detector->setHessianThreshold(minHessian);
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    if(orb){
        // Detect the keypoints using ORB Detector, compute the descriptors
        Ptr<ORB> detector = ORB::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

        // Flann needs the descriptors to be of type CV_32F
        descriptors_1.convertTo(descriptors_1, CV_32F);
        descriptors_2.convertTo(descriptors_2, CV_32F);
    }
    if(kaze){
        // Detect the keypoints using ORB Detector, compute the descriptors
        Ptr<KAZE> detector = KAZE::create();
        detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
        detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
    }
    //-- Step 2: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );
    double max_dist = 0; double min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
    }
    //printf("-- Max dist : %f \n", max_dist );
    //printf("-- Min dist : %f \n", min_dist );
    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
    //-- small)
    //-- PS.- radiusMatch can also be used here.
    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ )
    { if( matches[i].distance <= max(2*min_dist, 0.02) )
    { good_matches.push_back( matches[i]); }
    }
    //-- Draw only "good" matches
    Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    namedWindow("Good Matches", WINDOW_NORMAL);
    imshow( "Good Matches", img_matches );

    int i = (int)good_matches.size();
    cout << "-- Number of Good Matches ["<< i <<"]"  << endl;

    waitKey(0);
    return 0;
}