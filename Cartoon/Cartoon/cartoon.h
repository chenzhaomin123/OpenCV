#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

#define WINDOW_NAME "卡通化效果图"

const int MEDIAN_BLUR_SIZE = 7;
const int LAPLACIAN_FILTER_SIZE = 5;
const int EDGE_THRESHOLD = 80;
const int BILATERAL_FILTER_KSIZE = 5;
const int BILATERAL_FILTER_COLOR = 18;
const int BILATERAL_FILTER_SPACE = 7;
void edgeDetection(Mat& srcImage, Mat& edge);//检测边缘函数
void cartoonlization(Mat& srcImage, Mat& cartoon);//卡通化处理函数
void equalizationImage(Mat& srcImage, Mat& dstImage);