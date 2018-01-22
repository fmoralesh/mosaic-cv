#include "detector.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

// See description in header file
std::vector<DMatch> getGoodMatches(int n_kp, std::vector<std::vector<cv::DMatch> > matches){
    vector<DMatch> good_matches;
    // int n_matches = matches.size();
    // double min_dist = 100;
    // // get the minimun distance, corresponding to the strongest matches
    // for( int i = 0; i < n_matches; i++ ){
    //     double dist = matches[i].distance;
    //     if( dist < min_dist )
    //         min_dist = dist;
    // }
    // // Keep with matches with similar distance to the best one
    // for( int i = 0; i < n_matches; i++ ){
    //     if( matches[i].distance <= max(2*min_dist, 0.02) ){
    //         good_matches.push_back( matches[i]);
    //     }
    // }
    for (int i = 0; i < std::min(n_kp, (int)matches.size()); i++) {
        if ((matches[i][0].distance < 0.6 * (matches[i][1].distance)) &&
            ((int) matches[i].size() <= 2 && (int) matches[i].size() > 0)) {
            // take the first result only if its distance is smaller than 0.6*second_best_dist
            // that means this descriptor is ignored if the second distance is bigger or of similar
            good_matches.push_back(matches[i][0]);
        }
    }
    return good_matches;
}


// See description in header file
std::vector<string> read_filenames(const std::string dir_ent){
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


// See description in header file
void gridDetector(const Mat src[2], Feature2D* detector, vector<KeyPoint> keypoints[2], Mat descriptors[2]){
    int stepx=102, stepy=76;
    Rect roi(0, 0, stepx, stepy);
    vector<KeyPoint> aux_keypoint;
    Mat aux_descriptor(0, detector->descriptorSize(), detector->descriptorType());
    // Define the number of columns based on descriptor type
    descriptors[0].create(0, detector->descriptorSize(), detector->descriptorType());

    // Detect keypoints in first image in sections of size stepx * stepy
    // number of iterations fixed to 10x10 -> 10*64 x 10*48 = TARGET_WIDHT x TARGET_HEIGHT
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            // move the region of interest to the next section
            roi.x = stepx*i;
            roi.y = stepy*j;
            detector->detectAndCompute( src[0](roi), Mat(), aux_keypoint, aux_descriptor);
            // save the new descriptors in the original descriptors matrix
            vconcat(descriptors[0], aux_descriptor, descriptors[0]);
            // save the new keypoints in the original Keypoints vector
            for(auto kp: aux_keypoint){
                kp.pt.x += stepx*i;
                kp.pt.y += stepy*j;
                keypoints[0].push_back(kp);
            }
        }
    }
    // Compute the keypoints and descriptors of the second image
    detector->detectAndCompute( src[1], Mat(), keypoints[1], descriptors[1]);
}