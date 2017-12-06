#include "detector.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void sift_detector(Mat img[2], Mat descriptors[2], vector<KeyPoint> keypoints[2]){
    // Detect the keypoints using SIFT Detector, compute the descriptors
    Ptr<SIFT> detector = SIFT::create();

    detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
    detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
}

void surf_detector(Mat img[2], Mat descriptors[2], vector<KeyPoint> keypoints[2]){
    // Detect the keypoints using SUFR Detector, compute the descriptors
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create();
    detector->setHessianThreshold(minHessian);
    detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
    detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
}

void orb_detector(Mat img[2], Mat descriptors[2], vector<KeyPoint> keypoints[2]){
    // Detect the keypoints using ORB Detector, compute the descriptors
    Ptr<ORB> detector = ORB::create();
    detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
    detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );

    // Flann needs the descriptors to be of type CV_32F
    descriptors[0].convertTo(descriptors[0], CV_32F);
    descriptors[1].convertTo(descriptors[1], CV_32F);
}

void kaze_detector(Mat img[2], Mat descriptors[2], vector<KeyPoint> keypoints[2]){
    // Detect the keypoints using ORB Detector, compute the descriptors
    Ptr<KAZE> detector = KAZE::create();
    detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
    detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
}

vector<DMatch> getGoodMatches(int n_matches, vector<DMatch> matches){
    vector<DMatch> good_matches;

    double max_dist = 0; double min_dist = 100;
    for( int i = 0; i < n_matches; i++ ){
        double dist = matches[i].distance;
        if( dist < min_dist )
            min_dist = dist;
        if( dist > max_dist )
            max_dist = dist;
    }
    for( int i = 0; i < n_matches; i++ ){
        if( matches[i].distance <= max(2*min_dist, 0.02) ){
            good_matches.push_back( matches[i]);
        }
    }

    return good_matches;
}

vector<string> read_filenames(string dir_ent){
    vector<string> file_names;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(dir_ent.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            file_names.push_back(string(ent->d_name));
        }
        closedir (dir);
    } else {
    // If the directory could not be opened
    cout << "Directory could not be opened" <<endl;
    }
    // Sorting the vector of strings so it is alphabetically ordered
    sort(file_names.begin(), file_names.end());
    file_names.erase(file_names.begin(), file_names.begin()+2);

    return file_names;
}