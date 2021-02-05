#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;

Mat convertColor(Mat imager);
void SneakRect(double dx,double dy, vector<Rect>br,vector<Rect>br2,double gd, Mat im3, Mat im);

int main(int argc, const char * argv[]) {
    Mat image, image2, image3, drawing;
    Rect rect;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    double ratio, delta_x, delta_y, gradient;
    int refinery_count = 0;
    
    image = imread("");
    
    image.copyTo(image2);
    image.copyTo(image3);
    
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
    
    for(int i = 0; i< contours.size(); i++){
        ratio = (double) boundRect[i].height / boundRect[i].width;
        
        if((ratio <= 2.5) && (ratio >= 0.5) && (boundRect[i].area() <= 700) && (boundRect[i].area() >= 100)){
            drawContours(drawing, contours, i, Scalar(0,255,255), 1, 8, hierarchy, 0, Point());
            rectangle(drawing, boundRect[i].tl(), boundRect[i].br() ,Scalar(255,0,0), 1,8,0);
            
            refinery_count += 1;
            boundRect2[refinery_count] = boundRect[i];
        }
    }
    
    boundRect2.resize(refinery_count);
    
    imshow("Original -> Gray -> Canny -> Contours&Rectangles", drawing);
    waitKey(0);
    
    SneakRect(delta_x = 0,delta_y = 0,boundRect,boundRect2,gradient, image3, image);
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

void SneakRect(double dx,double dy, vector<Rect>br,vector<Rect>br2, double gd, Mat im3, Mat im) {
    Rect temp_rect;
    int count = 0;
    int friend_count = 0;
    int select = 0;
    int plate_width = 0;
    
    for(int i=0; i<br2.size(); i++){
        for(int j=0; j<(br2.size()-i);j++){
            if(br2[j].tl().x > br2[j+1].tl().x){
                
                temp_rect = br2[j];
                br2[j] = br2[j+1];
                br2[j+1] = temp_rect;
                
            }
        }
    }
    for(int i =0; i< br2.size();i++){
        rectangle(im3, br2[i].tl(), br2[i].br(), Scalar(0,255,0), 1, 8, 0);
        
        for(int j=i+1; j<br2.size();j++){
            dx = abs(br2[j].tl().x - br2[i].tl().x);
            
            if(dx > 150) break;
            
            dx = abs(br2[j].tl().y - br2[i].tl().y);
            
            if(dx == 0) dx = 1;
            if(dy == 0) dy = 1;
            
            gd = dy / dx;
            cout << gd << endl;
            
            if(gd < 0.25){
                count +=1;
            }
        }
        
        if(count > friend_count){
            select = i;
            friend_count = count;
            rectangle(im3, br2[select].tl(), br2[select].br(), Scalar(255,0,0), 1, 8, 0);
            plate_width = dx;
        }
    }
    
    rectangle(im3, br2[select].tl(), br2[select].br(), Scalar(0,0,255), 2, 8, 0);
    line(im3, br2[select].tl(),Point(br2[select].tl().y), Scalar(0,0,255), 1, 8, 0);
    
    imshow("Rectangles on origin", im3);
    waitKey(0);
    
    imshow("Region of interest", im(Rect(br2[select].tl().x-20, br2[select].tl().y-20, plate_width+40, plate_width*0.3)));
    waitKey(0);

    imwrite("/Home/kirist/Downloads/result.jpg",
            im(Rect(br2[select].tl().x-20, br2[select].tl().y-20, plate_width+40, plate_width*0.3)));


    exit(0);
    
}
