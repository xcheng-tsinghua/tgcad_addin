#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif


#include "SKPnt_2d.h"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using std::vector;
using std::string;
using cv::Mat;

class PROCESS_API SketchCompletion
{
public:
	SketchCompletion(const std::string& folder_root = "C:\\Users\\ChengXi\\Desktop\\sketchrnn_proj_txt");
	~SketchCompletion();

	vector<vector<SKPnt_2d>> Infer(vector<vector<SKPnt_2d>> partial_sketch);


private:
	vector<vector<vector<SKPnt_2d>>> m_dataset_point;
	cv::Mat m_dataset_feature;

	vector<vector<SKPnt_2d>> transformSketch(const vector<vector<SKPnt_2d>>& full, const vector<vector<SKPnt_2d>>& user);

	vector<vector<vector<SKPnt_2d>>> loadDataset(const string& folder);

	vector<vector<SKPnt_2d>> loadSketchFromTxt(const string& path);

	Mat renderSketch(const vector<vector<SKPnt_2d>>& strokes, int imgSize = 128, int thickness = 3);

	Mat extractFeature(const Mat& img);

	int findNearest(const Mat& queryFeature, const Mat& dbFeatures);

	cv::Rect2f getBoundingBox(const vector<vector<SKPnt_2d>>& sketch);


};




