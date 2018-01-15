/********************************************
 * FILE NAME: main.cpp                     *
 * DESCRIPTION: Contains the main code for  *
 *              feature match comparison    *
 *              images                      *
 * AUTHOR: Victor García                    *
 ********************************************/

#include <chrono>
#include "options.h"
#include "detector.h"
#include "preprocessing.h"

/// Constant definitios
#define TARGET_WIDTH	640        //< Resized image width
#define TARGET_HEIGHT	480        //< Resized image height

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std::chrono;

double t;
/*
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )  {

    int n_matches=0, n_good=0;
    double tot_matches=0, tot_good =0;
    int i=0, n_img=0;

    parser.Prog(argv[0]);

    vector<string> file_names;
    vector<DMatch> matches, good_matches;
    vector<KeyPoint> keypoints[2];
    Mat descriptors[2];
    Mat img[2];
    string vid_dir = "./video/";
    string img_dir = "./image/";

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
        std::cerr << "Bad imput commands" << std::endl;
        std::cerr << "Use -h, --help command to see usage" << std::endl;
        //std::cerr << parser;
        return 1;
    }
    if (op_img){
        for(const auto img_name: args::get(op_img)){
            img[i++] = imread(img_name, IMREAD_GRAYSCALE);
            if( !img[i-1].data){
                cout<< " --(!) Error reading image "<< i << endl; 
                cerr << parser;
                return -1;
            }
            cout<< " -- Loaded image "<< img_name << endl;
            if(i<1){
                cout<< " -- Insuficient imput data "<< img_name << endl;
                std::cerr << "Use -h, --help command to see usage" << std::endl;
                return 1;
            }
        }
        resize(img[0], img[0], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
        resize(img[1], img[1], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);

        if(op_sift){
            sift_detector(img, descriptors, keypoints);
        }else if(op_surf){
            surf_detector(img, descriptors, keypoints);
        }else if(op_orb){
            orb_detector(img, descriptors, keypoints);
        }else if(op_kaze){
            kaze_detector(img, descriptors, keypoints);
        }

        if(op_brutef){
            BFMatcher matcher;
            matcher.match( descriptors[0], descriptors[1], matches );
        }else if(op_flann){
            FlannBasedMatcher matcher;
            matcher.match( descriptors[0], descriptors[1], matches );
        }
        n_matches = descriptors[0].rows;
        // Quick calculation of max and min distances between keypoints
        good_matches = getGoodMatches(n_matches, matches);
        n_good = good_matches.size();
        cout << endl;
        cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
        cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
        cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;
        if(op_out){
            Mat img_matches;
            // Draw only "good" matches
            drawMatches( img[0], keypoints[0], img[1], keypoints[1],
                        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
            // Show detected matches
            namedWindow("Good Matches", WINDOW_NORMAL);
            imshow( "Good Matches", img_matches );
            waitKey(0);
        }
        return 1;
    }
    if(op_split || op_vid){
        VideoCapture vid;
        if(op_split)
            vid.open(args::get(op_split));
        else
            vid.open(args::get(op_vid));
        
        if(!vid.isOpened()){
            cout << "Couldn't open Video " << endl;
            return -1;
        }
            
        double fps = vid.get(CAP_PROP_FPS);
        double fcnt = vid.get(CAP_PROP_FRAME_COUNT);
        cout << "Video opened \nFrames per second: "<< fps << "\nFrames in video:   "<<fcnt<< endl;
        int n=0;
        //high_resolution_clock::time_point t1 = high_resolution_clock::now();
        t = (double) getTickCount();
        for(i=0; i<fcnt; i+=fps){
            vid.set(CAP_PROP_POS_FRAMES,i);
            vid >> img[0];

            resize(img[0], img[0], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
            if(op_pre){
                colorChannelStretch(img[0], img[0], 1, 99);
            }
            
            cvtColor(img[0],img[0],COLOR_BGR2GRAY);
            // imshow("TEST",img[0]);
            // waitKey(0);
            if(op_split){
                imwrite("./video/frame_"+to_string(n_img/10)+to_string(n_img%10)+".jpg", img[0]);
            }else{
                vid.set(CAP_PROP_POS_FRAMES,i+=fps);
                vid >> img[1];
                resize(img[1], img[1], Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, CV_INTER_LINEAR);
                if(op_pre){
                    colorChannelStretch(img[1], img[1], 1, 99);
                }
                cvtColor(img[1],img[1],COLOR_BGR2GRAY);

                if(op_sift){
                    sift_detector(img, descriptors, keypoints);
                }else if(op_surf){
                    surf_detector(img, descriptors, keypoints);
                }else if(op_orb){
                    orb_detector(img, descriptors, keypoints);
                }else if(op_kaze){
                    kaze_detector(img, descriptors, keypoints);
                }

                if(op_brutef){
                    BFMatcher matcher;
                    matcher.match( descriptors[0], descriptors[1], matches );
                }else if(op_flann){
                    FlannBasedMatcher matcher;
                    matcher.match( descriptors[0], descriptors[1], matches );
                }
                n_matches = descriptors[0].rows;
                // Quick calculation of max and min distances between keypoints
                good_matches = getGoodMatches(n_matches, matches);
                n_good = good_matches.size();
                cout << "Pair  "<< n_img+1 <<" -- -- -- -- -- -- -- -- -- --"  << endl;
                cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
                cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
                cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;
            }
            n_img++;
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
        for(int i=0; i<file_names.size()-1; i+=2){
            img[0] = imread(dir_ent+"/"+file_names.at(i),1);
            img[1] = imread(dir_ent+"/"+file_names.at(i+1),1);

            cvtColor(img[0],img[0],COLOR_RGB2GRAY);
            cvtColor(img[1],img[1],COLOR_RGB2GRAY);

            if(op_sift){
                sift_detector(img, descriptors, keypoints);
            }else if(op_surf){
                surf_detector(img, descriptors, keypoints);
            }else if(op_orb){
                orb_detector(img, descriptors, keypoints);
            }else if(op_kaze){
                kaze_detector(img, descriptors, keypoints);
            }

            if(op_brutef){
                BFMatcher matcher;
                matcher.match( descriptors[0], descriptors[1], matches );
            }else if(op_flann){
                FlannBasedMatcher matcher;
                matcher.match( descriptors[0], descriptors[1], matches );
            }
            n_matches = descriptors[0].rows;
            // Quick calculation of max and min distances between keypoints
            good_matches = getGoodMatches(n_matches, matches);
            n_good = good_matches.size();
            cout << "Pair  "<< ++i <<" -- -- -- -- -- -- -- -- -- --"  << endl;
            cout << "-- Possible matches  ["<< n_matches <<"]"  << endl;
            cout << "-- Good Matches      ["<< n_good <<"]"  << endl;
            cout << "-- Accuracy  ["<< n_good*100/n_matches <<" %]"  << endl;
        }
        cout << "\nTotal pairs "<< i <<" -- -- -- -- -- -- -- -- -- --"  << endl;
        cout << "-- Total Possible matches  ["<< tot_matches <<"]"  << endl;
        cout << "-- Total Good Matches      ["<< tot_good <<"]"  << endl;
        cout << "-- Total Accuracy  ["<< (int)(tot_good*100/tot_matches) <<" %]"  << endl;
        //high_resolution_clock::time_point t2 = high_resolution_clock::now();
        //auto duration = duration_cast<microseconds>( t2 - t1 ).count()/1000;
        t = 1000 * ((double) getTickCount() - t) / getTickFrequency();        
        cout << "   Execution time: " << t << " ms" <<endl;
    }
    return 0;
}



