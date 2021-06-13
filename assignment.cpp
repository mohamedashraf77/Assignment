#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

// apply 3x3 sopil filter with strid 3
Mat sobil_filter(Mat img)
{
    Mat outImg(img.rows, img.cols, CV_8UC1);

    for (int w = 0; w < img.rows; w++)
        for (int h = 0; h < img.cols; h++)
            outImg.at<uchar>(w, h) = 0;
    
    char filter_h[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };  // sobil_x
    char filter_v[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };  // sobil_Y
    
    for (int w = 0; w < img.rows; w += 3)
        for (int h = 0; h < img.cols; h += 3)
        {
            float change_h = 0;
            float change_v = 0;
            for (int i = 0; i < 3; i++)
            {
                if ((w - 1 + i) < 0 || (w - 1 + i) >= img.rows)
                    continue;
                for (int j = 0; j < 3; j++)
                {
                    if ((h - 1 + j) < 0 || (h - 1 + j) >= img.cols)
                        continue;
                    change_h += filter_h[i][j] * img.at<uchar>((w - 1 + i), (h - 1 + j));
                    change_v += filter_v[i][j] * img.at<uchar>((w - 1 + i), (h - 1 + j));
                }

            }
            if (abs(change_h) != 0)
                outImg.at<uchar>(w, h) = 255;
            else if (abs(change_v) != 0)
                outImg.at<uchar>(w, h) = 255;
        }
    return outImg;
}

Mat avg_filter(Mat img, int kernel_size)
{
    Mat outImg(img.rows, img.cols, CV_8UC1);

    for (int w = 0; w < img.rows; w++)
        for (int h = 0; h < img.cols; h++)
            outImg.at<uchar>(w, h) = img.at<uchar>(w, h);

    int kernel_p = kernel_size / 2; //width of filter
    for (int w = 0; w < outImg.rows; w++)
        for (int h = 0; h < outImg.cols; h++)
        {
            float avg = 0;
            for (int i = 0; i < kernel_size; i++)
            {
                if ((w - kernel_p + i) < 0 || (w - kernel_p + i) >= outImg.rows)
                    continue;
                for (int j = 0; j < kernel_size; j++)
                {
                    if ((h - kernel_p + j) < 0 || (h - kernel_p + j) >= outImg.cols)
                        continue;
                    avg += outImg.at<uchar>((w - kernel_p + i), (h - kernel_p + j));
                }

            }

            outImg.at<uchar>(w, h) = avg / (kernel_size * kernel_size);
        }
    return outImg;
}

Mat orange_filter(Mat img)
{
    for (int w = 0; w < img.rows; w++)
        for (int h = 0; h < img.cols; h++)
            if (img.at<Vec3b>(w, h)[0] >= 40
                ||img.at<Vec3b>(w, h)[1] >= 150 || img.at<Vec3b>(w, h)[1] <= 60
                ||img.at<Vec3b>(w, h)[2] <= 200)
                
                img.at<Vec3b>(w, h) = { 0,0,0 };
    return img;

}

void split_chanels(Mat img, Mat *chanels)
{
    for (int i = 0; i < 3; i++)
        chanels[i] = Mat(img.rows, img.cols, CV_8UC1);
    for (int w = 0; w < img.rows; w++)
        for (int h = 0; h < img.cols; h++)
        {
            chanels[0].at<uchar>(w, h) = img.at<Vec3b>(w, h)[0];
            chanels[1].at<uchar>(w, h) = img.at<Vec3b>(w, h)[1];
            chanels[2].at<uchar>(w, h) = img.at<Vec3b>(w, h)[2];
        }
}

void merge_chanels(Mat img, Mat* chanels)
{
    for (int w = 0; w < img.rows; w++)
        for (int h = 0; h < img.cols; h++)
        {
            img.at<Vec3b>(w, h)[2] = chanels[0].at<uchar>(w, h);
            img.at<Vec3b>(w, h)[1] = chanels[1].at<uchar>(w, h);
            img.at<Vec3b>(w, h)[0] = chanels[2].at<uchar>(w, h);
        }
}

int main()
{
    string path = "C:/Users/muham/Downloads/Techncial assignment/image-1.png";
    
    // TASK1

    Mat img = imread(path); //read image
    imshow("input", img);
    
    Mat chanels[3];
    split_chanels(img, chanels); //split image to rgb chanels

    // apply average and sobel filter for each chanel
    for (int i = 0; i < 3; i++)
    {
        int kernel_size = 3;

        Mat avg_img = avg_filter(chanels[i], kernel_size);

        Mat outImg = sobil_filter(avg_img);

        chanels[i] = outImg;
    }
    Mat outImg(img.rows, img.cols, CV_8UC3);
    merge_chanels(outImg, chanels); //merge chanels
    imshow("out", outImg);

    
    //TASK2
    Mat img1 = imread(path);
    Mat orange = orange_filter(img1);
    imshow("orange", orange);
    waitKey(0);
    
    return 0;
}
