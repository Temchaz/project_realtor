// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.cpp"

#include <iostream>


struct room {
    int highPriorityItem = 0;
    int cntItems = 0;
};

//82 - refrigerator
//79 - oven
//78 - microwave
//81 - sink
//80 - toaster
//67 -   dining table
bool isKitchenroom(unsigned& ptr)
{
    if (ptr == 79 || ptr == 78 || ptr == 67 || ptr == 80)
        return true;
    else return false;
}

//69 - desk
//62 - chair
//63 - couch
//72 - tv
//65 - bed
bool isLivingroom(unsigned& ptr)
{
    if (ptr == 69 || ptr == 62 || ptr == 72)
        return true;
    else return false;
}

// 70- toilet
// 81 - sink
//90 -     toothbrush
bool isbathroom(unsigned& ptr)
{
    if (ptr == 81 || ptr == 90)
        return true;
    else return false;
}

bool isHighPriorityKitchen(unsigned& ptr) {
    if (ptr == 82) return true;
    else return false;
}

bool isHighPriorityLivingroom(unsigned& ptr) {
    if (ptr == 63) return true;
    else return false;
}

bool isHighPriorityBathroom(unsigned& ptr) {
    if (ptr == 70) return true;
    else return false;
}

int main()
{
    const float nmsThreshold = 0.45f;
    const float probThreshold = 0.2f;

    cv::Mat img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data", "v3.jpg"));

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
    
    room kitchen, bathroom, livingroom;
    
    for (int i(0); i < classes.size(); i++) 
    {
        if (isHighPriorityKitchen(classes[i])) kitchen.highPriorityItem++;
        if (isHighPriorityBathroom(classes[i])) bathroom.highPriorityItem++;
        if (isHighPriorityLivingroom(classes[i])) livingroom.highPriorityItem++;
    }

    for (int i = 0; i < classes.size(); i++)
    {
        if (isbathroom(classes[i]))
            bathroom.cntItems++;
        if (isLivingroom(classes[i]))
            livingroom.cntItems++;
        if (isKitchenroom(classes[i]))
            kitchen.cntItems++;
    }    
    
    // Determine the room by the count of high priority items
    if (kitchen.highPriorityItem > bathroom.highPriorityItem && kitchen.highPriorityItem > livingroom.highPriorityItem)
        std::cout << "kitchen" << std::endl;
    else if (bathroom.highPriorityItem > kitchen.highPriorityItem && bathroom.highPriorityItem > livingroom.highPriorityItem)
        std::cout << "bathroom" << std::endl;
    else if (livingroom.highPriorityItem > kitchen.highPriorityItem && livingroom.highPriorityItem > bathroom.highPriorityItem)
        std::cout << "livingroom" << std::endl;
    // The number of the high priority items are equal for all rooms
    else if (kitchen.highPriorityItem == livingroom.highPriorityItem && bathroom.highPriorityItem == livingroom.highPriorityItem && kitchen.highPriorityItem != 0)
        std::cout << "room is not identified" << std::endl;
    // No high high priority items, check by other items
    else if (kitchen.highPriorityItem == 0 && bathroom.highPriorityItem == 0 && livingroom.highPriorityItem == 0)
    {
        if (kitchen.cntItems > bathroom.cntItems && kitchen.cntItems > livingroom.cntItems)
        {
            std::cout << "kitchen" << std::endl;
        }
        else if (bathroom.cntItems > kitchen.cntItems && bathroom.cntItems > livingroom.cntItems)
        {
            std::cout << "bathroom" << std::endl;
        }
        else if (livingroom.cntItems > kitchen.cntItems && livingroom.cntItems > bathroom.cntItems)
        {
            std::cout << "livingroom" << std::endl;
        }
        else std::cout << "room is not identified" << std::endl;
    }
    // The number of the high priority items are equal for two rooms
    else if (kitchen.highPriorityItem == bathroom.highPriorityItem)
    {
        if (kitchen.cntItems > bathroom.cntItems) std::cout << "kitchen" << std::endl;
        else if (kitchen.cntItems < bathroom.cntItems) std::cout << "bathroom" << std::endl;
        else std::cout << "room is not identified" << std::endl;
    }
    else if (kitchen.highPriorityItem == livingroom.highPriorityItem)
    {
        if (kitchen.cntItems > livingroom.cntItems) std::cout << "kitchen" << std::endl;
        else if (kitchen.cntItems < livingroom.cntItems) std::cout << "livingroom" << std::endl;
        else std::cout << "room is not identified" << std::endl;
    }
    else if (bathroom.highPriorityItem == livingroom.highPriorityItem)
    {
        if (bathroom.cntItems > livingroom.cntItems) std::cout << "bathroom" << std::endl;
        else if (bathroom.cntItems < livingroom.cntItems) std::cout << "livingroom" << std::endl;
        else std::cout << "room is not identified" << std::endl;
    }

    cv::waitKey();
}