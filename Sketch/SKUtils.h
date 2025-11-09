#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

#ifndef M_ZERO
#define M_ZERO 1e-6
#endif

#include<vector>

using std::vector;

namespace SKUtils
{
	PROCESS_API void CreateConsole();

	PROCESS_API vector<double> linspace(double start, double end, int sec);  //sec>=3


}

