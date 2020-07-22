// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.cpp"
#include "room.cpp"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>

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

std::string test_func(const float _nmsThreshold, const float _probThreshold, cv::Mat& _img) {
	Detector model;
	std::vector<cv::Rect> boxes;
	std::vector<float> probs;
	std::vector<unsigned> classes;
	model.detect(_img, _nmsThreshold, _probThreshold, boxes, probs, classes);
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

	if (rooms.at(0)->highPriorityItem == 0 && rooms.at(0)->cntItems == 0) return "Room is not identified";
	else return rooms.at(0)->Name();
}

void test_main(std::string path, std::string trueTypeRoom, std::string falseTypeRoom1, std::string falseTypeRoom2)
{
	const float nmsThreshold = 0.45f;
	const float probThreshold = 0.2f;
	int count_true_prediction = 0;
	int count_false_prediction = 0;
	int count_failed_prediction = 0;
	std::string imageName;
	std::string type_of_room;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		imageName = entry.path().filename().string();
		cv::Mat img = cv::imread(cv::utils::fs::join(path, imageName));
		type_of_room = test_func(nmsThreshold, probThreshold, img);
		if (type_of_room == trueTypeRoom)
		{
			count_true_prediction++;
		}
		else if (type_of_room == falseTypeRoom1 || type_of_room == falseTypeRoom2)
		{
			count_false_prediction++;
		}
		else if (type_of_room == "Room is not identified") {
			count_failed_prediction++;
		}
	}
	int count_images = count_true_prediction + count_false_prediction + count_failed_prediction;
	std::cout << "Count of images = " << count_images << std::endl;
	std::cout << "Count of correctly recognized images = " << count_true_prediction << std::endl;
	std::cout << "Count of incorrectly recognized images = " << count_false_prediction << std::endl;
	std::cout << "Count of unclassified images = " << count_failed_prediction << std::endl;
	std::cout << "Accuracy for our " << trueTypeRoom + "s" << " = " << std::setprecision(2) << (float)count_true_prediction / count_images << std::endl;
	cv::waitKey();
}

int main()
{
	const float nmsThreshold = 0.45f;
	const float probThreshold = 0.2f;
	std::string imageName;
	char choose;

	std::cout << "Press 1 to run program\nPress 2 to run test\nPress q to quit\n>> ";
	std::cin >> choose;
	while (choose != 'q') {
		switch (choose) {
		case ('1'):
			while (1) {
				std::cout << "Input name of image (q for quit)\n>> ";
				std::cin >> imageName;
				if (imageName == "q") {
					break;
				}

				cv::Mat img;
				switch (imageName.at(0)) {
				case ('k'):
					img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/kitchen", imageName));
					break;
				case ('l'):
					img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/livingroom", imageName));
					break;
				case ('b'):
					img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/bathroom", imageName));
					break;
				default:
					break;
				}

				heuristic(nmsThreshold, probThreshold, img);
				cv::waitKey();
			}
			break;
		case ('2'):
			std::cout << "Choose type of room for test: \n 1-kitchens\n 2-livingrooms \n 3-bathrooms\n q-step back\n>> ";
			char ch;
			std::cin >> ch;
			switch (ch) {
			case ('1'):
				test_main("C:/Users/79308/Desktop/projectRealtor/data/kitchen", "Kitchenroom", "Livingroom", "Bathroom");
				break;
			case ('2'):
				test_main("C:/Users/79308/Desktop/projectRealtor/data/livingroom", "Livingroom", "Kitchenroom", "Bathroom");
				break;
			case('3'):
				test_main("C:/Users/79308/Desktop/projectRealtor/data/bathroom", "Bathroom", "Kitchenroom", "Livingroom");
				break;
			case('q'):
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		std::cout << std::endl;
		std::cout << "Press 1 to run program\nPress 2 to run test\nPress q to quit\n>> ";
		std::cin >> choose;
	}
	std::cout << "bay bay"<<std::endl;
}