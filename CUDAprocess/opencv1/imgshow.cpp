#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void readimg();
void creatandchange();
void at_pixel();

int main(int, char *argv[])

{


	

	waitKey();
	getchar();
	return 0;

}

//读取一张位于D盘的图片，并生成灰白照片，再保存
void readimg() {
	Mat img_in = imread("D://Dropbox//Snipaste_2020-08-14_13-57-46.jpg", IMREAD_UNCHANGED);
	Mat img_out;

	if (img_in.empty()) {
		cout << "Error";
		return;
	}

	namedWindow("abc", WINDOW_AUTOSIZE);
	namedWindow("abc_grey", WINDOW_AUTOSIZE);

	cvtColor(img_in, img_out, COLOR_BGR2GRAY);
	imshow("abc_grey", img_out);
	cout << "Press any key to exit...\n";


	imshow("abc", img_in);
	imwrite("abc.jpg", img_out);
	//等待用户按键
	waitKey();
}

//随即生成1000*1000照片，单通道，32f深度，并且转换成0-255区间范围内，展示
void creatandchange() {
	Mat m1 = Mat(1000, 1000, CV_32FC1);
	randu(m1, 0, 1e6);
	imshow("original", m1);
	double minRange, MaxRange;
	Point mLoc, Mloc;
	minMaxLoc(m1, &minRange, &MaxRange, &mLoc, &Mloc);
	Mat img1;
	m1.convertTo(img1, CV_8U, 255.0 / (MaxRange - minRange), -255.0*minRange / (MaxRange - minRange));//将矩阵线性缩放，得到新的区间
	imshow("result", img1);
}

//读取图片矩阵的第一个像素，无论是灰度图还是彩色图，并且运用uchar/Vec3b类型去读取，并且输出。简单但是低效
void at_pixel() {
	Mat src1 = imread("D://Dropbox//earth.jpg", IMREAD_GRAYSCALE);
	uchar pixel1 = src1.at<uchar>(0, 0);
	cout << "Value of pixel (0,0):" << (unsigned int)pixel1 << endl;

	Mat src2 = imread("D://Dropbox//earth.jpg", IMREAD_COLOR);
	Vec3b pixel2 = src2.at<Vec3b>(0, 0);
	cout << "R component of pixel (0,0):" << (unsigned int)pixel2[2] << endl;
}