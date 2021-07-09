
#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "opencv2/video/tracking.hpp"
#include <opencv2/highgui.hpp>
#include <math.h>


void recogniseStickersByThreshold(cv::Mat image, std::vector<std::vector<cv::Point>> &stickers) {
    cv::Mat image_hsv;
    std::vector< std::vector<cv::Point> > contours;
    cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV ); 
    cv::Mat tmp_img(image.size(), CV_8U);
    cv::inRange(image_hsv, cv::Scalar(100, 100, 100), cv::Scalar(255, 255, 255), tmp_img);
    cv::dilate(tmp_img,tmp_img,cv::Mat(), cv::Point(-1,-1), 3);
    cv::erode(tmp_img,tmp_img,cv::Mat(), cv::Point(-1,-1), 1);
    cv::findContours(tmp_img, stickers, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    for (uint i = 0; i<contours.size(); i++) {
        cv::Mat sticker;
        cv::Rect rect=cv::boundingRect(contours[i]);
        image(rect).copyTo(sticker);
        cv::rectangle(image, rect, cv::Scalar(0, 250, 0), 2);
        stickers.push_back(sticker); 
    }
}

bool cmpPointX(const cv::Point &a, const cv::Point &b) {
    return a.x < b.x;
}

bool cmpPointY(const cv::Point &a, const cv::Point &b) {
    return a.y < b.y;
}

static bool stickers_comp(std::vector<cv::Point>& st1, std::vector<cv::Point>& st2) {
    int first_x = abs((*min_element(st1.begin(), st1.end(), cmpPointX)).x - (*max_element(st1.begin(), st1.end(), cmpPointX)).x);
    int first_y = abs((*min_element(st1.begin(), st1.end(), cmpPointY)).y - (*max_element(st1.begin(), st1.end(), cmpPointY)).y);

    int sec_x = abs((*min_element(st2.begin(), st2.end(), cmpPointX)).x - (*max_element(st2.begin(), st2.end(), cmpPointX)).x);
    int sec_y = abs((*min_element(st2.begin(), st2.end(), cmpPointY)).y - (*max_element(st2.begin(), st2.end(), cmpPointY)).y);

    return (first_x * first_y < sec_x * sec_y);
}

int main() {
    using namespace cv;
    using namespace std;

    VideoCapture cap("circle.mp4");
    if (!cap.isOpened()) {
        return -1;
    }

    cap.set(CAP_PROP_POS_MSEC, 10000);
    namedWindow("Detection",cv::WindowFlags::WINDOW_AUTOSIZE); 

    std::vector<std::vector<cv::Point>> stickers;
    std::vector<cv::Point> trajectory;

    // Kalman Filter for tracking
    KalmanFilter KF(4, 2, 0);
    KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
    Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));
    KF.statePre.at<float>(0) = 0;
    KF.statePre.at<float>(1) = 0;
    KF.statePre.at<float>(2) = 0;
    KF.statePre.at<float>(3) = 0;
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(KF.errorCovPost, Scalar::all(.1));

    while(1) {
        Mat frame;
        bool bSuccess = cap.read(frame); 
        if (!bSuccess) {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }

        recogniseStickersByThreshold(frame, stickers);
        auto st = max_element(stickers.begin(), stickers.end(), stickers_comp);

        cv::Point sticker1; 
        sticker1.x = (*min_element((*st).begin(), (*st).end(), cmpPointX)).x;
        sticker1.y = (*min_element((*st).begin(), (*st).end(), cmpPointY)).y;
        cv::Point sticker2; 
        sticker2.x = (*max_element((*st).begin(), (*st).end(), cmpPointX)).x;
        sticker2.y = (*max_element((*st).begin(), (*st).end(), cmpPointY)).y;

        Mat prediction = KF.predict();
        Point predictPt(prediction.at<float>(0),prediction.at<float>(1));

        measurement(0) = sticker1.x + abs(sticker1.x - sticker2.x) / 2;
        measurement(1) = sticker1.y + abs(sticker1.y - sticker2.y) / 2;
        Point measPt(measurement(0),measurement(1));

        Mat estimated = KF.correct(measurement);
        Point statePt(estimated.at<float>(0),estimated.at<float>(1));
        trajectory.push_back(statePt);

        for (auto point = trajectory.begin(); point < trajectory.end() - 1; ++point) {
            line(frame, point[0], point[1], Scalar(0,0,255), 3, LINE_AA, 0);
        }

        cv::rectangle(frame, Rect(sticker1, sticker2),cv::Scalar(0,250,0),2);

        imshow("Detection", frame); 
        if(waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}
