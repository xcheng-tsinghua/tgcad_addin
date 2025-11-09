#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

#include <vector>

using std::vector;

class PROCESS_API SketchRNN
{
public:
	SketchRNN();
	~SketchRNN();

	static vector<vector<double>> Infer(vector<vector<double>> input_tensor);

};

