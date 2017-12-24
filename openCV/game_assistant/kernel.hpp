#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opencv2/core/core.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/legacy/legacy.hpp"

using namespace cv;
using namespace std;
const int NCHANNELS = 3;
bool ch[NCHANNELS] = { true, true, true }; // This sets what channels should be adjusted for background bounds
class Codebook_kernel
{
public:
	int m_show_debug_info = 0;
	int nframesToLearnBG = 100;
	~Codebook_kernel() 
	{
		cvDestroyWindow("Raw");
		cvDestroyWindow("ForegroundCodeBook");
		cvDestroyWindow("CodeBook_ConnectComp");
	};
	CvBGCodeBookModel* model = 0;
	IplImage* rawImage = 0, *yuvImage = 0;
	//yuvImage is for codebook method
	IplImage *ImaskCodeBook = 0, *ImaskCodeBookCC = 0;
	int c, n, nframes = 0;
	int singlestep = 0;
	int pause = 0;
	int m_image_width, m_image_height;
	Codebook_kernel()
	{
		model = cvCreateBGCodeBookModel();
		model->modMin[0] = 3;
		model->modMin[1] = model->modMin[2] = 3;
		model->modMax[0] = 10;
		model->modMax[1] = model->modMax[2] = 10;
		model->cbBounds[0] = model->cbBounds[1] = model->cbBounds[2] = 10;
		int nframesToLearnBG = 100;
		bool useCamera = 1;

	};
	Mat return_void_Mat()
	{
		return Mat::zeros(m_image_height, m_image_width, CV_8UC(3));
	}

	Mat run(Mat& img_in)
	{
        IplImage temp = (IplImage)img_in;
		rawImage = &temp;
		if (!pause)
		{
			//rawImage = cvQueryFrame(capture);
			++nframes;
		}
		else
		{
			return return_void_Mat();
		}

		if (nframes <= nframesToLearnBG)
		{
			m_image_width = img_in.cols;
			m_image_height = img_in.rows;
		}
		if (singlestep)
			pause = true;

		//First time:
		if (nframes == 1 && rawImage)
		{
			// CODEBOOK METHOD ALLOCATION
			yuvImage = cvCloneImage(rawImage);
			ImaskCodeBook = cvCreateImage(cvGetSize(rawImage), IPL_DEPTH_8U, 1);
			ImaskCodeBookCC = cvCreateImage(cvGetSize(rawImage), IPL_DEPTH_8U, 1 );
			//ImaskCodeBookCC = &IplImage(img_in);
			cvSet(ImaskCodeBook, cvScalar(255));
			if (m_show_debug_info)
			{
				cvNamedWindow("Raw", 1);
				cvNamedWindow("ForegroundCodeBook", 1);
				cvNamedWindow("CodeBook_ConnectComp", 1);
			}
		}
		// If we've got an rawImage and are good to go:
		if (rawImage)
		{
			cvCvtColor(rawImage, yuvImage, CV_BGR2YCrCb);//YUV For codebook method
			//This is where we build our background model
			if (!pause && nframes - 1 < nframesToLearnBG)
			{
				cvBGCodeBookUpdate(model, yuvImage);
				cout << "Updata ing --> " << nframes << " | "<< nframesToLearnBG << endl;
				return return_void_Mat();
			}
			if (nframes - 1 == nframesToLearnBG)
				cvBGCodeBookClearStale(model, model->t / 2);

			//Find the foreground if any
			if (nframes - 1 >= nframesToLearnBG)
			{
				// Find foreground by codebook method
				cvBGCodeBookDiff(model, yuvImage, ImaskCodeBook);
				// This part just to visualize bounding boxes and centers if desired
				cvCopy(ImaskCodeBook, ImaskCodeBookCC);
				cvSegmentFGMask(ImaskCodeBookCC);
			}
			//Display
			if (m_show_debug_info)
			{
				cvShowImage("Raw", rawImage);
				cvShowImage("ForegroundCodeBook", ImaskCodeBook);
				cvShowImage("CodeBook_ConnectComp", ImaskCodeBookCC);
			}
		}

		//c = cvWaitKey(1) ;
		
		return cv::Mat(ImaskCodeBookCC);
	}

};
