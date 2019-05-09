#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
// findNonZero()

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

	Mat binary;
	threshold(img, binary, 0, 255, THRESH_BINARY + THRESH_OTSU);
	//bitwise_not(binary, binary);
	vector<Point> points;
	//findNonZero(binary, points);
	Mat_<uchar>::iterator it = binary.begin<uchar>();
	Mat_<uchar>::iterator end = binary.end<uchar>();
	for(; it != end; ++it)
		if(!*it)
			points.push_back(it.pos());
	RotatedRect box = minAreaRect(points);
	double angle = box.angle;
	cout << "File " << imgName << ": " << angle << endl;
	if(angle < -45.)
		angle += 90.;
	cout << "File " << imgName << ": " << angle << endl;
	Point2f vertices[4];
	box.points(vertices);
	for(int i = 0; i < 4; ++i)
		line(binary, vertices[i], vertices[(i + 1) % 4], Scalar(105, 105, 105));
	imshow(imgName, binary);
	
	// Rotation
	Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);
	Mat rotated;
	warpAffine(binary, rotated, rot_mat, binary.size(), INTER_CUBIC);
	// Cropping
	Size box_size = box.size;
	if(box.angle < -45.)
		swap(box_size.width, box_size.height);
	Mat cropped;
	getRectSubPix(rotated, box_size, box.center, cropped);
	imwrite("Cropped.jpg", cropped);
	waitKey();
	return 0;
}
