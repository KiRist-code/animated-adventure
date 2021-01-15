#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;

Mat convertColor(Mat imager);

int main(int argc, const char * argv[]) {
    Mat image, image2, image3, drawing;
    Rect rect, temp_rect;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    double ratio, delta_x, delta_y, gradient;
    int select, plate_width, count, friend_count = 0, refinery_count = 0;
    
    image = imread("");
    
    imshow("Original", image);
    waitKey(0);
    
    Mat convImg = convertColor(image2);
    
    findContours(convImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Rect> boundRect2(contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }
}

Mat convertColor(Mat imager){
    cvtColor(imager, imager, COLOR_RGB2GRAY);
    imshow("Original -> Gray", imager);
    waitKey(0);
    
    Canny(imager, imager, 100, 300 ,3);
    imshow("Original -> Gray -> Canny", imager);
    waitKey(0);
    
    return imager;
}
