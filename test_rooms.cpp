#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "detector.hpp"
#include "room.hpp"
#include <opencv2\core\utils\filesystem.hpp>

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

void test_main(std::string path)
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
		if (tolower(imageName.at(0)) == tolower(type_of_room.at(0)))
		{
			count_true_prediction++;
		}
		else if (type_of_room == "Room is not identified")
		{
			count_failed_prediction++;
		}
		else {
			count_false_prediction++;
		}
	}
	int count_images = count_true_prediction + count_false_prediction + count_failed_prediction;
	std::cout << "Count of images = " << count_images << std::endl;
	std::cout << "Count of correctly recognized images = " << count_true_prediction << std::endl;
	std::cout << "Count of incorrectly recognized images = " << count_false_prediction << std::endl;
	std::cout << "Count of unclassified images = " << count_failed_prediction << std::endl;
	std::cout << "Accuracy" << " = " << std::setprecision(2) << (float)count_true_prediction / count_images << std::endl;
	cv::waitKey();
}

int main() {
	test_main("C:/Users/79308/Desktop/projectRealtor/data3");
}