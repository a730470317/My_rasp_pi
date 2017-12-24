#include "kernel.hpp"
#include <iostream>
//#include <Thread>
//#include <Mutex>
//#include <time>
#define STABLE_TOTAL_COUNT 100
#define ALERT_THRESHOLD 100
//std::mutex g_lock;

using namespace std;
using namespace cv;
int if_alert(Mat & img_in)
{
        int img_w = img_in.cols;
        int img_h = img_in.rows;
        double score = 0;
        if (img_in.channels() == 3)
    {
                cvtColor(img_in, img_in, CV_RGB2GRAY);
            
    }
        for (int i = 0; i < img_w; i++)
    {
                for (int j = 0; j < img_h; j++)
        {
                        score += img_in.at<uchar>(j*img_w + i);
                    
        }
            
    }
        if (score > ALERT_THRESHOLD )
    {
                return 1;
            
    }
        else
    {
                return 0;
            
    }

}

int main(int argc, const char** argv)
{
	Codebook_kernel codebook;
	cv::VideoCapture cap;
	cap.open(0);
	int remove_unstable_img = 0;
	int alert_stat = 0;
	while (1)
	{
		Mat img;
		cap.read(img);
		imshow("img", img);
		cv::waitKey(1);
		if (remove_unstable_img < STABLE_TOTAL_COUNT)
		{
			cout << "before stable ing --> " << remove_unstable_img << " | " << STABLE_TOTAL_COUNT << endl;
			remove_unstable_img++;
			continue; 
		}
		Mat res_codebook = codebook.run(img);
		imshow("res", res_codebook);
		if (if_alert(res_codebook))
		{
			cout << "Alert -> " << alert_stat++ << endl;
			//alert();
		}
	}
	return 0;
}
