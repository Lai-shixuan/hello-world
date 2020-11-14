/*#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace std;
using namespace cv;*/



#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;
using namespace cv;

#define PI 3.1415

int thresh = 280, N = 1;       //������ȡ��������
float EpsilonThd = 0.05;
float MaxRegion = 200000, MinRegion = 8000;
float SharpeThd = 2.5;
float AngleMaxThd = 105;
float AngleMinThd = 75;
float AngleParaThd = 10;
float EqualThd = 10;       //�����Ƿ�Ϊͬһ��ľ�����ֵ

void findSquares(const Mat& image, vector<vector<Point>>& squares);
double AngleCal(Point pt1, Point pt2, Point pt0);
void RemoveInvalid(vector<vector<Point>> &corner);      //�Ƴ���Ч����
bool CompareCorner(vector<Point> corner1, vector<Point> corner2);     //�жϽǵ��Ƿ�Ϊͬһ����
void drawSquares(Mat& image, const vector<vector<Point> >& squares);

float getDistance(Point2f a, Point2f b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int main()
{
	Mat image = imread("123.jpg");
	if (!image.data)
	{
		cout << "��ͼƬ����" << endl;
		return -1;
	}

	Mat image_gray;
	vector<vector<Point>> squares;

	cvtColor(image, image_gray, COLOR_BGR2GRAY);

	imshow("SorceImage", image_gray);

	findSquares(image_gray, squares);
	RemoveInvalid(squares);
	drawSquares(image, squares);

	imshow("Result", image);

	waitKey();

	return 0;
}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double AngleCal(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}


void findSquares(const Mat& image, vector<vector<Point>>& squares)
{
	/*�ҵ������εĺ���
	image ��������ҳ����ε�ͼƬ
	squares ��������ε㼯*/

	int index = 0;
	RNG rng = theRNG();

	squares.clear();

	vector<vector<Point> > contours;
	for (int con = 0; con < 1; con++)
	{
		Mat timg, cimg;
		image.convertTo(cimg, CV_8U, 0.5 + 0.5*con, 0);//cimg�ı�
		imshow("Image" + to_string(con), cimg);
		for (int th = 0; th < 3; th++)
		{
			bilateralFilter(cimg, timg, -1, 3 + th * 3, 15);//˫��
			Mat gray;

			for (int l = 0; l < N; l++)
			{
				if (l == 0)
				{
					Canny(timg, gray, 5, thresh, 5);//canny
					imshow("canny" + to_string(th), gray);
					Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
					dilate(gray, gray, kernel, Point(-1, -1));//����
				}
				else
				{
					gray = timg >= (l + 1) * 255 / N;//***GRAY->timg
				}
				int kernel_length = timg.cols;//invalid

				findContours(gray, contours, RETR_LIST, CHAIN_APPROX_NONE);//������

				Mat display;
				cvtColor(timg, display, COLOR_GRAY2BGR);

				vector<Point> approx;

				for (size_t i = 0; i < contours.size(); i++)
				{
					approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*EpsilonThd, true);      //��������

					Scalar color = { (double)rng(256),(double)rng(256) ,(double)rng(256) };//�����ɫ
					drawContours(display, contours, i, color, 1);

					RotatedRect rect = minAreaRect(contours[i]);        //��������
					double sharpe = rect.size.width / rect.size.height; //���γ��ȸ�

					if (approx.size() == 4 &&//������С
						fabs(contourArea(Mat(approx))) > MinRegion &&//�����Χ
						fabs(contourArea(Mat(approx))) < MaxRegion &&//�����Χ
						sharpe < SharpeThd && //����ȷ�Χ
						sharpe > 1.0 / SharpeThd &&//����ȷ�Χ
						isContourConvex(Mat(approx)))
					{
						float maxAngle = 90, minAngle = 90;
						float rect_angle[3];

						for (int j = 2; j < 5; j++)
						{
							float angle = acos(AngleCal(approx[j % 4], approx[j - 2], approx[j - 1])) * 180 / PI;
							rect_angle[j - 2] = angle;

							maxAngle = MAX(maxAngle, angle);
							minAngle = MIN(minAngle, angle);
						}

						float paral = (abs(180 - rect_angle[1] - rect_angle[0]) + abs(180 - rect_angle[1] - rect_angle[2])) / 2;

						if (maxAngle < AngleMaxThd && minAngle > AngleMinThd && paral < AngleParaThd)
							squares.push_back(approx);
					}

				}
				imshow("����" + to_string(index), display);
				index++;
			}
		}
	}

}

void RemoveInvalid(vector<vector<Point>> &corner)
{
	if (corner.size() == 0)
		return;

	for (int i = 1; i < corner.size(); i++)
	{
		vector<Point> temp = corner[i];

		for (int j = 0; j < i; j++)
		{
			vector<Point> temp2 = corner[j];

			if (CompareCorner(temp, temp2))
			{
				corner.erase(corner.begin() + i);

				i--;
				break;
			}
		}
	}
}

bool CompareCorner(vector<Point> corner1, vector<Point> corner2)
{
	bool is_equal = false;

	for (int i = 0; i < corner1.size(); i++)
	{
		Point temp = corner1[i];

		for (int j = 0; j < corner2.size(); j++)
		{
			Point temp2 = corner2[j];

			if (getDistance((Point2f)temp, (Point2f)temp2) < EqualThd)
			{
				is_equal = true;
				break;
			}

		}

		if (!is_equal)
			break;
	}

	return is_equal;
}

void drawSquares(Mat& image, const vector<vector<Point> >& squares)
{
	/*���Ѿ��ҵ������ε�ͼƬ�ϻ���������
	image ��Ȧ�������ε�ͼƬ
	squares �ҵ��ĳ����ε㼯*/

	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];

		int n = (int)squares[i].size();
		if (p->x > 3 && p->y > 3)
			polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 2, LINE_AA);
	}
}


/*int main()     sobel���ӣ����Բ��
{
	//��1����������
	Mat g_x, g_y;
	Mat a_x, a_y, dst, midImage;

	//��2������ԭʼͼ
	Mat src = imread("111.jpg");

	//��3����ʾԭʼͼ
	imshow("src", src);



	//��4����X�����ݶ�
	Sobel(src, g_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_x, a_x);
	imshow("X_Ч��ͼ", a_x);

	//��5����Y�����ݶ�
	Sobel(src, g_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_y, a_y);
	imshow("Y_Ч��ͼ", a_y);

	//��6���ϲ��ݶȣ����ƣ�
	addWeighted(a_x, 0.5, a_y, 0.5, 0, dst);
	imshow("���巽��", dst);

	cvtColor(dst, midImage, CV_BGR2GRAY);
	GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);

	vector<Vec3f> circles;
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//����Բ��
		circle(dst, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//����Բ����
		circle(dst, center, radius, Scalar(155, 50, 255), 3, 8, 0);
	}

	imshow("���巽��", dst);

	waitKey(0);
	return 0;
}*/



/*int main(void) {        ��ɫ����

	char input_image[] = "input image";
	char output_image[] = "output image";

	Mat src, abc2, abc3, abc4, binary;
	src = imread("111.jpg");
	namedWindow("����1",WINDOW_KEEPRATIO);
	imshow("ori", src);
	cvtColor(src, abc2, COLOR_BGR2HSV);
	inRange(abc2, Scalar(110, 60, 17), Scalar(180, 255, 255), abc3);
	inRange(abc2, Scalar(0, 60, 17), Scalar(5, 255, 255), abc4);
	add(abc3, abc4, binary);
	imshow("det", binary);


	// �ղ���������ͨ�����ڲ�
	Mat morphImage;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, morphImage, MORPH_CLOSE, kernel, Point(-1, -1), 2);
	imshow("morphology", morphImage);

	// ��ȡ�������
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(morphImage, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	Mat connImage = Mat::zeros(src.size(), CV_8UC3);

	drawContours(connImage, contours, -1, Scalar(0, 0, 255), 1, 8, hireachy);
	imshow("����1", connImage);

	waitKey();
	destroyAllWindows();

	return 0;
}*/

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