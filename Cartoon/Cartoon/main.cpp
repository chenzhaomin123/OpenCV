#include"cartoon.h"


int main()
{
	/*Mat img = imread("20.jpg");
	Mat cartoonImage;
	cartoonlization(img, cartoonImage);
	imshow("1", cartoonImage);
	waitKey(0);*/
	VideoCapture camera(0);//调用摄像头
	Mat displayImage;
	if (!camera.isOpened()){
		cerr << "Camera can not open!!" << endl;
		exit(0);
	}
	while (true){
		Mat frame;
		Mat cartoonImage;
		camera >> frame;
		if (frame.empty()){
			continue;
		}
		displayImage.create(frame.size().height,frame.size().width*2,frame.type());
		cartoonlization(frame, cartoonImage);
		//一个窗口同时显示没处理的图像和卡通化处理之后的图像
		Mat imageROI1 = displayImage(Rect(0,0,displayImage.size().width/2,displayImage.size().height));
		Mat imageROI2 = displayImage(Rect(displayImage.size().width / 2, 0, displayImage.size().width / 2, displayImage.size().height));
		frame.copyTo(imageROI1);
		cartoonImage.copyTo(imageROI2);
		imshow(WINDOW_NAME, displayImage);
		char key = waitKey(24);
		//ESC退出
		if (key == 27){
			break;
		}
	}
	
	return 0;
}
