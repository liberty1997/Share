#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// we're NOT "using namespace std;" here, to avoid collisions between the beta variable and std::beta in c++17
using std::cin;
using std::cout;
using std::endl;
using namespace cv;

Mat img, new_img;
int alpha = 10; // Simple contrast control
int beta = 0;       // Simple brightness control
int const max_contrast = 50;
int const max_brightness = 100;
const char * trackbar_alpha = "contrast";
const char * trackbar_beta = "brightness";
const char * window_name = "Contrast&Brightness Demo";

static void Contrast_Brightness_Demo(int, void *)
{
	double dalpha = alpha / 10.0;
	img.convertTo(new_img, -1, dalpha, beta);
	imshow(window_name, new_img);
}

int main(int argc, char** argv)
{
	String imgName("/home/vanellope/lazar01.jpg"); // by default
    if(argc > 1)
    {
        imgName = argv[1];
    }
    img = imread( imgName, IMREAD_COLOR); // Load an image
	/* 另一种读取图片文件的形式
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    img = imread(parser.get<String>( "@input" ));
	*/
    if( img.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar(trackbar_alpha, window_name, &alpha, max_contrast, Contrast_Brightness_Demo);
	createTrackbar(trackbar_beta, window_name, &beta, max_brightness, Contrast_Brightness_Demo);
	Contrast_Brightness_Demo(0,0);
    waitKey();
	imwrite("toTesse.png", new_img);
    return 0;
}
