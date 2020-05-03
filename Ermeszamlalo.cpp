#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace cv;
using namespace std;

int main() {
	cout << "Kerem valassza ki az osszehasonlitas modjat: " << "\n" << "1. Szin alapu" << "\n" << "2. Templatematching" << endl;
	int x;
	cin >> x;
	//Globális változók
	int Osszeg, Kepszam = 0;
	Mat eredmeny;
	Mat image[10], Template[6];
	image[0] = imread("Image1.jpg", IMREAD_COLOR);
	image[1] = imread("Image2.jpg", IMREAD_COLOR);
	image[2] = imread("Image3.jpg", IMREAD_COLOR);
	image[3] = imread("Image4.jpg", IMREAD_COLOR);
	image[4] = imread("Image5.jpg", IMREAD_COLOR);
	image[5] = imread("Image6.jpg", IMREAD_COLOR);
	image[6] = imread("Image7.jpg", IMREAD_COLOR);
	image[7] = imread("Image8.jpg", IMREAD_COLOR);
	image[8] = imread("Image9.jpg", IMREAD_COLOR);
	image[9] = imread("Image10.jpg", IMREAD_COLOR);
	Template[0] = imread("Sz5.jpg", IMREAD_COLOR);
	Template[1] = imread("Sz10.jpg", IMREAD_COLOR);
	Template[2] = imread("Sz20.jpg", IMREAD_COLOR);
	Template[3] = imread("Sz50.jpg", IMREAD_COLOR);
	Template[4] = imread("Sz100.jpg", IMREAD_COLOR);
	Template[5] = imread("Sz200.jpg", IMREAD_COLOR);
	int kepszam = 0;
	if (x == 1) {
		for (int i = 0; i <= 9; i++) {
			int X;
			int Y;
			float arany = (float)image[i].cols / image[i].rows;
			if (arany >= 1) {
				X = 500;
				Y = 500 / (arany);
			}
			else {
				X = 500 / (arany);
				Y = 500;
			}
			Mat atmeretezett;
			resize(image[i], atmeretezett, Size(X, Y));
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
			int Osszeg = 0;
			Mat korforgatott;
			for (size_t i = 0; i < coin.size(); i++) {
				int d = 0;
				int sz = 0;
				int A = 0;
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				cout << center << endl;
				int radius = cvRound(coin[i][2]);
				int FsX = coin[i][0] - radius;
				int FsY = coin[i][1] - radius;
				cout << "Sugar : " << radius << endl;
				Rect r(FsX, FsY, 2 * radius - 5, 2 * radius - 5);
				Mat kor = new_image(r);
				Mat tempresized;
				int Elteres = 500000, Erme = 6, lightm, lightkor;
				double angle;
				for (int i = 0; i <= 5; i++) {
					cout << "ezt az ermet vizsgalom : " << i << endl;
					for (alpha = 0.3; alpha <= 0.7; alpha += 0.1) {
						Mat new_template = Mat::zeros(Template[i].size(), Template[i].type());
						for (int y = 0; y < Template[i].rows; y++) {
							for (int x = 0; x < Template[i].cols; x++) {
								for (int c = 0; c < Template[i].channels(); c++) {
									new_template.at<Vec3b>(y, x)[c] =
										saturate_cast<uchar>(alpha*Template[i].at<Vec3b>(y, x)[c] + beta);
								}
							}
						}
						for (angle = -360; angle <= 0; angle += 30) {
							Mat r = getRotationMatrix2D(Point(kor.rows / 2, kor.cols / 2), angle, 1.0);
							warpAffine(kor, korforgatott, r, Size(2 * radius - 5, 2 * radius - 5));
							for (int re = 0; re <= 30; re += 2) {
								Mat rekorf;
								resize(korforgatott, rekorf, Size(2 * radius - 15 + re, 2 * radius - 15 + re));
								resize(new_template, tempresized, Size(2 * radius - 15 + re, 2 * radius - 15 + re));
								lightm = ((sum(abs(new_template))[0] + sum(abs(new_template))[1] + sum(abs(new_template))[2]) / (3 * new_template.rows*new_template.cols));
								lightkor = ((sum(abs(rekorf))[0] + sum(abs(rekorf))[1] + sum(abs(rekorf))[2]) / (3 * rekorf.rows*rekorf.cols));
								d = (sum(abs(rekorf - tempresized))[0]);// +(sum(abs(rekorf - tempresized))[1]) + (sum(abs(rekorf - tempresized))[2]);
								if (d < Elteres) {
									Elteres = d;
									Erme = i;
									cout << "Az elteres : " << i << ". ermevel: " << Elteres << endl;
								}
							}
						}
					}
				}
				switch (Erme)
				{
				case 0:
					Osszeg = Osszeg + 5;
					Erme = 5;
					putText(atmeretezett, "5", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 1:
					Osszeg = Osszeg + 10;
					Erme = 10;
					putText(atmeretezett, "10", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 2:
					Osszeg = Osszeg + 20;
					Erme = 20;
					putText(atmeretezett, "20", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 3:
					Osszeg = Osszeg + 50;
					Erme = 50;
					putText(atmeretezett, "50", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 4:
					Osszeg = Osszeg + 100;
					Erme = 100;
					putText(atmeretezett, "100", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 5:
					Osszeg = Osszeg + 200;
					Erme = 200;
					putText(atmeretezett, "200", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				}
				cout << "Erme erteke: " << Erme << endl;
			}
			stringstream ss;
			ss << Osszeg;
			string text;
			ss >> text;
			putText(atmeretezett, text, Point(10, 300), FONT_HERSHEY_SIMPLEX, 2.0, Scalar(200, 255, 200), 4);
			cout << "Az ermek osszege : " << Osszeg << endl;
			cout << "\n";
			namedWindow("Coin Counter", WINDOW_AUTOSIZE);
			imshow("Coin Counter", atmeretezett);
			string mentes = std::to_string(++kepszam) + ".jpg";
			imwrite(mentes, atmeretezett);
		}
	}
	if (x == 2) {
		for (int i = 0; i <= 9; i++) {
			Osszeg = 0;
			int X;
			int Y;
			float arany = (float)image[i].cols / image[i].rows;
			if (arany >= 1) {
				X = 500;
				Y = 500 / (arany);
			}
			else {
				X = 500 / (arany);
				Y = 500;
			}
			//kor keresés
			Mat atmeretezett;
			resize(image[i], atmeretezett, Size(X, Y));
			int beta = 0;
			double alpha = 0.5;
			Mat gray;
			cvtColor(atmeretezett, gray, COLOR_BGR2GRAY);
			vector<Vec3f> coin;
			GaussianBlur(gray, gray, Size(9, 9), 2, 2);
			HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 55, 50, 0, 0);
			int l = coin.size();
			cout << "A korok szama : " << l << "\n\n";
			//körök ellenõrzése, hogy érmék-e és ha igen akkor milyenek?
			for (size_t i = 0; i < coin.size(); i++) {
				int Erme = 6;
				double d[6];
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				cout << center << endl;
				int radius = cvRound(coin[i][2]);
				int FsX = coin[i][0] - radius;
				int FsY = coin[i][1] - radius;
				cout << "Sugar : " << radius << endl;
				Rect r(FsX, FsY, 2 * radius, 2 * radius);
				Mat kor = atmeretezett(r);
				double dmin = 1000000000000;
				//Ermek betöltése egymás után
				for (int t = 0; t <= 5; t++) {
					Mat mintare1, korforgatott;
					resize(Template[t], mintare1, Size(2 * radius + 9, 2 * radius + 9));
					for (double angle = -360; angle <= 0; angle += 10) {
						Mat r = getRotationMatrix2D(Point(kor.rows / 2, kor.cols / 2), angle, 1.0);
						warpAffine(kor, korforgatott, r, Size(2 * radius, 2 * radius));
						matchTemplate(korforgatott, mintare1, eredmeny, TM_SQDIFF_NORMED);
						if (d[t] < dmin) {
							d[t] = sum(abs(eredmeny))[0];
							cout << d[t] << endl;
						}
					}
				}
				double min = 1000000000000;
				for (int x = 0; x <= 5; x++) {
					if (d[x] <= min) {
						min = d[x];
						Erme = x;
					}
				}
				switch (Erme)
				{
				case 0:
					Osszeg = Osszeg + 5;
					Erme = 5;
					putText(atmeretezett, "5", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 1:
					Osszeg = Osszeg + 10;
					Erme = 10;
					putText(atmeretezett, "10", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 2:
					Osszeg = Osszeg + 20;
					Erme = 20;
					putText(atmeretezett, "20", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 3:
					Osszeg = Osszeg + 50;
					Erme = 50;
					putText(atmeretezett, "50", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 4:
					Osszeg = Osszeg + 100;
					Erme = 100;
					putText(atmeretezett, "100", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				case 5:
					Osszeg = Osszeg + 200;
					Erme = 200;
					putText(atmeretezett, "200", center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 255, 200), 2);
					break;
				}
			}
			stringstream ss;
			ss << Osszeg;
			string text;
			ss >> text;
			putText(atmeretezett, text, Point(10, 300), FONT_HERSHEY_SIMPLEX, 2.0, Scalar(200, 255, 200), 4);
			cout << "Az ermek osszege : " << Osszeg << endl;
			cout << "\n";
			namedWindow("Coin Counter", WINDOW_AUTOSIZE);
			imshow("Coin Counter", atmeretezett);
			string mentes = std::to_string(++Kepszam) + ".jpg";
			imwrite(mentes, atmeretezett);
		}
		waitKey();
		return 0;
	}

}