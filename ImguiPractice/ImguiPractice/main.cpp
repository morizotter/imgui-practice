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

using namespace std;
using namespace cv;

void onMouse( int event, int x, int y, int flag, void* )
{
    
}

string win_name = "Window";

int main(int argc, const char * argv[]) {
    
    VideoCapture cap(0);
    
    namedWindow(win_name);
    
    while (1) {
        Mat frame;
        cap >> frame;
        
        imshow("Window", frame);
        
        int key = waitKey(1);
        if(key==27) {
            break;
        }
    }
    
    destroyWindow(win_name);
    
    return 0;
}
