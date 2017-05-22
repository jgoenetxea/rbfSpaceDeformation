// Common includes
#include <unistd.h>
#include <iostream>
#include <vector>
// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// My includes
#include "MultiplatformTimer.h"
#include "rbf.h"

using std::cout;
using std::endl;
using std::vector;
using cv::Mat;
using cv::Point2f;
using cv::imshow;
using cv::waitKey;
using cv::flip;

int main() {
    cout << "Start this staff!" << endl;
    
    int width = 800;
    int height = 600;
    
    vector<Point2f> original;
    original.push_back(Point2f(1, 1));
    original.push_back(Point2f(2, 1));
    original.push_back(Point2f(3, 1));
    original.push_back(Point2f(1, 2));
    original.push_back(Point2f(2, 2));
    original.push_back(Point2f(3, 2));
    original.push_back(Point2f(1, 3));
    original.push_back(Point2f(2, 3));
    original.push_back(Point2f(3, 3));
    
    // Space transformation
    Mat trans(2, 2, CV_32F);
    trans.at<float>(0, 0) = 2;
    trans.at<float>(0, 1) = 0;
    trans.at<float>(1, 0) = -1;
    trans.at<float>(1, 1) = 1;
    
    /*size_t m = original.size();
    vector<Point2f> deformed(m);
    for (size_t i = 0; i < m; ++i) {
		deformed[i] = trans * original[i];
	}*/
    
    // Drawing
    Mat backImg;
    
    const float t = 1000 / 30.;
    float elapsed;
    float sleeptime_millis;
    multiplat::Timer tmr;
    int dotSize = 3;
    
    Point2f op(1, 3);
    float dotStep = 0.06;
    float cont = 0;
    
    tmr.init();
    while(1) {
		elapsed = tmr.getTotalElapsedSeconds() * 1000;  // secs to milis
		sleeptime_millis = t - elapsed;
		if (sleeptime_millis > 0) usleep(sleeptime_millis * 1000); 
		tmr.init();
		
        backImg = Mat::zeros(height, width, CV_8UC3);
        
        size_t n = original.size();
        float xStep = width / 4.f;
        float yStep = height / 4.f;
        
        // Update moving dot
        cont += dotStep;
        if (cont > 3.1416f * 2) cont = 0.f;
        
        op.x = 2 + sin(cont * 2);
        op.y = 2 + cos(cont);
        
        // Draw moving dot
        cv::circle(backImg, Point2f(op.x * xStep, op.y * yStep), 
				   dotSize + 2, CV_RGB(255, 0, 0), -1);
                       
        // Draw original dots
        for (size_t i = 0; i < n; ++i) {
            cv::circle(backImg, Point2f(original[i].x * xStep, 
                       original[i].y * yStep), dotSize, 
                       CV_RGB(0, 0, 255), -1);
        }
        
        flip(backImg, backImg, 0);
        imshow("Back image", backImg);
        
        int c = waitKey(10);
        if (c == 27) {
			break;
		}
    }
}
