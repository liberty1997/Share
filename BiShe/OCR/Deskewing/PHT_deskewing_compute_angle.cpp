#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>

/*
 *	Probabilistic Hough Transform for line detection
 *	
 *	compute the mean angle of all the lines
 */

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	String imgName = "/home/vanellope/skew_angle01.png"; // by default
	if(argc > 1)
	{
		imgName = argv[1];
	}
	Mat img = imread(imgName, IMREAD_GRAYSCALE);
	if( img.empty() ) { cout << "Error loading img" << endl; return -1; }
	Size size = img.size();
	bitwise_not(img, img); // invert the colors of our image
	vector<Vec4i> lines;
	HoughLinesP(img, lines, 1, CV_PI/180, 100, size.width / 2.f, 20);
	Mat disp_lines(size, CV_8UC1, Scalar(0, 0, 0));
	double angle = 0;
	unsigned nb_lines = lines.size();
	for(unsigned i = 0; i < nb_lines; ++i)
	{
		line(disp_lines, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 0, 0));
		angle += atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0]);
	}
	angle /= nb_lines; // mean angle, in radians.
	cout << "File " << imgName << ":" << angle * 180 / CV_PI << endl;
	imshow(imgName, disp_lines);
	waitKey();
	return 0;
}
