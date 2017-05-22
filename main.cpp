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
using cv::Matx22f;

void drawLines2(cv::Mat* img, const std::vector<cv::Point2f>& plist,
               const cv::Scalar color, const float xStep, const float yStep) {
    cv::line(*img, Point2f(plist[0].x * xStep,
                           plist[0].y * yStep),
                   Point2f(plist[1].x * xStep,
                           plist[1].y * yStep), color);

    cv::line(*img, Point2f(plist[1].x * xStep,
                           plist[1].y * yStep),
                   Point2f(plist[2].x * xStep,
                           plist[2].y * yStep), color);

    cv::line(*img, Point2f(plist[3].x * xStep,
                           plist[3].y * yStep),
                   Point2f(plist[4].x * xStep,
                           plist[4].y * yStep), color);

    cv::line(*img, Point2f(plist[4].x * xStep,
                           plist[4].y * yStep),
                   Point2f(plist[5].x * xStep,
                           plist[5].y * yStep), color);

    cv::line(*img, Point2f(plist[6].x * xStep,
                           plist[6].y * yStep),
                   Point2f(plist[7].x * xStep,
                           plist[7].y * yStep), color);

    cv::line(*img, Point2f(plist[7].x * xStep,
                           plist[7].y * yStep),
                   Point2f(plist[8].x * xStep,
                           plist[8].y * yStep), color);

    cv::line(*img, Point2f(plist[0].x * xStep,
                           plist[0].y * yStep),
                   Point2f(plist[3].x * xStep,
                           plist[3].y * yStep), color);

    cv::line(*img, Point2f(plist[3].x * xStep,
                           plist[3].y * yStep),
                   Point2f(plist[6].x * xStep,
                           plist[6].y * yStep), color);

    cv::line(*img, Point2f(plist[1].x * xStep,
                           plist[1].y * yStep),
                   Point2f(plist[4].x * xStep,
                           plist[4].y * yStep), color);

    cv::line(*img, Point2f(plist[4].x * xStep,
                           plist[4].y * yStep),
                   Point2f(plist[7].x * xStep,
                           plist[7].y * yStep), color);

    cv::line(*img, Point2f(plist[2].x * xStep,
                           plist[2].y * yStep),
                   Point2f(plist[5].x * xStep,
                           plist[5].y * yStep), color);

    cv::line(*img, Point2f(plist[5].x * xStep,
                           plist[5].y * yStep),
                   Point2f(plist[8].x * xStep,
                           plist[8].y * yStep), color);
}

int main() {
    cout << "Start this staff!" << endl;
    
    int width = 800;
    int height = 600;

    RBF rbf;
    
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
    Matx22f trans(1, -0.5,
                  0, 1);
    
    size_t m = original.size();
    vector<Point2f> deformed(m);
    for (size_t i = 0; i < m; ++i) {
		deformed[i] = trans * original[i];
	}
    
    // Drawing
    Mat backImg = Mat::zeros(height, width, CV_8UC3);
    Mat oriImg = Mat::zeros(height, width, CV_8UC3);
    Mat defImg = Mat::zeros(height, width, CV_8UC3);
    Mat backImg2 = Mat::zeros(height, width, CV_8UC3);
    Mat oriImg2 = Mat::zeros(height, width, CV_8UC3);
    Mat defImg2 = Mat::zeros(height, width, CV_8UC3);
    
    const float t = 1000 / 30.;
    float elapsed;
    float sleeptime_millis;
    multiplat::Timer tmr;
    int dotSize = 3;
    
    vector<Point2f> op(1);
    vector<Point2f> dp(1);
    float dotStep = 0.06;
    float cont = 0;

    float lambda = 1.f;
    bool clearImages = false;
    bool drawCircle = false;
    bool showLines = false;
    
    tmr.init();
    while(1) {
		elapsed = tmr.getTotalElapsedSeconds() * 1000;  // secs to milis
		sleeptime_millis = t - elapsed;
		if (sleeptime_millis > 0) usleep(sleeptime_millis * 1000); 
		tmr.init();
		
        if (!drawCircle) {
            backImg = Mat::zeros(height, width, CV_8UC3);
            oriImg = Mat::zeros(height, width, CV_8UC3);
            defImg = Mat::zeros(height, width, CV_8UC3);
        }
        
        size_t n = original.size();
        float xStep = width / 4.f;
        float yStep = height / 4.f;
        
        // Update moving dot
        cont += dotStep;
        if (cont > 3.1416f * 2) cont = 0.f;
        
        op[0].x = 2 + lambda * sin(cont);
        op[0].y = 2 + lambda * cos(cont);
        
        // RBF interpolation
        rbf.interpolate(original, deformed, op, &dp);

        // Draw moving def dot
        cv::circle(backImg, Point2f(dp[0].x * xStep, dp[0].y * yStep),
                   dotSize + 2, CV_RGB(0, 255, 0), -1);
        cv::circle(defImg, Point2f(dp[0].x * xStep, dp[0].y * yStep),
                   dotSize + 2, CV_RGB(0, 255, 0), -1);
				   
		// Draw deformed dots
        if (showLines) {
            drawLines2(&backImg, deformed, CV_RGB(50, 50, 50), xStep, yStep);
            drawLines2(&defImg, deformed, CV_RGB(50, 50, 50), xStep, yStep);
        }
        for (size_t i = 0; i < n; ++i) {
            cv::circle(backImg, Point2f(deformed[i].x * xStep, 
                       deformed[i].y * yStep), dotSize + 1, 
                       CV_RGB(0, 255, 255), 1);

            cv::circle(defImg, Point2f(deformed[i].x * xStep,
                       deformed[i].y * yStep), dotSize + 1,
                       CV_RGB(0, 255, 255), 1);
        }

        // Draw moving dot
        cv::circle(backImg, Point2f(op[0].x * xStep, op[0].y * yStep),
                   dotSize + 2, CV_RGB(255, 0, 0), -1);
        cv::circle(oriImg, Point2f(op[0].x * xStep, op[0].y * yStep),
                   dotSize + 2, CV_RGB(255, 0, 0), -1);

        // Draw original dots
        if (showLines) {
            drawLines2(&backImg, original, CV_RGB(128, 128, 128), xStep, yStep);
            drawLines2(&oriImg, original, CV_RGB(128, 128, 128), xStep, yStep);
        }
        for (size_t i = 0; i < n; ++i) {
            cv::circle(backImg, Point2f(original[i].x * xStep, 
                       original[i].y * yStep), dotSize, 
                       CV_RGB(0, 0, 255), -1);

            cv::circle(oriImg, Point2f(original[i].x * xStep,
                       original[i].y * yStep), dotSize,
                       CV_RGB(0, 0, 255), -1);
        }
        
        flip(backImg, backImg2, 0);
        flip(oriImg, oriImg2, 0);
        flip(defImg, defImg2, 0);
        imshow("Back image", backImg2);
        imshow("Original image", oriImg2);
        imshow("Deformed image", defImg2);
        
        int c = waitKey(10);
        if (c == 27) {
			break;
		}
        switch(static_cast<char>(c)) {
        case '1': {
            size_t m = original.size();
            for (size_t i = 0; i < m; ++i) {
                deformed[i] = trans * original[i];
            }
            clearImages = true;
            break;
        }
        case '2': {
            deformed[0] = Point2f(1.5, 1.5);
            deformed[1] = Point2f(2.2, 1.2);
            deformed[2] = Point2f(3.5, 0.5);
            deformed[3] = Point2f(1.2, 2.2);
            deformed[4] = Point2f(2, 2);
            deformed[5] = Point2f(2.8, 1.8);
            deformed[6] = Point2f(0.5, 3.5);
            deformed[7] = Point2f(1.8, 2.8);
            deformed[8] = Point2f(2.5, 2.5);
            clearImages = true;
            break;
        }
        case '3': {
            deformed[0] = Point2f(0.5, 0.5);
            deformed[1] = Point2f(2, 1);
            deformed[2] = Point2f(3.5, 0.5);
            deformed[3] = Point2f(1, 2);
            deformed[4] = Point2f(2, 2);
            deformed[5] = Point2f(3, 2);
            deformed[6] = Point2f(0.5, 3.5);
            deformed[7] = Point2f(2, 3);
            deformed[8] = Point2f(3.5, 3.5);
            clearImages = true;
            break;
        }
        case '4': {
            deformed[0] = Point2f(0.5, 0.9);
            deformed[1] = Point2f(2.1, 1.1);
            deformed[2] = Point2f(2.8, 1.2);
            deformed[3] = Point2f(1.3, 1.8);
            deformed[4] = Point2f(2.1, 2);
            deformed[5] = Point2f(2.8, 2);
            deformed[6] = Point2f(0.7, 2.7);
            deformed[7] = Point2f(2.1, 3.6);
            deformed[8] = Point2f(3.2, 2.9);
            clearImages = true;
            break;
        }
        case '+': {
            lambda += 0.1f;
            clearImages = true;
            break;
        }
        case '-': {
            lambda -= 0.1f;
            clearImages = true;
            break;
        }
        case 'C':
        case 'c': {
            drawCircle = !drawCircle;
            clearImages = true;
            break;
        }
        case 'L':
        case 'l': {
            showLines = !showLines;
            clearImages = true;
            break;
        }
        }

        if (clearImages) {
            backImg = Mat::zeros(height, width, CV_8UC3);
            oriImg = Mat::zeros(height, width, CV_8UC3);
            defImg = Mat::zeros(height, width, CV_8UC3);
            clearImages = false;
        }
    }
}
