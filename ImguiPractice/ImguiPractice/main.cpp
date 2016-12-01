//
//  main.cpp
//  ImguiPractice
//
//  Created by MORITANAOKI on 2016/12/02.
//  Copyright © 2016年 molabo. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    
    VideoCapture cap(0);
    
    namedWindow("Window");
    
    while (1) {
        Mat frame;
        cap >> frame;
        
        imshow("Window", frame);
    }
    
    return 0;
}
