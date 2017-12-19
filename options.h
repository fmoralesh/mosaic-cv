/********************************************
 * FILE NAME: options.h                     *
 * DESCRIPTION: Contains the argument       *
 *              parser flags                *
 *              images                      *
 * AUTHOR: Victor García                    *
 ********************************************/

#ifndef OPTIONS
#define OPTIONS

#include "args.hxx"
#include <unordered_map>
#include <functional>

args::ArgumentParser parser("Feature Detectors comparison", "Author: Victor Garcia");
args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

args::Group sub_p(parser, "", args::Group::Validators::AllOrNone);
args::Group op_feature(sub_p, "Select the Feature Extractor and Descriptor:", args::Group::Validators::AtMostOne);
args::Flag op_sift(op_feature, "SIFT", "Use SIFT Extractor and Descriptor", {"sift"});
args::Flag op_surf(op_feature, "SURF", "Use SURF Extractor and Descriptor", {"surf"});
args::Flag op_orb(op_feature, "ORB", "Use ORB Extractor and Descriptor", {"orb"});
args::Flag op_kaze(op_feature, "KAZE", "Use KAZE Extractor and Descriptor", {"kaze"});

args::Group op_matcher(sub_p, "Select the Feature Matcher:", args::Group::Validators::AtMostOne);
args::Flag op_brutef(op_matcher, "Brute Force", "Use Brute Force Matcher", {'b'});
args::Flag op_flann(op_matcher, "Flann", "Use Flann Matcher", {'f'});

args::Group op_data(sub_p, "Select imput data:", args::Group::Validators::AtMostOne);
args::ValueFlagList<std::string> op_img(op_data, "ImageName", "Image imput name. Mus specify two file names (one per flag)",{'i'});
args::ValueFlag<std::string> op_vid(op_data, "VideoName", "Video imput name",{'v'});
args::ValueFlag<std::string> op_split(op_data,"VideoName","Split video in images, one image per second",{'s'});
//args::ValueFlag<std::string> op_dir(op_data,"","Directory to load the frames. (Not yet implemented)",{'d'});
args::Group optional(parser, "(Optional)", args::Group::Validators::DontCare);
args::Flag op_out(optional, "Output", "Only for images imput. Show output for good matches.", {'o'});




#endif