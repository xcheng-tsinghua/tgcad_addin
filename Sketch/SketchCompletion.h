#pragma once
#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

//#include <opencv2/flann/flann.hpp>
//#include <opencv2/opencv.hpp>
#include <vector>
//#include <string>
//
using std::vector;
//using std::string;
//using namespace cv;

class SKPnt_2d;

//class Mat;

class PROCESS_API SketchCompletion
{
public:
	SketchCompletion();
	~SketchCompletion();

	vector<vector<SKPnt_2d>> Infer(vector<vector<SKPnt_2d>> input_tensor);

private:
	//void ConstructDataset(const string& folder);
	//vector<vector<vector<SKPnt_2d>>> m_dataset_pnt;
	//vector<vector<SKPnt_2d>> loadSketchFromTxt(const string& path);
	//Mat renderSketch(const vector<vector<SKPnt_2d>>& strokes, int imgSize = 128, int thickness = 3);
	//Mat extractFeature(const Mat& img);
	//int findNearest(const Mat& queryFeature, const Mat& dbFeatures);
	//cv::Rect2f getBoundingBox(const vector<vector<SKPnt_2d>>& sketch);
	//vector<vector<SKPnt_2d>> transformSketch(const vector<vector<SKPnt_2d>>& full, const vector<vector<SKPnt_2d>>& user);



	// cv::Mat m_dataset_fea;1,994,591
	// 



};

