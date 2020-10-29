#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;
using namespace cv;

void readimg();
void creatandchange();
void at_pixel();
void Buer();
void accuracy_check();
void black_check();

int main(void)
{
	readimg();
	//int i = 1;

	//for (i = 1; i <= 15; i++) {
	//	Mat image = imread("d://大型工作文件//08 葡萄面积处理//处理后图像//" + to_string(i) + "-1.jpg", IMREAD_UNCHANGED);
	//	long img_area[3] = { 0 };

	//	//遍历图片所有点，并获取原始深度，并输出坐标
	//	for (int row = 0; row < image.rows; row++)
	//	{
	//		for (int col = 0; col < image.cols; col++)
	//		{
	//			if (image.at<uchar>(row, col) > 0) {
	//				if ((row > -1) && (row < 160)) {
	//					img_area[0]++;
	//				}
	//				else if ((row > 159) && (row < 320)) {
	//					img_area[1]++;
	//				}
	//				else if ((row > 319) && (row < 480)) {
	//					img_area[2]++;
	//				}
	//			}
	//		}

	//	}

	//	cout << img_area[0] << '\t' << img_area[1] << '\t' << img_area[2] << endl;
	//}

	cin.get();
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

void accuracy_check(){
	Mat src = imread("d://0914test//2-3//org_3.jpg", IMREAD_UNCHANGED);
	//imshow("abc", src);
	uchar pixel1 = src.at<uchar>(0, 0);
	int num = 0;
	int sumdepth = 0;
	for (int i = 316; i < 447; i = i + 10) {
		for (int j = 0; j < 597; j = j + 10) {
			pixel1 = src.at<uchar>(i, j);
			if (pixel1 > 255)
				continue;
			else {
				num++;
				sumdepth = pixel1 * 12 + sumdepth;
			}
		}
	}

	cout << (double)sumdepth / (double)num << endl;
}

void black_check() {
	int i = 1;
	int count[3] = { 0 };

	for (i = 1; i <= 15; i++) {
		Mat src = imread("d://大型工作文件//08 葡萄面积处理//" + to_string(i) + ".png", IMREAD_UNCHANGED);
		//Mat src = imread("d://大型工作文件//08 葡萄面积处理//left_data.frame-1 - 副本.png", IMREAD_UNCHANGED);
		count[0] = 0;
		count[1] = 0;
		count[2] = 0;


		for (int i = 0; i < 160; i++) {
			for (int j = 0; j < src.cols; j++) {
				Vec4b & pixel = src.at<Vec4b>(i, j);
				if ((int)pixel[3] != 255) {
					continue;
				}
				else {
					count[0]++;
				}
			}
		}
		for (int i = 160; i < 320; i++) {
			for (int j = 0; j < src.cols; j++) {
				Vec4b & pixel = src.at<Vec4b>(i, j);
				if (pixel[3] != 255) {
					continue;
				}
				else {
					count[1]++;
				}
			}
		}
		for (int i = 320; i < 480; i++) {
			for (int j = 0; j < src.cols; j++) {
				Vec4b & pixel = src.at<Vec4b>(i, j);
				if (pixel[3] != 255) {
					continue;
				}
				else {
					count[2]++;
				}
			}
		}
		cout << count[0] << '\t' << count[1] << '\t' << count[2] << endl;
	}
}