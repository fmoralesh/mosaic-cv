#include "detector.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

vector<DMatch> getGoodMatches(int n_matches, vector<DMatch> matches){
    vector<DMatch> good_matches;

    double max_dist = 0, min_dist = 100;
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