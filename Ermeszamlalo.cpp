#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main() {
	cout << "Kerem adja meg milyen szinu a hatter: \n1. Szurke \n2. Fekete" << endl;
	int Hatter;
	cin >> Hatter;
	Mat image;
	Mat Template[6];
	if (Hatter == 1) {
		image = imread("Image1.jpg", IMREAD_COLOR);
		Template[0] = imread("Sz5.jpg", IMREAD_COLOR);
		Template[1] = imread("Sz10.jpg", IMREAD_COLOR);
		Template[2] = imread("Sz20.jpg", IMREAD_COLOR);
		Template[3] = imread("Sz50.jpg", IMREAD_COLOR);
		Template[4] = imread("Sz100.jpg", IMREAD_COLOR);
		Template[5] = imread("Sz200.jpg", IMREAD_COLOR);
	}
	else if (Hatter == 2) {
		image = imread("BImage1.jpg", IMREAD_COLOR);
		Template[0] = imread("B5.jpg", IMREAD_COLOR);
		Template[1] = imread("B10.jpg", IMREAD_COLOR);
		Template[2] = imread("B20.jpg", IMREAD_COLOR);
		Template[3] = imread("B50.jpg", IMREAD_COLOR);
		Template[4] = imread("B100.jpg", IMREAD_COLOR);
		Template[5] = imread("B200.jpg", IMREAD_COLOR);
	}
	int X;
	int Y;
	float arany = (float) image.cols / image.rows;
	if (arany >= 1) {
		X = 500;
		Y = 500 / (arany);
	}
	else {
		X = 500 / (arany);
		Y = 500;
	}
	Mat atmeretezett;
	resize(image, atmeretezett, Size(X, Y));
	Mat gray;
	cvtColor(atmeretezett, gray, COLOR_BGR2GRAY);
	vector<Vec3f> coin;
	GaussianBlur(gray, gray, Size(9, 9), 2, 2);
	HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 55, 50, 0, 0);
	int l = coin.size();
	cout << "A korok szama : " << l << "\n\n";
	int beta = 0;
	double alpha = 0.3;
	Mat new_image = Mat::zeros(atmeretezett.size(), atmeretezett.type());
	for (int y = 0; y < atmeretezett.rows; y++) {
		for (int x = 0; x < atmeretezett.cols; x++) {
			for (int c = 0; c < atmeretezett.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*atmeretezett.at<Vec3b>(y, x)[c] + beta);
			}
		}
	}
	blur(new_image, new_image, Size(9, 9), Point(-1, -1), BORDER_CONSTANT);
	int Osszeg = 0;
	for (size_t i = 0; i < coin.size(); i++)	{
		int d = 0;
		int sz = 0;
		int A = 0;
		Point center(cvRound(coin[i][0]), cvRound(coin[i][1]));
		cout << center << endl;
		int radius = cvRound(coin[i][2]);
		int FsX = coin[i][0] - radius;
		int FsY = coin[i][1] - radius;
		cout << "Sugar : " << radius << endl;
		Rect r(FsX, FsY, 2*radius-10, 2*radius-10);
		Mat kor = new_image(r);
		Mat tempresized;
		int Elteres = 500000;
		int Erme = 6;
		for (int i = 0; i <= 5; i++) {
			for (alpha = 0.3; alpha <= 0.3; alpha += 0.1) {
				Mat new_template = Mat::zeros(Template[i].size(), Template[i].type());
				for (int y = 0; y < Template[i].rows; y++) {
					for (int x = 0; x < Template[i].cols; x++) {
						for (int c = 0; c < Template[i].channels(); c++) {
							new_template.at<Vec3b>(y, x)[c] =
								saturate_cast<uchar>(alpha*Template[i].at<Vec3b>(y, x)[c] + beta);
						}
					}
				}
				blur(new_template, new_template, Size(9, 9), Point(-1,-1), BORDER_CONSTANT);
				resize(new_template, tempresized, Size(2 * radius-10, 2 * radius-10));
				/*cvtColor(tempresized, tempresized, COLOR_BGR2GRAY);
				cvtColor(kor, kor, COLOR_BGR2GRAY);
				matchShapes(kor, tempresized, CONTOURS_MATCH_I1, 1);*/
				d = (sum(abs(kor - tempresized))[0]) + (sum(abs(kor - tempresized))[1]) + (sum(abs(kor - tempresized))[2]);
				if (d < Elteres) {
					Elteres = d;
					Erme = i;
					cout << "Az elteres : " << i + 1 << ". ermevel: " << Elteres << endl;
					cout << "Ez Alpha :" << alpha << endl;
				}
			}
		}
		switch (Erme)
		{
		case 0:
			Osszeg = Osszeg + 5;
			Erme = 5;
			break;
		case 1:
			Osszeg = Osszeg + 10;
			Erme = 10;
			break;
		case 2:
			Osszeg = Osszeg + 20;
			Erme = 20;
			break;
		case 3:
			Osszeg = Osszeg + 50;
			Erme = 50;
			break;
		case 4:
			Osszeg = Osszeg + 100;
			Erme = 100;
			break;
		case 5:
			Osszeg = Osszeg + 200;
			Erme = 200;
			break;
		}
		cout << "Erme erteke: " << Erme << endl;
		circle(atmeretezett, center, radius, Scalar(200, 255, 200), 3, 8, 0);
	}
	cout << "Az ermek osszege : " << Osszeg << endl;
	cout << "\n";
	namedWindow("Coin Counter", WINDOW_AUTOSIZE);
	imshow("Coin Counter", atmeretezett);
	waitKey(0);
	return 0;
}