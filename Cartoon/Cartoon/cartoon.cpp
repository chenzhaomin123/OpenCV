#include"cartoon.h"
void cartoonlization(Mat& srcImage, Mat& displayImage)
{
	displayImage.create(srcImage.size(),srcImage.type());
	Mat edge;
	edgeDetection(srcImage,edge);
	//阈值化图像,得到黑白素描图像，白色为背景，以便当遮罩使用
	Mat mask;
	threshold(edge,mask,EDGE_THRESHOLD,255,THRESH_BINARY_INV);
	//先开运算再闭运算
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(mask, mask, MORPH_OPEN, element);
	morphologyEx(mask, mask, MORPH_CLOSE, element);


	//由于双边滤波效率太低，导致实时处理速度太慢，这里对图像分辨率减少为原来的1/16以提高速度
	Size normalSize = srcImage.size();
	Size smallSize;
	smallSize.width = normalSize.width / 4;
	smallSize.height = normalSize.height / 4;
	Mat smallImage = Mat(smallSize,srcImage.type());
	resize(srcImage,smallImage,smallSize,0,0,INTER_LINEAR);
	
	//使用小内核的滤波器进行多次滤波取代大内核的滤波器，在保证效果的前提下提高速度
	Mat tmp = Mat(smallSize,srcImage.type());
	for (int i = 0; i < 7; i++){
		bilateralFilter(smallImage, tmp, BILATERAL_FILTER_KSIZE, BILATERAL_FILTER_COLOR, BILATERAL_FILTER_SPACE);
		bilateralFilter(tmp, smallImage, BILATERAL_FILTER_KSIZE, BILATERAL_FILTER_COLOR, BILATERAL_FILTER_SPACE);
	}
	Mat normalImage;
	resize(smallImage,normalImage,normalSize,0,0,INTER_LINEAR);
	Mat equaledImage;
	equalizationImage(normalImage,equaledImage);

	/*Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(equaledImage, equaledImage, MORPH_OPEN, element);
	morphologyEx(equaledImage, equaledImage, MORPH_CLOSE, element);*/

	displayImage = Scalar::all(0);
	equaledImage.copyTo(displayImage, mask);
}
void edgeDetection(Mat& srcImage,Mat& edge)
{
	Mat gray;
	//Laplacian边缘滤波器只能处理灰度图像，首先对图像颜色空间进行转换
	cvtColor(srcImage, gray, COLOR_BGR2GRAY);
	//调用中值滤波（在使用Laplacian边缘滤波器之前对图像进行去噪，使用中值滤波可以得到很好的去噪效果并且可以使边缘锐化，而且效率高）
	medianBlur(gray,gray,MEDIAN_BLUR_SIZE);
	//GaussianBlur(gray,gray,Size(7,7),0,0);
	//对灰度图像进行边缘检测，得到图像的边缘
	Laplacian(gray,edge,CV_8U,LAPLACIAN_FILTER_SIZE);

}

void equalizationImage(Mat& srcImage, Mat& dstImage)
{
	vector<Mat> channels;
	Mat blueChannel;
	Mat greenChannel;
	Mat redChannel;

	split(srcImage,channels);
	blueChannel = channels.at(0);
	greenChannel = channels.at(1);
	redChannel = channels.at(2);

	equalizeHist(blueChannel,blueChannel);
	equalizeHist(greenChannel, greenChannel);
	equalizeHist(redChannel, redChannel);

	merge(channels,dstImage);
}