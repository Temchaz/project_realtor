// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.cpp"
#include "room.cpp"
#include <iostream>
#include <algorithm>

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
    
    Kitchenroom kitchen;
    Bathroom bathroom; 
    Livingroom livingroom;
    
    for (int i(0); i < classes.size(); i++) 
    {
        kitchen.countPriorityItem(classes[i]);
        kitchen.countItems(classes[i]);
        bathroom.countPriorityItem(classes[i]);
        bathroom.countItems(classes[i]);
        livingroom.countPriorityItem(classes[i]);
        livingroom.countItems(classes[i]);
    }

    std::vector<Room*> rooms;
    rooms.push_back(new Kitchenroom(kitchen));
    rooms.push_back(new Bathroom(bathroom));
    rooms.push_back(new Livingroom(livingroom));

    sort(rooms.begin(), rooms.end(),
        [](const Room* a, const Room* b)
        {
            if (a->highPriorityItem == b->highPriorityItem)
            {
                return a->cntItems > b->cntItems;
            }
            else return a->highPriorityItem > b->highPriorityItem;
        }
    );
    for (auto i : rooms) {
        std::cout << i->highPriorityItem << " " << i->cntItems <<std::endl;     
    }
    std::cout << rooms.at(0)->Name() << std::endl;


    cv::waitKey();
}