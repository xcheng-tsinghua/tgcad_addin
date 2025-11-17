#include "SketchCompletion.h"
#include "SKPnt_2d.h"


SketchCompletion::SketchCompletion()
{

}

SketchCompletion::~SketchCompletion()
{

}

vector<vector<SKPnt_2d>> SketchCompletion::Infer(vector<vector<SKPnt_2d>> input_tensor)
{
	double tmp_move = 30;

	vector<vector<SKPnt_2d>> transed_sketch;
	for (const auto& c_stk : input_tensor)
	{
		vector<SKPnt_2d> moved_stk;

		for (const auto& c_pnt : c_stk)
		{
			moved_stk.push_back(c_pnt + SKPnt_2d(tmp_move, tmp_move));
		}
		transed_sketch.push_back(moved_stk);

	}

	return transed_sketch;

}

