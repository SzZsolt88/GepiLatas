#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sstream>


using namespace cv;
using namespace std;

int main() {
	cout << "Kerem valassza ki az osszehasonlitas modjat: " << "\n" << "1. Szin" << "\n" << "2. Templatematching" << "\n" << "3. ORB" << "\n" << "4. Szin+ORB" << endl;
	int modszer;
	cin >> modszer;
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
	int X;
	int Y;
	float arany = (float)image[0].cols / image[0].rows;
	if (arany >= 1) {
		X = 500;
		Y = 500 / (arany);
	}
	else {
		X = 500 / (arany);
		Y = 500;
	}
	Mat atmeretezett;
	resize(image[0], atmeretezett, Size(X,Y));
	Mat gray;
	cvtColor(atmeretezett, gray, COLOR_BGR2GRAY);
	vector<Vec3f> coin;
	GaussianBlur(gray, gray, Size(9, 9), 2, 2);
	HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 85, 50, 29, 70);
	int l = coin.size();
	if (l == 6) {
		int k;
		int j;
		int tmp;
		for (k = l - 1; 0 < k; --k) {
			for (j = 0; j < k; ++j) {
				if (coin[j][0] > coin[j + 1][0]) {
					tmp = coin[j][0];
					coin[j][0] = coin[j + 1][0];
					coin[j + 1][0] = tmp;
					tmp = coin[j][1];
					coin[j][1] = coin[j + 1][1];
					coin[j + 1][1] = tmp;
					tmp = coin[j][2];
					coin[j][2] = coin[j + 1][2];
					coin[j + 1][2] = tmp;
				}
			}
		}
		for (size_t i = 0; i < l; i++) {
			int radius = cvRound(coin[i][2]);
			int FsX = coin[i][0] - (radius);
			int FsY = coin[i][1] - (radius);
			Rect r(FsX, FsY, 2 * (radius), 2 * (radius));
			Mat minta = atmeretezett(r);
			switch (i) {
			case 0: imwrite("T5.jpg", minta);
			case 1:	imwrite("T10.jpg", minta);
			case 2:	imwrite("T20.jpg", minta);
			case 3:	imwrite("T50.jpg", minta);
			case 4:	imwrite("T100.jpg", minta);
			case 5:	imwrite("T200.jpg", minta);
			}
		}
		Template[0] = imread("T5.jpg", IMREAD_COLOR);
		Template[1] = imread("T10.jpg", IMREAD_COLOR);
		Template[2] = imread("T20.jpg", IMREAD_COLOR);
		Template[3] = imread("T50.jpg", IMREAD_COLOR);
		Template[4] = imread("T100.jpg", IMREAD_COLOR);
		Template[5] = imread("T200.jpg", IMREAD_COLOR);
	}
	int kepszam = 0;
	if (modszer == 1) {
		for (int i = 0; i <= 9; i++) {
			//elõkészület
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
			HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 85, 50, 29, 70);
			int l = coin.size();
			cout << "A korok szama : " << l << "\n\n";
			double alpha = 0.3;
			Mat new_image = Mat::zeros(atmeretezett.size(), atmeretezett.type());
			for (int y = 0; y < atmeretezett.rows; y++) {
				for (int x = 0; x < atmeretezett.cols; x++) {
					for (int c = 0; c < atmeretezett.channels(); c++) {
						new_image.at<Vec3b>(y, x)[c] =
							saturate_cast<uchar>(alpha*atmeretezett.at<Vec3b>(y, x)[c]);
					}
				}
			}		
			int Osszeg = 0;
			Mat korforgatott;
			//Érme azonosítás
			for (size_t i = 0; i < coin.size(); i++) {
				int d = 0;
				int sz = 0;
				int A = 0;
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				cout << center << endl;
				int radius = cvRound(coin[i][2]);
				int FsX = coin[i][0] - (radius);
				int FsY = coin[i][1] - (radius);
				cout << "Sugar : " << radius << endl;
				Rect r(FsX, FsY, 2 * (radius), 2 * (radius));
				Mat kor = new_image(r);
				//GaussianBlur(kor, kor, Size(3, 3), 2, 2);
				Mat tempresized;
				int Elteres = 1000000, Erme = 6;
				double angle;
				for (int i = 0; i <= 5; i++) {
					cout << "ezt az ermet vizsgalom : " << i << endl;
					for (alpha = 0.3; alpha <= 0.3; alpha += 0.1) {
						Mat new_template = Mat::zeros(Template[i].size(), Template[i].type());
						for (int y = 0; y < Template[i].rows; y++) {
							for (int x = 0; x < Template[i].cols; x++) {
								for (int c = 0; c < Template[i].channels(); c++) {
									new_template.at<Vec3b>(y, x)[c] =
										saturate_cast<uchar>(alpha*Template[i].at<Vec3b>(y, x)[c]);
								}
							}
						}
						for (angle = -360; angle <= -360; angle += 10) {
							Mat r = getRotationMatrix2D(Point(kor.rows / 2, kor.cols / 2), angle, 1.0);
							warpAffine(kor, korforgatott, r, Size(2 * (radius), 2 * (radius)));
							resize(new_template, tempresized, Size(2 * (radius), 2 * (radius)));
							//GaussianBlur(tempresized, tempresized, Size(3, 3), 2, 2);
							int db = 0, dg = 0, dr = 0, d = 0;
							for (int y = 0; y < korforgatott.rows; y++) {
								for (int x = 0; x < korforgatott.cols; x++) {
									db = abs(korforgatott.at<Vec3b>(y, x)[0] - tempresized.at<Vec3b>(y, x)[0]);
									dg = abs(korforgatott.at<Vec3b>(y, x)[1] - tempresized.at<Vec3b>(y, x)[1]);
									dr = abs(korforgatott.at<Vec3b>(y, x)[2] - tempresized.at<Vec3b>(y, x)[2]);
									d = d + db + dg + dr;
								}
							}
							/*if (i == 0) {
								cout << "ez d 5!" << d << endl;
								d = d - abs((radius - 30) * 4250);
								cout << "ez d 5! utana :" << d << endl;
							}
							if (i == 1) {
								d = d - abs((radius - 35) * 4250);
								cout << "ez d 10!" << d << endl;
							}
							if (i == 2) {
								cout << "ez d 20!" << d << endl;
								d = d - abs((radius - 36 ) * 4250);
								cout << "ez d 20! utana :" << d << endl;
							}
							if (i == 3) {
								cout << "ez d 50!" << d << endl;
								d = d - abs((radius - 38) * 4250);
								cout << "ez d 50! utana : " << d << endl;
							}
							if (i == 4) {
								cout << "ez d 100!" << d << endl;
								d = d - abs((radius - 34) * 4250);
								cout << "ez d 100! utana :" << d << endl;
							}
							if (i == 5) {
								cout << "ez d 200!" << d << endl;
								d = d - abs((radius - 38) * 4250);
								cout << "ez d 200! utana : " << d << endl;
							}*/
							if (d < Elteres) {
								Elteres = d;
								Erme = i;
								cout << "Az elteres : " << i << ". ermevel: " << Elteres << endl;
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
			//Kiértékelés
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
	else if (modszer == 2) {
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
			Mat atmeretezett;
			resize(image[i], atmeretezett, Size(X, Y));
			int beta = 0;
			double alpha = 0.5;
			Mat gray;
			cvtColor(atmeretezett, gray, COLOR_BGR2GRAY);
			vector<Vec3f> coin;
			GaussianBlur(gray, gray, Size(9, 9), 2, 2);
			HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 85, 50, 29, 70);
			int l = coin.size();
			cout << "A korok szama : " << l << "\n\n";
			alpha = 0.5;
			Mat new_image = Mat::zeros(atmeretezett.size(), atmeretezett.type());
			for (int y = 0; y < atmeretezett.rows; y++) {
				for (int x = 0; x < atmeretezett.cols; x++) {
					for (int c = 0; c < atmeretezett.channels(); c++) {
						new_image.at<Vec3b>(y, x)[c] =
							saturate_cast<uchar>(alpha*atmeretezett.at<Vec3b>(y, x)[c] + beta);
					}
				}
			}
			//körök ellenõrzése, hogy érmék-e és ha igen akkor milyenek?
			for (size_t i = 0; i < coin.size(); i++) {
				int Erme = 6;
				double d[6];
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				int radius = cvRound(coin[i][2]);
				cout << center << endl;
				int FsX = coin[i][0] - radius;
				int FsY = coin[i][1] - radius;
				Rect r(FsX, FsY, 2 * radius, 2 * radius);
				Mat kor = atmeretezett(r);
				double dmin = 10000000000000;
				//Ermek betöltése egymás után
				for (int t = 0; t <= 5; t++) {
					Mat mintare1, korforgatott;
					for (double angle = -360; angle <= -360; angle += 10) {
						Mat r = getRotationMatrix2D(Point(kor.rows / 2, kor.cols / 2), angle, 1.0);
						warpAffine(kor, korforgatott, r, Size(2 * radius*(sin(angle)), 2 * radius*(cos(angle))));
						resize(Template[t], mintare1, Size(2 * radius, 2 * radius));
						matchTemplate(korforgatott, mintare1, eredmeny, TM_SQDIFF_NORMED);
						double minVal, maxVal; Point minLoc, maxLoc;
						minMaxLoc(eredmeny, &minVal, &maxVal, &minLoc, &maxLoc);
						if (d[t] < dmin) {
							d[t] = minVal;
						}
					}
				}
				double min = 1000000000000;
				for (int x = 0; x <= 5; x++) {
					if (d[x] <= min) {
						cout << "ez d erteke" << d[x] << ": " << x << "ermevel" << endl;
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
				circle(atmeretezett, center, radius, Scalar(200, 255, 200), 2);
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
	}
	else if (modszer == 3) {
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
			HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 85, 50, 29, 70);
			cout << "A korok szama : " << l << "\n\n";
			int beta = 0;
			double alpha = 0.3;
			int Erme = 6;
			int Osszeg = 0;
			for (int i = 0; i < coin.size(); i++) {
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				cout << "ez a kozepe: " << center << endl;
				int radius = cvRound(coin[i][2]);
				int FsX = coin[i][0] - (radius);
				int FsY = coin[i][1] - (radius);
				Rect r(FsX, FsY, 2 * (radius), 2 * (radius));
				Mat kor = atmeretezett(r);
				Mat tempresized;
				int dmax = 0;
				Mat GrayIm, GrayTemp, Dscrp1, Dscrp2, korkp, Tempkp;
				std::vector<cv::KeyPoint> Kpts1, Kpts2;
				Ptr<ORB> orb = ORB::create(500, 1.2f, 8, 5, 0, 4, ORB::FAST_SCORE, 5, 3);
				cvtColor(kor, GrayIm, COLOR_BGR2GRAY);
				orb->detectAndCompute(GrayIm, noArray(), Kpts1, Dscrp1);
				for (int erme = 0; erme <= 5; erme++) {
					Mat forgatott;
					for (int angle = -360; angle <= -360; angle += 10) {
						Mat r = getRotationMatrix2D(Point(kor.rows / 2, kor.cols / 2), angle, 1.0);
						warpAffine(Template[erme], forgatott, r, Size(2 * radius, 2 * radius));
						cout << "ezt az ermet vizsgalom: " << erme << endl;
						//resize(Template[erme], Template[erme], Size(2 * radius, 2 * radius))
						cvtColor(forgatott, GrayTemp, COLOR_BGR2GRAY);
						orb->detectAndCompute(GrayTemp, noArray(), Kpts2, Dscrp2);
						Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMINGLUT);
						vector<vector<DMatch> > knn_matches;
						matcher->knnMatch(Dscrp1, Dscrp2, knn_matches, 2);
						const float ratio_thresh = 0.7f;
						vector<DMatch> good_matches;
						for (size_t match = 0; match < knn_matches.size(); match++)
						{
							if (knn_matches[match][0].distance < ratio_thresh * knn_matches[match][1].distance)
							{
								good_matches.push_back(knn_matches[match][0]);
							}
						}
						if (dmax < good_matches.size()) {
							dmax = good_matches.size();
							Erme = erme;
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
			}
			stringstream ss;
			ss << Osszeg;
			string text;
			ss >> text;
			putText(atmeretezett, text, Point(10, 300), FONT_HERSHEY_SIMPLEX, 2.0, Scalar(200, 255, 200), 4);
			namedWindow("Coin Counter", WINDOW_AUTOSIZE);
			imshow("Coin Counter", atmeretezett);
			string mentes = std::to_string(++Kepszam) + ".jpg";
			imwrite(mentes, atmeretezett);
		}
	}
	else if (modszer == 4) {
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
			HoughCircles(gray, coin, HOUGH_GRADIENT, 1, 20, 85, 50, 29, 70);
			cout << "A korok szama : " << l << "\n\n";
			int Erme = 6;
			int Osszeg = 0;
			for (int i = 0; i < coin.size(); i++) {
				Point2f center(cvRound(coin[i][0]), cvRound(coin[i][1]));
				cout << "ez a kozepe: " << center << endl;
				int radius = cvRound(coin[i][2]);
				int FsX = coin[i][0] - (radius*0.7);
				int FsY = coin[i][1] - (radius*0.7);
				Rect r(FsX, FsY, 2 * (radius*0.7), 2 * (radius*0.7));
				Mat kor = atmeretezett(r);
				Mat tempresized;
				int dmax = 100000;
				Mat GrayIm, GrayTemp, Dscrp1, Dscrp2, korkp, Tempkp;
				std::vector<cv::KeyPoint> Kpts1, Kpts2;
				Ptr<ORB> orb = ORB::create(150000, 1.2f, 8, 5, 0, 2, ORB::HARRIS_SCORE, 5, 8);
				cvtColor(kor, GrayIm, COLOR_BGR2GRAY);
				orb->detectAndCompute(GrayIm, noArray(), Kpts1, Dscrp1);
				int db = 0, dg = 0, dr = 0, d = 0;
				for (int erme = 0; erme <= 5; erme++) {
					tempresized = Template[erme];
					cout << "ezt az ermet vizsgalom: " << erme << endl;
					for (int y = 0; y < kor.rows; y++) {
						for (int x = 0; x < kor.cols; x++) {
							db = abs(kor.at<Vec3b>(y, x)[0] - tempresized.at<Vec3b>(y, x)[0]);
							dg = abs(kor.at<Vec3b>(y, x)[1] - tempresized.at<Vec3b>(y, x)[1]);
							dr = abs(kor.at<Vec3b>(y, x)[2] - tempresized.at<Vec3b>(y, x)[2]);
							d = d + db + dg + dr;
						}
					}
					cvtColor(Template[erme], GrayTemp, COLOR_BGR2GRAY);
					orb->detectAndCompute(GrayTemp, noArray(), Kpts2, Dscrp2);
					Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
					vector<vector<DMatch> > knn_matches;
					matcher->knnMatch(Dscrp1, Dscrp2, knn_matches, 2);
					const float ratio_thresh = 0.75f;
					vector<DMatch> good_matches;
					for (size_t match = 0; match < knn_matches.size(); match++)
					{
						if (knn_matches[match][0].distance < ratio_thresh * knn_matches[match][1].distance)
						{
							good_matches.push_back(knn_matches[match][0]);
						}
					}
					int azonossag = d / (good_matches.size());
					cout << "talalatok szama :" << azonossag << endl;
					if (dmax > azonossag) {
						dmax = azonossag;
						Erme = erme;
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
			namedWindow("Coin Counter", WINDOW_AUTOSIZE);
			imshow("Coin Counter", atmeretezett);
			string mentes = std::to_string(++Kepszam) + ".jpg";
			imwrite(mentes, atmeretezett);
		}
	}
	waitKey();
	return 0;
}