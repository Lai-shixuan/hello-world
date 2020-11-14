#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(void) {

	char input_image[] = "input image";
	char output_image[] = "output image";

	Mat src, abc2, abc3, abc4, binary;
	src = imread("grape_example.jpg");
	imshow("ori", src);
	cvtColor(src, abc2, COLOR_BGR2HSV);
	inRange(abc2, Scalar(150, 60, 20), Scalar(180, 255, 255), abc3);
	inRange(abc2, Scalar(0, 60, 20), Scalar(10, 255, 255), abc4);
	add(abc3, abc4, binary);
	imshow("det", binary);


	// 闭操作进行联通物体内部
	Mat morphImage;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, morphImage, MORPH_CLOSE, kernel, Point(-1, -1), 2);
	imshow("morphology", morphImage);

	// 获取最大轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(morphImage, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	Mat connImage = Mat::zeros(src.size(), CV_8UC3);
	/*for (size_t t = 0; t < contours.size(); t++) {
		cout << "abc" << endl;
		Rect rect = boundingRect(contours[t]);
		if (rect.width < src.cols / 2) continue;
		if (rect.width > src.cols - 20) continue;

		double area = contourArea(contours[t]);
		double len = arcLength(contours[t], true);


		cout << "def" << endl;
		drawContours(connImage, contours, t, Scalar(0, 0, 255), 1, 8, hireachy);
		//printf("area of star could : %f \n", area);
		//printf("lenght of star could : %f \n", len);
	}*/
	drawContours(connImage, contours, -1, Scalar(0, 0, 255), 1, 8, hireachy);
	imshow(output_image, connImage);

	waitKey();
	destroyAllWindows();

	return 0;
}