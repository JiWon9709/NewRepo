// opencv c++로 해보기
#include <stdio.h>
#include "string"
#include "fstream"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void detect_text(string input) {
	Mat large = imread(input);

	Mat rgb;
	// downsample and use it for processing
	pyrDown(large, rgb);
	Mat small;
	cvtColor(rgb, small, COLOR_BGR2GRAY);
	// morphological gradient
	Mat grad;
	Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(small, grad, MORPH_GRADIENT, morphKernel);
	// binarize
	Mat bw;
	threshold(grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
	// connect horizontally oriented regions
	Mat connected;
	morphKernel = getStructuringElement(MORPH_RECT, Size(9, 1));
	morphologyEx(bw, connected, MORPH_CLOSE, morphKernel);
	// find contours
	Mat mask = Mat::zeros(bw.size(), CV_8UC1);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(connected, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// filter contours
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		Rect rect = boundingRect(contours[idx]);
		Mat maskROI(mask, rect);
		maskROI = Scalar(0, 0, 0);
		// fill the contour
		drawContours(mask, contours, idx, Scalar(255, 255, 255), FILLED);
		// ratio of non-zero pixels in the filled region
		double r = (double)countNonZero(maskROI) / (rect.width * rect.height);

		// assume at least 45% of the area is filled if it contains text
		if (r > 0.45 &&
			(rect.height > 8 && rect.width > 8) // constraints on region size
												// these two conditions alone are not very robust. better to use something 
												//like the number of significant peaks in a horizontal projection as a third condition
			) {
			rectangle(rgb, rect, Scalar(0, 255, 0), 2);
		}
	}

	imwrite(string("test_text_contours.jpg"), rgb);
}

int main(int argc, char* argv[]) {
	detect_text(string("test2.jpg"));
}