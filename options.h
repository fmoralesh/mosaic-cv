/********************************************
 * FILE NAME: options.h                     *
 * DESCRIPTION: Contains the argument       *
 *              parser flags                *
 *              images                      *
 * AUTHOR: Victor García                    *
 ********************************************/

#ifndef OPTIONS
#define OPTIONS

#include "./args/args.hxx"

args::ArgumentParser parser("Feature Detectors comparison", "Author: Victor Garcia");
args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
args::Positional<std::string> op_img1(parser, "IMAGE_1", "Image1.jpg");
args::Positional<std::string> op_img2(parser, "IMAGE_2", "image2.jpg");
args::ValueFlag<std::string> op_vid(parser,"VIDEO","Video",{'v'});

args::Group op_feature(parser, "Select the Feature Detector and Descriptor:", args::Group::Validators::DontCare);
args::Flag op_sift(op_feature, "SIFT", "Use SIFT Detector and Descriptor", {"sift"});
args::Flag op_surf(op_feature, "SURF", "Use SURF Detector and Descriptor", {"surf"});
args::Flag op_orb(op_feature, "ORB", "Use ORB Detector and Descriptor", {"orb"});
args::Flag op_kaze(op_feature, "KAZE", "Use KAZE Detector and Descriptor", {"kaze"});

args::Group op_matcher(parser, "Select the Feature Detector and Descriptor:", args::Group::Validators::DontCare);
args::Flag op_brutef(op_matcher, "Brute Force", "Use Brute Force Matcher", {'b'});
args::Flag op_flann(op_matcher, "Flann", "Use Flann Matcher", {'f'});

#endif