#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int, char *argv[])

{

	//读取本地的一张图片便显示出来

	Mat img_in = imread(argv[1],IMREAD_UNCHANGED);
	Mat img_out;

	if (img_in.empty() ) {
		cout << "Error";
		return -1;
	}

	namedWindow(argv[1], WINDOW_AUTOSIZE);
	//namedWindow(argv[2], WINDOW_NORMAL);


	imshow(argv[1], img_in);
	//等待用户按键
	waitKey();
	return 0;

}