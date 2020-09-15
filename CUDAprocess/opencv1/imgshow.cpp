#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

void readimg();
void creatandchange();
void at_pixel();
void Buer();

int main(int, char *argv[])

{
	Mat image = imread("D://Dropbox//earth2.jpg");
	imshow("cLena.png", image);
	Rect rect2(10, 20, 100, 200);	//区域从（10，20）开始取点，宽100，高200像素
	Rect rect(10, 10, 150, 100);   //区域的左上角点的坐标为（10,10）
								   //区域宽为150，高为100
	Mat image_ori = image(rect2);
	namedWindow("结果", 1);
	imshow("结果", image_ori);
	waitKey();
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
	imwrite("D:\\Dropbox\\abc.jpg", img1);
}

//读取图片矩阵的第一个像素，无论是灰度图还是彩色图，并且运用uchar/Vec3b类型去读取，并且输出。简单但是低效
void at_pixel() {
	Mat src1 = imread("D://Dropbox//earth2.jpg", IMREAD_GRAYSCALE);
	uchar pixel1 = src1.at<uchar>(0, 0);
	cout << "Value of pixel (0,0):" << (unsigned int)pixel1 << endl;

	Mat src2 = imread("D://Dropbox//earth3.jpg", IMREAD_COLOR);
	Vec3b pixel2 = src2.at<Vec3b>(0, 0);
	cout << "R component of pixel (0,0):" << (unsigned int)pixel2[2] << endl;

	uchar R, G, B;
	for (int i = 0; i < src2.rows; i++) {
		Vec3b* pixrow = src2.ptr<Vec3b>(i);
		for (int j = 0; j < src2.cols; j++) {
			B = pixrow[j][0];
			G = pixrow[j][1];
			R = pixrow[j][2];
			if ((unsigned int)B == 0 && (unsigned int)G == 0 && (unsigned int)R == 0)
				break;
			cout << (unsigned int)R << "-" << (unsigned int)G << "-" << (unsigned int)B << ", ";
		}
		cout << endl;
	}
}

void Buer() {

	Mat img1 = imread("D://Dropbox//earth2.jpg", IMREAD_GRAYSCALE);
	if (img1.empty())
	{
		cout << "Cannot load image!" << endl;
		return;
	}

	imshow("Original", img1);

	//创建掩码图像
	Mat mask(img1.rows, img1.cols, CV_8UC1, Scalar(0, 0, 0));
	circle(mask, Point(img1.cols / 2, img1.rows / 2), 150, 255, -1);
	imshow("Mask", mask);

	//执行AND
	Mat r;
	bitwise_and(img1, mask, r);
	imshow("Result0", r);

	//用白色填充外部
	const uchar white = 255;
	for (int i = 0; i < r.rows; i++)
		for (int j = 0; j < r.cols; j++)
			if (pow((i - img1.rows / 2), 2) + pow((j - img1.cols / 2), 2) >= pow(150, 2))
				r.at<uchar>(i, j) = white;
	imshow("Result1", r);
}