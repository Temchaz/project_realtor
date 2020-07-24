#include "detector.hpp"
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
		if (ecdist < mindist && ecdist != 0.0) {
			mindist = ecdist;
			similarImg = img;
		}
	}
	imshow("similar image", similarImg);
}
