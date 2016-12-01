//
//  main.cpp
//  ImguiPractice
//
//  Created by MORITANAOKI on 2016/12/02.
//  Copyright © 2016年 molabo. All rights reserved.
//

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "imgui.h"

using namespace std;
using namespace cv;

string win_name = "Window";

int main(int argc, const char * argv[]) {
    
    VideoCapture cap(0);
    
    namedWindow(win_name);
    
    while (1) {
        Mat frame;
        cap >> frame;
        
        imshow(win_name, frame);
        
        int key = waitKey(1);
        if(key==27) {
            break;
        }
    }
    
    destroyWindow(win_name);
    
    return 0;
}
