/**
@file videocapture_basic.cpp
@brief A very basic sample for using VideoCapture and VideoWriter
@author PkLab.net
@date Aug 24, 2016
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image;
	image = imread("test1.jpg", IMREAD_COLOR);
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	Canny(image, image, 50, 150);
	imshow("Original", image);

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", image);

	waitKey(0);
}
