// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.cpp"
#include "room.hpp"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <opencv2\core\utils\filesystem.hpp>

void heuristic(const float _nmsThreshold, const float _probThreshold, cv::Mat& _img) {
	Detector model;
	std::vector<cv::Rect> boxes;
	std::vector<float> probs;
	std::vector<unsigned> classes;
	model.detect(_img, _nmsThreshold, _probThreshold, boxes, probs, classes);

	for (int i = 0; i < boxes.size(); ++i) {
		rectangle(_img, boxes[i], cv::Scalar(0, 0, 255));
		putText(_img, cv::format("%.u", classes[i]), cv::Point(boxes[i].x, boxes[i].y - 2),
			cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0)); //%.2f
		putText(_img, cv::format("%.2f", probs[i]), cv::Point(boxes[i].x + 30, boxes[i].y - 2),
			cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255)); //%.2f
	}
	imshow("Detection", _img);

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

	if (rooms.at(0)->highPriorityItem == 0 && rooms.at(0)->cntItems == 0) std::cout << "Room is not identified" << std::endl;
	else std::cout << "This room is " << rooms.at(0)->Name() << std::endl;
}

int main()
{
	const float nmsThreshold = 0.45f;
	const float probThreshold = 0.2f;
	std::string imageName;
	std::cout << "Input name of image (q for quit)\n>> ";
	std::cin >> imageName;
	while (imageName != "q") {
		cv::Mat img;
		switch (imageName.at(0)) {
		case ('k'):
			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/kitchen", imageName+".jpg"));
			break;
		case ('l'):
			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/livingroom", imageName+".jpg"));
			break;
		case ('b'):
			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/bathroom", imageName+ ".jpg"));
			break;
		default:
			break;
		}
		heuristic(nmsThreshold, probThreshold, img);
		cv::waitKey();
		std::cout << "Input name of image (q for quit)\n>> ";
		std::cin >> imageName;
	}
}