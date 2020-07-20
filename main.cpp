// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.cpp"

#include <iostream>
//#include <opencv2/core/utils/filesystem.hpp>
//#include <opencv2/opencv.hpp>
//#include <inference_engine.hpp>

//using namespace cv;
//using namespace detect;

//82 - refrigerator
//79 - oven
//78 - microwave
//81 - sink
//80 - toaster
//67 -   dining table
bool isKitchenroom(unsigned& ptr)
{
    if (ptr == 82 || ptr == 79 || ptr == 78 || ptr == 67 || ptr == 80)
        return true;
    else return false;
}

//69 - desk
//67 -dining table
//62 - chair
//63 - couch
//72 - tv
//65 - bed

bool isLivingroom(unsigned& ptr)
{
    if (ptr == 69 || ptr == 67 || ptr == 62 || ptr == 72 || ptr == 63 || ptr == 65)
        return true;
    else return false;
}
// 70- toilet
// 81 - sink
//90 -     toothbrush

bool isbathroom(unsigned& ptr)
{
    if (ptr == 70 || ptr == 81 || ptr == 90)
        return true;
    else return false;
}

int main()
{
    const float nmsThreshold = 0.45f;
    const float probThreshold = 0.2f;

    cv::Mat img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data", "g1.jpg"));

    Detector model;
    std::vector<cv::Rect> boxes;
    std::vector<float> probs;
    std::vector<unsigned> classes;
    model.detect(img, nmsThreshold, probThreshold, boxes, probs, classes);

    for (int i = 0; i < boxes.size(); ++i) {
        rectangle(img, boxes[i], cv::Scalar(0, 0, 255));
        putText(img, cv::format("%.u", classes[i]), cv::Point(boxes[i].x, boxes[i].y - 2),
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0)); //%.2f
        putText(img, cv::format("%.2f", probs[i]), cv::Point(boxes[i].x+30, boxes[i].y - 2),
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255)); //%.2f
    }
    imshow("Detection", img);
    
    int kitchen(0), bathroom(0), livingroom(0);
    for (int i = 0; i < classes.size(); i++)
    {
        if (isbathroom(classes[i]))
            bathroom++;
        if (isLivingroom(classes[i]))
            livingroom++;
        if (isKitchenroom(classes[i]))
            kitchen++;

    }    
    //std::cout << bathroom << " " << kitchen << " " << livingroom << std::endl;
    if (kitchen > bathroom && kitchen > livingroom)
    {
        std::cout<< "kitchen"<< std::endl;
    }
    else if (bathroom > kitchen && bathroom > livingroom)
    {
        std::cout << "bathroom" << std::endl;
    }
    else if (livingroom > kitchen && livingroom > bathroom)
    {
        std::cout << "livingroom" << std::endl;
    }
    else if (kitchen == bathroom && kitchen == livingroom && bathroom == livingroom ) {
        std::cout << "room is not identified" << std::endl;
    }
    else if (kitchen == bathroom) {
        for (int i(0); i < classes.size(); i++) {
            if (classes[i] == 82) {
                std::cout << "kitchen" << std::endl;
            }
            else if (classes[i] == 70) {
                std::cout << "bathroom" << std::endl;
            }
        }
    }
    else if (kitchen == livingroom) {
        for (int i(0); i < classes.size(); i++) {
            if (classes[i] == 65) {
                std::cout << "livingroom" << std::endl;
            }
            else if (classes[i] == 82) {
                std::cout << "kitchen" << std::endl;
            }
        }
    }
    else if (bathroom == livingroom) {
        for (int i(0); i < classes.size(); i++) {
            if (classes[i] == 65) {
                std::cout << "livingroom" << std::endl;
            }
            else if (classes[i] == 70) {
                std::cout << "bathroom" << std::endl;
            }
        }
    }
    
    cv::waitKey();
}