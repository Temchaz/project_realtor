// realtor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "detector.hpp"
#include "room.hpp"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <opencv2\core\utils\filesystem.hpp>

InferenceEngine::Blob::Ptr wrapMatToBlob1(const cv::Mat& m) {
	CV_Assert(m.depth() == CV_8U);
	std::vector<size_t> dims = { 1, (size_t)m.channels(), (size_t)m.rows, (size_t)m.cols };
	return InferenceEngine::make_shared_blob<uint8_t>(InferenceEngine::TensorDesc(InferenceEngine::Precision::U8, dims, InferenceEngine::Layout::NHWC),
		m.data);
}

class Embedding {
public:
	Embedding() {
		InferenceEngine::Core ie;

		// load deep learning network into memory
		auto net = ie.ReadNetwork(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/neuralNetworks", "model_caff2.xml"), //ssd_mobilenet_v2_coco
			cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/neuralNetworks", "model_caff2.bin"));
		// specify preprocessing procedures
		// (note: this part is different for different models!)
		auto inputinfo = net.getInputsInfo()["input.1"];
		inputinfo->getPreProcess().setResizeAlgorithm(InferenceEngine::ResizeAlgorithm::RESIZE_BILINEAR);
		inputinfo->setLayout(InferenceEngine::Layout::NCHW);
		inputinfo->setPrecision(InferenceEngine::Precision::U8);

		auto output = net.getOutputsInfo();
		for (auto& item : output) {
			auto& outputData = item.second;
			outputData->setPrecision(InferenceEngine::Precision::FP32);
			outputData->setLayout(InferenceEngine::Layout::NCHW);
		}
		outputName = net.getOutputsInfo().begin()->first;

		// initialize runnable object on cpu device
		InferenceEngine::ExecutableNetwork execnet = ie.LoadNetwork(net, "CPU");

		// create a single processing thread
		req = execnet.CreateInferRequest();
	}

	std::vector<float> embed(const cv::Mat& image) {
		// Create 4D blob from BGR image
		InferenceEngine::Blob::Ptr input = wrapMatToBlob1(image);

		// Pass blob as network's input. "data" is a name of input from .xml file
		req.SetBlob("input.1", input);
		// Launch network
		req.Infer();

		// Copy output. "prob" is a name of output from .xml file
		float* outPut = req.GetBlob(outputName)->buffer().as<float*>();
		std::vector<float> outVec(req.GetBlob(outputName)->size());
		std::copy(outPut, outPut + outVec.size(), outVec.begin());
		return outVec;
	}
private:
	InferenceEngine::InferRequest req;
	std::string outputName;
};

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

//int main()
//{
//	const float nmsThreshold = 0.45f;
//	const float probThreshold = 0.2f;
//	std::string imageName;
//	std::cout << "Input name of image (q for quit)\n>> ";
//	std::cin >> imageName;
//	while (imageName != "q") {
//		cv::Mat img;
//		switch (imageName.at(0)) {
//		case ('k'):
//			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/kitchen", imageName+".jpg"));
//			break;
//		case ('l'):
//			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/livingroom", imageName+".jpg"));
//			break;
//		case ('b'):
//			img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data/bathroom", imageName+ ".jpg"));
//			break;
//		default:
//			break;
//		}
//		heuristic(nmsThreshold, probThreshold, img);
//		cv::waitKey();
//		std::cout << "Input name of image (q for quit)\n>> ";
//		std::cin >> imageName;
//	}
//}

void emb_func(std::string inputPath, std::string inputName) {
	Embedding embedding;
	std::string imageName;
	cv::Mat embInput = cv::imread(cv::utils::fs::join(inputPath, inputName + ".jpg"));
	std::vector<float> inputVec = embedding.embed(embInput);

	std::vector<float> imageVec;
	float dist;
	float ecdist;
	float mindist(100000);
	cv::Mat similarImg;
	for (const auto& entry : std::filesystem::directory_iterator("C:/Users/79308/Desktop/projectRealtor/data3")) {
		imageName = entry.path().filename().string();
		cv::Mat img = cv::imread(cv::utils::fs::join("C:/Users/79308/Desktop/projectRealtor/data3", imageName));
		imageVec = embedding.embed(img);
		dist = 0;
		for (int i = 0; i < inputVec.size(); i++) {
			dist += pow((inputVec.at(i) - imageVec.at(i)), 2);
		}
		ecdist = sqrt(dist);
		if (ecdist < mindist && ecdist != 0) {
			mindist = ecdist;
			similarImg = img;
		}
		
	}
	imshow("similar image", similarImg);
}

int main() {
	std::cout << "input name of your image" << std::endl;
	std::string yourImg;
	std::cin >> yourImg;
	
	switch (yourImg.at(0)) {
	case ('k'):
		emb_func("C:/Users/79308/Desktop/projectRealtor/data/kitchen", yourImg);
		break;
	case ('l'):
		emb_func("C:/Users/79308/Desktop/projectRealtor/data/livingroom", yourImg);
		break;
	case ('b'):
		emb_func("C:/Users/79308/Desktop/projectRealtor/data/bathroom", yourImg);
		break;
	default:
		break;
	}

	cv::waitKey();
}