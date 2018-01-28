#include "stitch.h"

using namespace std;
using namespace cv;

Rect getBound(Mat H, int width, int height){
	Point offset;
	Rect r;
	vector<Point2f> points;
	points.push_back(Point2f(0,0));
	points.push_back(Point2f(width,0));
	points.push_back(Point2f(width,height));
	points.push_back(Point2f(0,height));

	double w;
	for(int i=0; i<points.size(); i++){
		w 	=  H.at<double>(2,0)*points[i].x + H.at<double>(2,1)*points[i].y + H.at<double>(2,2);
		points[i].x = (H.at<double>(0,0)*points[i].x + H.at<double>(0,1)*points[i].y + H.at<double>(0,2)) / w;
		points[i].y = (H.at<double>(1,0)*points[i].x + H.at<double>(1,1)*points[i].y + H.at<double>(1,2)) / w;
	}

	// line(img, points[0], points[1], Scalar(0,255,0));
	// line(img, points[1], points[2], Scalar(0,255,0));
	// line(img, points[2], points[3], Scalar(0,255,0));
	// line(img, points[3], points[0], Scalar(0,255,0));
	return boundingRect(points);
}

Mat translateImg(Mat &img, int offsetx, int offsety){
    Mat t_img = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,t_img,img.size());
    return t_img;
}