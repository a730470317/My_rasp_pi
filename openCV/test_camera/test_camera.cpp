/*************************************************************************
	> File Name: test_camera.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年12月10日 星期日 22时16分21秒
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp> 
using namespace std;
using namespace cv;
int main()
{
    VideoCapture cap;
    cap.open(0);
    if(cap.isOpened())
    {
        while(1)
        {
        Mat  I;
        cap >>  I;
        imshow("image",I);
        cv::waitKey(10);
    
     
        }   
    }
    else
    {
        //imshow("Hello")
        cout<<"Open camera fail\n"<<endl;  
    }
    return 0;
}


