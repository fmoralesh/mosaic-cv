/**
 * @file main.h
 * @brief Contais the main code for feature extraction and match comparison
 * @version 1.0
 * @date 20/01/2018
 * @author Victor Garcia
 * @title Main code
 */

#include "options.h"
#include "detector.h"
#include "preprocessing.h"

/// Constant definitios
#define TARGET_WIDTH	640       
#define TARGET_HEIGHT	480       

/// User namespaces
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

double t;

/*
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )  {

    double tot_matches=0, tot_good =0;
    int n_matches=0, n_good=0;
    int i=0, n_img=0;
    int nIter = 0;

    parser.Prog(argv[0]);

    vector<string> file_names;
    vector<DMatch> matches, good_matches;
    vector<KeyPoint> keypoints[2];
    Mat descriptors[2];
    Mat img[2];

    try{
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help){
        std::cout << parser;
        return 1;
    }
    catch (args::ParseError e){
        std::cerr << e.what() << std::endl;
        std::cerr << "Use -h, --help command to see usage" << std::endl;
        //std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e){
        std::cerr << "Bad imput commands" << std::endl;
        std::cerr << "Use -h, --help command to see usage" << std::endl;
        return 1;
    }
    
    int minHessian = 400;
    Ptr<Feature2D> detector;
    Ptr<DescriptorMatcher> matcher;

    // Create the desired feature extractor based on imput commands
    if(op_sift){
        detector = SIFT::create();
    }else if(op_surf){
        detector = SURF::create();
    }else if(op_orb){
        detector = ORB::create();
    }else if(op_kaze){
        detector = KAZE::create();
    }
    // Create the desired feature matcher based on imput commands
    if(op_flann){
        matcher = FlannBasedMatcher::create();
    }else if(op_brutef){
        matcher = BFMatcher::create();
    }

    // Two images as imput
    if (op_img){
        t = (double) getTickCount();
        // Check for two image flags and patchs (-i imageName)
        for(const auto img_name: args::get(op_img)){
            img[i++] = imread(img_name, IMREAD_COLOR);
            if( !img[i-1].data){
                cout<< " --(!) Error reading image "<< i << endl; 
                cerr << parser;
                return -1;
            }
            cout<< " -- Loaded image "<< img_name << endl;
            if(i<1){
                cout<< " -- Insuficient imput data "<< img_name << endl;
                std::cerr << "Use -h, --help command to see usage" << std::endl;
                return -1;
            }
            // Two images loaded successfully
        }
        // Resize the images to 640 x 480
        resize(img[0], img[0], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
        resize(img[1], img[1], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
        
        // Apply pre-processing algorithm if selected
        if(op_pre){
            colorChannelStretch(img[0], img[0], 1, 99);
            colorChannelStretch(img[1], img[1], 1, 99);
        }
        // Conver images to gray
        cvtColor(img[0],img[0],COLOR_BGR2GRAY);
        cvtColor(img[1],img[1],COLOR_BGR2GRAY);

        // Detect the keypoints using desired Detector and compute the descriptors
        detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
        detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
        // Flann matcher needs the descriptors to be of type CV_32F
        descriptors[0].convertTo(descriptors[0], CV_32F);
        descriptors[1].convertTo(descriptors[1], CV_32F);

        // Match the keypoints for input images
        matcher->match( descriptors[0], descriptors[1], matches );
        
        n_matches = descriptors[0].rows;
        // Discard the bad matches (outliers)
        good_matches = getGoodMatches(n_matches, matches);
        n_good = good_matches.size();

        cout << endl;
        cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
        cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
        cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;
        t = 1000 * ((double) getTickCount() - t) / getTickFrequency();        
        cout << "   Execution time: " << t << " ms" <<endl;
        // for output command ( -o )
        if(op_out){
            Mat img_matches;
            // Draw only "good" matches
            drawMatches( img[0], keypoints[0], img[1], keypoints[1],
                        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
            // Show matches
            namedWindow("Good Matches", WINDOW_NORMAL);
            imshow( "Good Matches", img_matches );
            waitKey(0);
        }
        return 1;
    }
    // video file as imput
    if(op_vid){
        VideoCapture vid;
        vid.open(args::get(op_vid));
        
        if(!vid.isOpened()){
            // Error openning the video
            cout << "Couldn't open Video " << endl;
            return -1;
        }
        // Get the video paramenters. frames per second and frames number
        double fps = vid.get(CAP_PROP_FPS);
        double fcnt = vid.get(CAP_PROP_FRAME_COUNT);
        cout << "Video opened \nFrames per second: "<< fps << "\nFrames in video:   "<<fcnt<< endl;

        t = (double) getTickCount();
        for(i=0; i<fcnt; i+=fps){
            vid.set(CAP_PROP_POS_FRAMES,i);
            vid >> img[0];
            vid.set(CAP_PROP_POS_FRAMES,i+=fps);
            vid >> img[1];

            // Resize the images to 640 x 480
            resize(img[0], img[0], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
            resize(img[1], img[1], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
            
            // Apply pre-processing algorithm if selected
            if(op_pre){
                colorChannelStretch(img[0], img[0], 1, 99);
                colorChannelStretch(img[1], img[1], 1, 99);
            }
            // Conver images to gray
            cvtColor(img[0],img[0],COLOR_BGR2GRAY);
            cvtColor(img[1],img[1],COLOR_BGR2GRAY);
            // imshow("TEST",img[0]);
            // waitKey(0);
            
            // Detect the keypoints using desired Detector and compute the descriptors
            detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
            detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
            // Flann matcher needs the descriptors to be of type CV_32F
            descriptors[0].convertTo(descriptors[0], CV_32F);
            descriptors[1].convertTo(descriptors[1], CV_32F);

            // Match the keypoints for input images
            matcher->match( descriptors[0], descriptors[1], matches );

            n_matches = descriptors[0].rows;
            // Quick calculation of max and min distances between keypoints
            good_matches = getGoodMatches(n_matches, matches);
            n_good = good_matches.size();
            cout << "Pair  "<< n_img++; <<" -- -- -- -- -- -- -- -- -- --"  << endl;
            cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
            cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
            cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;

            tot_matches+=n_matches;
            tot_good+=n_good;
            img[0].release();
            img[1].release();
            good_matches.erase(good_matches.begin(), good_matches.end());
        }
        cout << "\nTotal "<< n_img++ <<" -- -- -- -- -- -- -- -- -- --"  << endl;
        cout << "-- Total Possible matches  ["<< tot_matches <<"]"  << endl;
        cout << "-- Total Good Matches      ["<< tot_good <<"]"  << endl;
        cout << "-- Total Accuracy  ["<< (int)(tot_good*100/tot_matches) <<" %]"  << endl;
        //high_resolution_clock::time_point t2 = high_resolution_clock::now();
        //auto duration = duration_cast<microseconds>( t2 - t1 ).count()/1000;
        t = 1000 * ((double) getTickCount() - t) / getTickFrequency();        
        cout << "   Execution time: " << t << " ms" <<endl;
        vid.release();
    }
    if(op_dir){
        string dir_ent = args::get(op_dir);
        vector<string> file_names = read_filenames(dir_ent);
        cout << file_names[0] << endl;
        t = (double) getTickCount();
        for(int i=0; i<file_names.size()-1; i+=2){
            img[0] = imread(dir_ent+"/"+file_names.at(i),1);
            img[1] = imread(dir_ent+"/"+file_names.at(i+1),1);
        
            // Resize the images to 640 x 480
            resize(img[0], img[0], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
            resize(img[1], img[1], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
            
            // Apply pre-processing algorithm if selected
            if(op_pre){
                colorChannelStretch(img[0], img[0], 1, 99);
                colorChannelStretch(img[1], img[1], 1, 99);
            }
            // Conver images to gray
            cvtColor(img[0],img[0],COLOR_BGR2GRAY);
            cvtColor(img[1],img[1],COLOR_BGR2GRAY);

            // Detect the keypoints using desired Detector and compute the descriptors
            detector->detectAndCompute( img[0], Mat(), keypoints[0], descriptors[0] );
            detector->detectAndCompute( img[1], Mat(), keypoints[1], descriptors[1] );
            // Flann matcher needs the descriptors to be of type CV_32F
            descriptors[0].convertTo(descriptors[0], CV_32F);
            descriptors[1].convertTo(descriptors[1], CV_32F);

            // Match the keypoints for input images
            matcher->match( descriptors[0], descriptors[1], matches );

            n_matches = descriptors[0].rows;
            // Quick calculation of max and min distances between keypoints
            good_matches = getGoodMatches(n_matches, matches);
            n_good = good_matches.size();
            
            tot_matches+=n_matches;
            tot_good+=n_good;
            cout << "Pair  "<< n_img++ <<" -- -- -- -- -- -- -- -- -- --"  << endl;
            cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
            cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
            cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;
        }
        cout << "\nTotal pairs "<< n_img <<" -- -- -- -- -- -- -- -- -- --"  << endl;
        cout << "-- Total Possible matches  ["<< tot_matches <<"]"  << endl;
        cout << "-- Total Good Matches      ["<< tot_good <<"]"  << endl;
        cout << "-- Total Accuracy  ["<< (int)(tot_good*100/tot_matches) <<" %]"  << endl;
        t = 1000 * ((double) getTickCount() - t) / getTickFrequency();        
        cout << "   Execution time: " << t << " ms" <<endl;
    }
    return 0;
}



